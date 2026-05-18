<?php

try {
    $ydb = FFI::load("build/ydb-c-sdk.i");
} catch (FFI\Exception $e) {
    echo $e;
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

    $tx = $ydb->YdbNoTx();
    $status = $ydb->YdbQueryResultToStatus($ydb->YdbGetSyncQueryResult(
        $ydb->YdbExecuteQuery($session, $query, $tx, $ydb->new('YdbParams'))));
    $ydb->YdbDestroyTx($tx);
    return $status;
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

    $tx = $ydb->YdbNoTx();
    $status = $ydb->YdbQueryResultToStatus($ydb->YdbGetSyncQueryResult(
        $ydb->YdbExecuteQuery($session, $query, $tx, $ydb->new('YdbParams'))));
    $ydb->YdbDestroyTx($tx);
    return $status;
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

    $tx = $ydb->YdbNoTx();
    $status = $ydb->YdbQueryResultToStatus($ydb->YdbGetSyncQueryResult(
        $ydb->YdbExecuteQuery($session, $query, $tx, $ydb->new('YdbParams'))));
    $ydb->YdbDestroyTx($tx);
    return $status;
}

function DropSeries($session, $data) {
    global $ydb;

    $query = "DROP TABLE series";

    $tx = $ydb->YdbNoTx();
    $status = $ydb->YdbQueryResultToStatus($ydb->YdbGetSyncQueryResult(
        $ydb->YdbExecuteQuery($session, $query, $tx, $ydb->new('YdbParams'))));
    $ydb->YdbDestroyTx($tx);
    return $status;
}

function DropSeasons($session, $data) {
    global $ydb;

    $query = "DROP TABLE seasons";
    
    $tx = $ydb->YdbNoTx();
    $status = $ydb->YdbQueryResultToStatus($ydb->YdbGetSyncQueryResult(
        $ydb->YdbExecuteQuery($session, $query, $tx, $ydb->new('YdbParams'))));
    $ydb->YdbDestroyTx($tx);
    return $status;
}

function DropEpisodes($session, $data) {
    global $ydb;

    $query = "DROP TABLE episodes";
    
    $tx = $ydb->YdbNoTx();
    $status = $ydb->YdbQueryResultToStatus($ydb->YdbGetSyncQueryResult(
        $ydb->YdbExecuteQuery($session, $query, $tx, $ydb->new('YdbParams'))));
    $ydb->YdbDestroyTx($tx);
    return $status;
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

    $tx = $ydb->YdbBeginTx($ydb->YDB_TX_SERIALIZABLE_RW, true, false);
    $status = $ydb->YdbQueryResultToStatus($ydb->YdbGetSyncQueryResult(
        $ydb->YdbExecuteQuery($session, $query, $tx, $params)));
    $ydb->YdbDestroyTx($tx);

    $ydb->YdbDestroyParams($params);

    return $status;
}

function SelectSimple($session, $data) {
    global $ydb;

    $result_set = $ydb->cast("YdbResultSet*", $data);

    $query = <<<END
        SELECT series_id, title, CAST(release_date AS Date) AS release_date
        FROM series
        WHERE series_id = 1;
    END;

    $tx = $ydb->YdbBeginTx($ydb->YDB_TX_SERIALIZABLE_RW, true, false);
    $result = $ydb->YdbGetSyncQueryResult(
        $ydb->YdbExecuteQuery($session, $query, $tx, $ydb->new("YdbParams")));
    $ydb->YdbDestroyTx($tx);

    $status = $ydb->YdbQueryResultGetStatus($result);
    if ($ydb->YdbIsSuccess($status)) {
        $result_set[0] = $ydb->YdbGetResultSet($result, 0);
        return $status;
    }
    return $status;
}

function UpsertSimple($session, $data) {
    global $ydb;

    $query = <<<END
        UPSERT INTO episodes (series_id, season_id, episode_id, title) VALUES (2, 6, 1, "TBD");
    END;

    $tx = $ydb->YdbBeginTx($ydb->YDB_TX_SERIALIZABLE_RW, true, false);
    return $ydb->YdbQueryResultToStatus($ydb->YdbGetSyncQueryResult(
        $ydb->YdbExecuteQuery($session, $query, $tx, $ydb->new('YdbParams'))));
}

