<?php

try {
    $ydb = FFI::load("build/ydb-c-sdk.i");
} catch (FFI\Exception $e) {
    echo "Generate ffi data using\n$ mkdir build; cd build; cmake ..\n";
    exit(1);
}

require_once "data.php";

function CreateSeries($session, $data) {
    global $ydb;

    $query = <<<END
        CREATE TABLE series (
            series_id Uint64,
            title Utf8,
            series_info Utf8,
            release_date Uint64,
            PRIMARY KEY (series_id)
        );
    END;

    return $ydb->YdbQueryResultAsStatus(
        $ydb->YdbExecuteQuerySync($session, $query, NULL, $ydb->new('YdbParams')));
}

function CreateSeasons($session, $data) {
    global $ydb;

    $query = <<<END
        CREATE TABLE seasons (
            series_id Uint64,
            season_id Uint64,
            title Utf8,
            first_aired Uint64,
            last_aired Uint64,
            PRIMARY KEY (series_id, season_id)
        );
    END;

    return $ydb->YdbQueryResultAsStatus(
        $ydb->YdbExecuteQuerySync($session, $query, NULL, $ydb->new('YdbParams')));
}

function CreateEpisodes($session, $data) {
    global $ydb;

    $query = <<<END
        CREATE TABLE episodes (
            series_id Uint64,
            season_id Uint64,
            episode_id Uint64,
            title Utf8,
            air_date Uint64,
            PRIMARY KEY (series_id, season_id, episode_id)
        );
    END;

    return $ydb->YdbQueryResultAsStatus(
        $ydb->YdbExecuteQuerySync($session, $query, NULL, $ydb->new('YdbParams')));
}

function DropSeries($session, $data) {
    global $ydb;

    $query = "DROP TABLE series";
    return $ydb->YdbQueryResultAsStatus(
        $ydb->YdbExecuteQuerySync($session, $query, NULL, $ydb->new('YdbParams')));
}

function DropSeasons($session, $data) {
    global $ydb;

    $query = "DROP TABLE seasons";
    return $ydb->YdbQueryResultAsStatus(
        $ydb->YdbExecuteQuerySync($session, $query, NULL, $ydb->new('YdbParams')));
}

function DropEpisodes($session, $data) {
    global $ydb;

    $query = "DROP TABLE episodes";
    return $ydb->YdbQueryResultAsStatus(
        $ydb->YdbExecuteQuerySync($session, $query, NULL, $ydb->new('YdbParams')));
}

function FillData($session, $data) {
    global $ydb;

    $query = <<<END
        DECLARE \$seriesData AS List<Struct<
        series_id: Uint64,
        title: Utf8,
        series_info: Utf8,
        release_date: Date>>;
        
        DECLARE \$seasonsData AS List<Struct<
        series_id: Uint64,
        season_id: Uint64,
        title: Utf8,
        first_aired: Date,
        last_aired: Date>>;
        
        DECLARE \$episodesData AS List<Struct<
        series_id: Uint64,
        season_id: Uint64,
        episode_id: Uint64,
        title: Utf8,
        air_date: Date>>;
        
        REPLACE INTO series
        SELECT
        series_id,
        title,
        series_info,
        CAST(release_date AS Uint16) AS release_date
        FROM AS_TABLE(\$seriesData);
        
        REPLACE INTO seasons
        SELECT
        series_id,
        season_id,
        title,
        CAST(first_aired AS Uint16) AS first_aired,
        CAST(last_aired AS Uint16) AS last_aired
        FROM AS_TABLE(\$seasonsData);
        
        REPLACE INTO episodes
        SELECT
        series_id,
        season_id,
        episode_id,
        title,
        CAST(air_date AS Uint16) AS air_date
        FROM AS_TABLE(\$episodesData);
    END;

    $params = CreateParams();

    $tx = $ydb->new("YdbTx");
    $tx->mode = $ydb->YDB_TX_SERIALIZABLE_RW;
    $tx->commit = true;
    return $ydb->YdbQueryResultAsStatus($ydb->YdbExecuteQuerySync($session, $query, FFI::addr($tx), $params));
}