function SelectWithParams($session, $data) {
    global $ydb;

    $result_set = $ydb->cast("YdbResultSet*", $data);

    $seriesId = 2;
    $seasonId = 3;
    $query = <<<END
        DECLARE \$seriesId AS Uint64;
        DECLARE \$seasonId AS Uint64;
        SELECT sa.title AS season_title, sr.title AS series_title
        FROM seasons AS sa
        INNER JOIN series AS sr
        ON sa.series_id = sr.series_id
        WHERE sa.series_id = \$seriesId AND sa.season_id = \$seasonId;
    END;

    $params_builder = $ydb->YdbCreateParamsBuilder();
    $series_id_param =
        $ydb->YdbAddParam($params_builder, "\$seriesId");
    $ydb->YdbParamUint64($series_id_param, $seriesId);
    $ydb->YdbBuildParamValue($series_id_param);
    $season_id_param =
        $ydb->YdbAddParam($params_builder, "\$seasonId");
    $ydb->YdbParamUint64($season_id_param, $seasonId);
    $ydb->YdbBuildParamValue($season_id_param);
    $params = $ydb->YdbBuildParams($params_builder);

    $tx = $ydb->YdbBeginTx($ydb->YDB_TX_SERIALIZABLE_RW, true, false);
    $result = $ydb->YdbGetSyncQueryResult(
        $ydb->YdbExecuteQuery($session, $query, $tx, $params));
    $ydb->YdbDestroyTx($tx);

    $status = $ydb->YdbQueryResultGetStatus($result);
    if ($ydb->YdbIsSuccess($status)) {
        $result_set[0] = $ydb->YdbGetResultSet($result, 0);
    }

    return $status;
}

function MultiStep($session, $data) {
    global $ydb;

    $result_set = $ydb->cast("YdbResultSet*", $data);

    $seriesId = 2;
    $seasonId = 5;
    $query1 = <<<END
        DECLARE \$seriesId AS Uint64;
        DECLARE \$seasonId AS Uint64;
        SELECT first_aired AS from_date FROM seasons
        WHERE series_id = \$seriesId AND season_id = \$seasonId;
    END;

    $params1_builder = $ydb->YdbCreateParamsBuilder();
    $series_id_param =
        $ydb->YdbAddParam($params1_builder, "\$seriesId");
    $ydb->YdbParamUint64($series_id_param, $seriesId);
    $ydb->YdbBuildParamValue($series_id_param);
    $season_id_param =
        $ydb->YdbAddParam($params1_builder, "\$seasonId");
    $ydb->YdbParamUint64($season_id_param, $seasonId);
    $ydb->YdbBuildParamValue($season_id_param);
    $params1 = $ydb->YdbBuildParams($params1_builder);

    // Execute the first query to retrieve the required values for the client.
    // Transaction control settings do not set the CommitTx flag, allowing the
    // transaction to remain active after query execution.
    $tx1 = $ydb->YdbBeginTx($ydb->YDB_TX_SERIALIZABLE_RW, false, false);
    $result1 =
        $ydb->YdbGetSyncQueryResult($ydb->YdbExecuteQuery($session, $query1, $tx1, $params1));
    $ydb->YdbDestroyTx($tx1);

    $status1 = $ydb->YdbQueryResultGetStatus($result1);
    if (!$ydb->YdbIsSuccess($status1)) {
        $ydb->YdbDestroyStatus($status1);
        $ydb->YdbDestroyParams($params1);
        return $ydb->YdbQueryResultToStatus($result1);
    }
    $ydb->YdbDestroyStatus($status1);

    // Get the active transaction id
    $transaction = $ydb->YdbQueryTransaction($result1);

    // Processing the request result
    $temp_result_set = $ydb->YdbGetResultSet($result1, 0);
    $parser = $ydb->YdbCreateResultSetParser($temp_result_set);
    $ydb->YdbNextRow($parser);

    $date_exists = $ydb->new("uint8_t");
    $from_date = $ydb->YdbInstantFromDays(
        $ydb->YdbParseUint64($ydb->YdbColumnParser($parser, "from_date"), FFI::addr($date_exists)));
    $to_date = $from_date + $ydb->YdbDurationFromDays(15);

    $ydb->YdbDestroyResultSet($temp_result_set);
    $ydb->YdbDestroyResultSetParser($parser);
    $ydb->YdbDestroyQueryResult($result1);

    // Construct next query based on the results of client logic
    $query2 = <<<END
        DECLARE \$seriesId AS Uint64;
        DECLARE \$fromDate AS Uint64;
        DECLARE \$toDate AS Uint64;
        SELECT season_id, episode_id, title, air_date FROM episodes
        WHERE series_id = \$seriesId AND air_date >= \$fromDate AND air_date <
        \$toDate;
    END;

    $params2_builder = $ydb->YdbCreateParamsBuilder();
    $series_id_param2 =
        $ydb->YdbAddParam($params2_builder, "\$seriesId");
    $ydb->YdbParamUint64($series_id_param2, $seriesId);
    $ydb->YdbBuildParamValue($series_id_param2);
    $from_date_param =
        $ydb->YdbAddParam($params2_builder, "\$fromDate");
    $ydb->YdbParamUint64($from_date_param, $ydb->YdbInstantToDays($from_date));
    $ydb->YdbBuildParamValue($from_date_param);
    $to_date_param =
        $ydb->YdbAddParam($params2_builder, "\$toDate");
    $ydb->YdbParamUint64($to_date_param, $ydb->YdbInstantToDays($to_date));
    $ydb->YdbBuildParamValue($to_date_param);
    $params2 = $ydb->YdbBuildParams($params2_builder);

    // Execute the second query.
    // The transaction control settings continue the active transaction (tx)
    // and commit it at the end of the second query execution.
    $tx2 = $ydb->YdbTransactionTx($transaction, true);

    $result2 =
        $ydb->YdbGetSyncQueryResult($ydb->YdbExecuteQuery($session, $query2, $tx2, $params2));

    $ydb->YdbDestroyTx($tx2);

    $status = $ydb->YdbQueryResultGetStatus($result2);
    if ($ydb->YdbIsSuccess($status)) {
        $result_set[0] = $ydb->YdbGetResultSet($result2, 0);
    }

    $ydb->YdbDestroyQueryResult($result2);
    $ydb->YdbDestroyParams($params1);
    $ydb->YdbDestroyParams($params2);
    $ydb->YdbDestroyTransaction($transaction);

    return $status;
}

function ExplicitTcl($client, $data) {
    global $ydb;

    $air_date = $ydb->YdbInstantNow();

    $session_result =
        $ydb->YdbGetSyncCreateSessionResult($ydb->YdbCreateSession($client));

    $status = $ydb->YdbCreateSessionResultGetStatus($session_result);
    if (!$ydb->YdbIsSuccess($status)) {
        $ydb->YdbDestroyStatus($status);
        return $ydb->YdbCreateSessionResultToStatus($session_result);
    }
    $ydb->YdbDestroyStatus($status);

    $session = $ydb->YdbCreateSessionResultGetSession($session_result);
    $ydb->YdbDestroyCreateSessionResult($session_result);

    $transaction_result =
        $ydb->YdbGetSyncBeginTransactionResult(
            $ydb->YdbBeginTransaction($session, $ydb->YDB_TX_SERIALIZABLE_RW, false));

    $status = $ydb->YdbBeginTransactionResultGetStatus($transaction_result);
    if (!$ydb->YdbIsSuccess($status)) {
        return $ydb->YdbBeginTransactionResultToStatus($transaction_result);
    }
    $ydb->YdbDestroyStatus($status);

    $transaction =
        $ydb->YdbBeginTransactionResultGetTransaction($transaction_result);
    $ydb->YdbDestroyBeginTransactionResult($transaction_result);

    $query = <<<END
        DECLARE \$airDate AS Date;
        UPDATE episodes SET air_date = CAST(\$airDate AS Uint16)
        WHERE title = "TBD";
    END;

    $params_builder = $ydb->YdbCreateParamsBuilder();
    $air_date_param =
        $ydb->YdbAddParam($params_builder, "\$airDate");
    $ydb->YdbParamDate($air_date_param, $air_date);
    $ydb->YdbBuildParamValue($air_date_param);
    $params = $ydb->YdbBuildParams($params_builder);

    $tx = $ydb->YdbTransactionTx($transaction, true);
    $update_result =
        $ydb->YdbGetSyncQueryResult($ydb->YdbExecuteQuery($session, $query, $tx, $params));
    $ydb->YdbDestroyTx($tx);

    $status1 = $ydb->YdbQueryResultGetStatus($update_result);
    if (!$ydb->YdbIsSuccess($status1)) {
        $ydb->YdbDestroyParams($params);
        $ydb->YdbDestroyTransaction($transaction);
        $ydb->YdbDestroySession($session);

        return $status1;
    }
    $ydb->YdbDestroyStatus($status1);
    $ydb->YdbDestroyQueryResult($update_result);

    $status2 =
        $ydb->YdbCommitResultToStatus($ydb->YdbGetSyncCommitResult($ydb->YdbCommit($transaction)));

    $ydb->YdbDestroyParams($params);
    $ydb->YdbDestroyTransaction($transaction);
    $ydb->YdbDestroySession($session);

    return $status2;
}