function SelectSimple($session, $data) {
    global $ydb;

    $result_set = $ydb->cast("YdbResultSet*", $data);

    $query = <<<END
        SELECT series_id, title, CAST(release_date AS Date) AS release_date
        FROM series
        WHERE series_id = 1;
    END;

    $tx = $ydb->new("YdbTx");
    $tx->mode = $ydb->YDB_TX_SERIALIZABLE_RW;
    $tx->commit = true;
    $result = $ydb->YdbExecuteQuerySync($session, $query, FFI::addr($tx), $ydb->new("YdbParams"));
    if ($ydb->YdbIsSuccess($ydb->YdbQueryResultAsStatus($result))) {
        $result_set[0] = $ydb->YdbGetResultSet($result, 0);
        return $ydb->YdbQueryResultAsStatus($result);
    }
    return $ydb->YdbQueryResultAsStatus($result);
}

function UnwrapStatus($status) {
    global $ydb;

    if (!$ydb->YdbIsSuccess($status)) {
        $error = $ydb->YdbGetErrorMessage($status);
        $error_string = FFI::string($error);
        echo "fatal error:\n$error_string\n";
        $ydb->free($error);
        return false;
    }
    $ydb->YdbDestroyStatus($status);
    return true;
}

function Run($client) {
    global $ydb; 

    if (!(UnwrapStatus($ydb->YdbRetryQuerySync($client, Closure::fromCallable('CreateSeries'), NULL)) &&
          UnwrapStatus($ydb->YdbRetryQuerySync($client, Closure::fromCallable('CreateSeasons'), NULL)) &&
          UnwrapStatus($ydb->YdbRetryQuerySync($client, Closure::fromCallable('CreateEpisodes'), NULL)))) {
        return false;
    }

    if (!UnwrapStatus($ydb->YdbRetryQuerySync($client, Closure::fromCallable('FillData'), NULL))) {
        return false;
    }

    $result_set = $ydb->new("YdbResultSet");
    if (!UnwrapStatus($ydb->YdbRetryQuerySync($client, Closure::fromCallable('SelectSimple'), FFI::addr($result_set)))) {
        return false;
    }

    $parser = $ydb->YdbCreateResultSetParser($result_set);
    while ($ydb->YdbNextRow($parser)) {
        echo "> SelectSimple:\nSeries";

        echo ", Id: ";
        $id_exists = $ydb->new("uint8_t");
        $id = $ydb->YdbParseUint64($ydb->YdbColumnParser($parser, "series_id"), FFI::addr($id_exists));
        if ($id_exists->cdata) {
            echo $id;
        } else {
            echo "(NULL)";
        }

        echo ", Title: ";
        $title = $ydb->YdbParseUtf8($ydb->YdbColumnParser($parser, "title"));
        if ($title) {
            echo FFI::string($title);
        } else {
            echo "(NULL)";
        }

        echo ", Release date: ";
        $parsed_date = $ydb->new("uint8_t");
        $release_date = $ydb->YdbParseDate($ydb->YdbColumnParser($parser, "release_date"), FFI::addr($parsed_date));
        if ($parsed_date->cdata) {
            echo FFI::string($ydb->YdbFormatLocalTime($release_date, "%Y-%m-%d"));
        } else {
            echo "(NULL)";
        }

        echo "\n";
    }

    if (!(UnwrapStatus($ydb->YdbRetryQuerySync($client, Closure::fromCallable('DropSeries'), NULL)) &&
          UnwrapStatus($ydb->YdbRetryQuerySync($client, Closure::fromCallable('DropSeasons'), NULL)) &&
          UnwrapStatus($ydb->YdbRetryQuerySync($client, Closure::fromCallable('DropEpisodes'), NULL)))) {
        return false;
    }

    return true;
}

$config = $ydb->YdbCreateDriverConfig();
$ydb->YdbDriverConfigSetEndpoint($config, "localhost:2136");
$ydb->YdbDriverConfigSetDatabase($config, "/Root/test");

$driver = $ydb->YdbCreateDriver($config);
$client = $ydb->YdbCreateQueryClient($driver);

$ok = Run($client);

$ydb->YdbDestroyQueryClient($client);
$ydb->YdbStopDriver($driver, true);
$ydb->YdbDestroyDriverConfig($config);