function StreamQuerySelect($client, $data) {
    global $ydb;

    $query = <<<END
        DECLARE \$series AS List<UInt64>;
        SELECT series_id, season_id, title, CAST(first_aired AS Date) AS first_aired
        FROM seasons
        WHERE series_id IN \$series
        ORDER BY season_id;
    END;

    $params_builder = $ydb->YdbCreateParamsBuilder();
    $list_param = $ydb->YdbAddParam($params_builder, "\$series");
    $ydb->YdbParamBeginList($list_param);

    $ydb->YdbParamAddListItem($list_param);
    $ydb->YdbParamUint64($list_param, 1);

    $ydb->YdbParamAddListItem($list_param);
    $ydb->YdbParamUint64($list_param, 10);

    $ydb->YdbParamEndList($list_param);
    $ydb->YdbBuildParamValue($list_param);
    $params = $ydb->YdbBuildParams($params_builder);

    $tx = $ydb->YdbNoTx();
    $resultStreamQuery = $ydb->YdbGetSyncExecuteQueryIterator(
        $ydb->YdbStreamExecuteQuery($client, $query, $tx, $params));
    $ydb->YdbDestroyTx($tx);

    $status = $ydb->YdbExecuteQueryIteratorGetStatus($resultStreamQuery);
    if (!$ydb->YdbIsSuccess($status)) {
        $ydb->YdbDestroyStatus($status);
        $ydb->YdbDestroyParams($params);
        return $ydb->YdbExecuteQueryIteratorToStatus($resultStreamQuery);
    }
    $ydb->YdbDestroyStatus($status);

    // Iterates over results
    $eos = false;

    while (!$eos) {
        $streamPart =
            $ydb->YdbGetSyncExecuteQueryPart($ydb->YdbReadNext($resultStreamQuery));

        $status = $ydb->YdbExecuteQueryPartGetStatus($streamPart);
        if (!$ydb->YdbIsSuccess($status)) {
            $eos = true;
            if (!$ydb->YdbIsEos($streamPart)) {
                $ydb->YdbDestroyExecuteQueryIterator($resultStreamQuery);
                $ydb->YdbDestroyExecuteQueryPart($streamPart);
                return $status;
            }
            $ydb->YdbDestroyExecuteQueryPart($streamPart);
            $ydb->YdbDestroyStatus($status);
            continue;
        }
        $ydb->YdbDestroyStatus($status);

        // It is possible to duplicate lines in the output stream due to an
        // external retryer.
        if ($ydb->YdbExecuteQueryPartHasResultSet($streamPart)) {
            $rs = $ydb->YdbExecuteQueryPartGetResultSet($streamPart);
            $parser = $ydb->YdbCreateResultSetParser($rs);
            while ($ydb->YdbNextRow($parser)) {
                echo "Season";

                echo ", SeriesId: ";
                $has_series_id = FFI::new("uint8_t");
                $series_id = $ydb->YdbParseUint64(
                    $ydb->YdbColumnParser($parser, "series_id"), FFI::addr($has_series_id));
                if ($has_series_id) {
                    echo $series_id;
                } else {
                    echo "(NULL)";
                }

                echo ", SeasonId: ";
                $has_season_id = FFI::new("uint8_t");
                $season_id = $ydb->YdbParseUint64(
                    $ydb->YdbColumnParser($parser, "season_id"), FFI::addr($has_season_id));
                if ($has_season_id) {
                    echo $season_id;
                } else {
                    echo "(NULL)";
                }

                echo ", Title: ";
                $title = $ydb->YdbParseUtf8($ydb->YdbColumnParser($parser, "title"));
                if ($title != NULL) {
                    echo FFI::string($title);
                } else {
                    echo "(NULL)";
                }
                $ydb->free($title);

                echo ", Air date: ";
                $has_air_date = FFI::new("uint8_t");
                $air_date = $ydb->YdbParseDate(
                    $ydb->YdbColumnParser($parser, "first_aired"), FFI::addr($has_air_date));
                if ($has_air_date) {
                    $formatted_date =
                        $ydb->YdbFormatLocalTime($air_date, "%Y-%m-%d");
                    echo FFI::string($formatted_date);
                    $ydb->free($formatted_date);
                } else {
                    echo "(NULL)";
                }

                echo "\n";
            }

            $ydb->YdbDestroyResultSet($rs);
            $ydb->YdbDestroyResultSetParser($parser);
        }

        $ydb->YdbDestroyExecuteQueryPart($streamPart);
    }

    $ydb->YdbDestroyExecuteQueryIterator($resultStreamQuery);
    $ydb->YdbDestroyParams($params);
    return $ydb->YdbStatusOk();
}

function UnwrapStatus($status) {
    global $ydb;

    if (!$ydb->YdbIsSuccess($status)) {
        $error = $ydb->YdbGetErrorMessage($status);
        $error_string = FFI::string($error);
        echo "fatal error:\n$error_string\n";
        $ydb->free($error);
        $ydb->YdbDestroyStatus($status);
        return false;
    }
    $ydb->YdbDestroyStatus($status);
    return true;
}

function RetryQuerySync($client, $retriable,
                         $data) {
    global $ydb;

    $retrier = $ydb->YdbCreateRetrier();

    $session = $ydb->new("YdbSession");
    while (true) {
        if ($session->data == NULL) {
            $session_result =
                $ydb->YdbGetSyncCreateSessionResult($ydb->YdbCreateSession($client));

            $session_status =
                $ydb->YdbCreateSessionResultGetStatus($session_result);
            if (!$ydb->YdbIsSuccess($session_status)) {
                $ydb->YdbDestroyCreateSessionResult($session_result);
                return $session_status;
            }
            $ydb->YdbDestroyStatus($session_status);

            $session = $ydb->YdbCreateSessionResultGetSession($session_result);
            $ydb->YdbDestroyCreateSessionResult($session_result);
        }

        $status = $retriable($session, $data);

        $next_step = $ydb->YdbRetrierNext($retrier, $status);
        switch ($next_step) {
        case $ydb->YDB_RETRY_RETRY:
            break;
        case $ydb->YDB_RETRY_RESET:
            $ydb->YdbDestroySession($session);
            $session->data = NULL;
            break;
        case $ydb->YDB_RETRY_FINISH:
            return $status;
        }
    }
}

function RetryQuerySyncNoSession($client, $retriable, $data) {
    global $ydb;

    $retrier = $ydb->YdbCreateRetrier();

    $session_result =
        $ydb->YdbGetSyncCreateSessionResult($ydb->YdbCreateSession($client));

    $status = $ydb->YdbCreateSessionResultGetStatus($session_result);
    if (!$ydb->YdbIsSuccess($status)) {
        $ydb->YdbDestroyStatus($status);
        $ydb->YdbDestroyCreateSessionResult($session_result);
        return $status;
    }
    $ydb->YdbDestroyStatus($status);

    $session = $ydb->YdbCreateSessionResultGetSession($session_result);
    $ydb->YdbDestroyCreateSessionResult($session_result);

    while (true) {
        $status = $retriable($client, $data);
        
        $next_step = $ydb->YdbRetrierNext($retrier, $status);
        switch ($next_step) {
        case $ydb->YDB_RETRY_RETRY:
            break;
        case $ydb->YDB_RETRY_RESET:
            $ydb->YdbDestroySession($session);
            $session->data = NULL;
            break;
        case $ydb->YDB_RETRY_FINISH:
            return $status;
        }
    }
    return $status;
}

function Run($client) {
    global $ydb; 

    if (!(UnwrapStatus(RetryQuerySync($client, Closure::fromCallable('CreateSeries'), NULL)) &&
          UnwrapStatus(RetryQuerySync($client, Closure::fromCallable('CreateSeasons'), NULL)) &&
          UnwrapStatus(RetryQuerySync($client, Closure::fromCallable('CreateEpisodes'), NULL)))) {
        return false;
    }

    if (!UnwrapStatus(RetryQuerySync($client, Closure::fromCallable('FillData'), NULL))) {
        return false;
    }

    $result_set = $ydb->new("YdbResultSet");
    if (!UnwrapStatus(RetryQuerySync($client, Closure::fromCallable('SelectSimple'), FFI::addr($result_set)))) {
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
        $ydb->free($title);

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
    $ydb->YdbDestroyResultSet($result_set);
    $ydb->YdbDestroyResultSetParser($parser);

    if (!UnwrapStatus(RetryQuerySync($client, Closure::fromCallable('UpsertSimple'), NULL))) {
        return false;
    }

    if (!UnwrapStatus(RetryQuerySync($client, Closure::fromCallable('SelectWithParams'), FFI::addr($result_set)))) {
        return false;
    }

    $parser = $ydb->YdbCreateResultSetParser($result_set);
    while ($ydb->YdbNextRow($parser)) {
        echo "> SelectWithParams:\nSeason";

        echo ", Title: ";
        $title = $ydb->YdbParseUtf8($ydb->YdbColumnParser($parser, "season_title"));
        if ($title) {
            echo FFI::string($title);
        } else {
            echo "(NULL)";
        }
        $ydb->free($title);

        echo ", Series title: ";
        $title = $ydb->YdbParseUtf8($ydb->YdbColumnParser($parser, "series_title"));
        if ($title) {
            echo FFI::string($title);
        } else {
            echo "(NULL)";
        }
        $ydb->free($title);

        echo "\n";
    }
    $ydb->YdbDestroyResultSet($result_set);
    $ydb->YdbDestroyResultSetParser($parser);

    if (!UnwrapStatus(RetryQuerySync($client, Closure::fromCallable('MultiStep'), FFI::addr($result_set)))) {
        return false;
    }

    $parser = $ydb->YdbCreateResultSetParser($result_set);
    echo "> MultiStep:\n";
    while ($ydb->YdbNextRow($parser)) {
        echo "Episode: ";
        $episode_id_exits = $ydb->new("uint8_t");
        $episode_id = $ydb->YdbParseUint64(
            $ydb->YdbColumnParser($parser, "episode_id"), FFI::addr($episode_id_exits));
        echo $episode_id;

        echo ", Season: ";
        $season_id_exits = $ydb->new("uint8_t");
        $season_id = $ydb->YdbParseUint64(
            $ydb->YdbColumnParser($parser, "season_id"), FFI::addr($season_id_exits));
        echo $season_id;

        echo ", Title: ";
        $title = $ydb->YdbParseUtf8($ydb->YdbColumnParser($parser, "title"));
        if ($title) {
            echo FFI::string($title);
        } else {
            echo "(NULL)";
        }
        $ydb->free($title);

        $air_date_exists = $ydb->new("uint8_t");
        $air_date = $ydb->YdbInstantFromDays($ydb->YdbParseUint64(
            $ydb->YdbColumnParser($parser, "air_date"), FFI::addr($air_date_exists)));

        $formatted_date = $ydb->YdbFormatLocalTime($air_date, "%a %b %d, %Y");
        echo ", Air date:,", FFI::string($formatted_date), "\n";
        $ydb->free($formatted_date);
    }
    $ydb->YdbDestroyResultSet($result_set);
    $ydb->YdbDestroyResultSetParser($parser);

    if (!UnwrapStatus(RetryQuerySyncNoSession($client, Closure::fromCallable('ExplicitTcl'), NULL))) {
        return false;
    }

    echo "> StreamQuery:\n";
    if (!UnwrapStatus(
            RetryQuerySyncNoSession($client, Closure::fromCallable('StreamQuerySelect'), NULL))) {
        return false;
    }

    if (!(UnwrapStatus(RetryQuerySync($client, Closure::fromCallable('DropSeries'), NULL)) &&
          UnwrapStatus(RetryQuerySync($client, Closure::fromCallable('DropSeasons'), NULL)) &&
          UnwrapStatus(RetryQuerySync($client, Closure::fromCallable('DropEpisodes'), NULL)))) {
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
