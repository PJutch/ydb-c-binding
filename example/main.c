#include "data.h"

#include <ydb-c-sdk.h>

#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

YdbStatus CreateSeries(YdbSession session, void *) {
    char *query =
        "CREATE TABLE series ("
        "    series_id Uint64,"
        "    title Utf8,"
        "    series_info Utf8,"
        "    release_date Uint64,"
        "    PRIMARY KEY (series_id)"
        ");";

    return YdbAsStatus(YdbExecuteQuerySync(session, query, NULL, YDB_NULL_PARAMS));
}

YdbStatus CreateSeasons(YdbSession session, void *) {
    char *query =
        "CREATE TABLE seasons ("
        "    series_id Uint64,"
        "    season_id Uint64,"
        "    title Utf8,"
        "    first_aired Uint64,"
        "    last_aired Uint64,"
        "    PRIMARY KEY (series_id, season_id)"
        ");";

    return YdbAsStatus(YdbExecuteQuerySync(session, query, NULL, YDB_NULL_PARAMS));
}

YdbStatus CreateEpisodes(YdbSession session, void *) {
    char *query =
        "CREATE TABLE episodes ("
        "    series_id Uint64,"
        "    season_id Uint64,"
        "    episode_id Uint64,"
        "    title Utf8,"
        "    air_date Uint64,"
        "    PRIMARY KEY (series_id, season_id, episode_id)"
        ");";
    return YdbAsStatus(YdbExecuteQuerySync(session, query, NULL, YDB_NULL_PARAMS));
}

YdbStatus DropSeries(YdbSession session, void *) {
    char *query = "DROP TABLE series";
    return YdbAsStatus(YdbExecuteQuerySync(session, query, NULL, YDB_NULL_PARAMS));
}

YdbStatus DropSeasons(YdbSession session, void *) {
    char *query = "DROP TABLE seasons";
    return YdbAsStatus(YdbExecuteQuerySync(session, query, NULL, YDB_NULL_PARAMS));
}

YdbStatus DropEpisodes(YdbSession session, void *) {
    char *query = "DROP TABLE episodes";
    return YdbAsStatus(YdbExecuteQuerySync(session, query, NULL, YDB_NULL_PARAMS));
}

YdbStatus FillData(YdbSession session, void*) {
    char* query = 
        "DECLARE $seriesData AS List<Struct<\n"
            "series_id: Uint64,\n"
            "title: Utf8,\n"
            "series_info: Utf8,\n"
            "release_date: Date>>;\n"
           "\n"
        "DECLARE $seasonsData AS List<Struct<\n"
            "series_id: Uint64,\n"
            "season_id: Uint64,\n"
            "title: Utf8,\n"
            "first_aired: Date,\n"
            "last_aired: Date>>;\n"
           "\n"
        "DECLARE $episodesData AS List<Struct<\n"
            "series_id: Uint64,\n"
            "season_id: Uint64,\n"
            "episode_id: Uint64,\n"
            "title: Utf8,\n"
            "air_date: Date>>;\n"
           "\n"
        "REPLACE INTO series\n"
        "SELECT\n"
            "series_id,\n"
            "title,\n"
            "series_info,\n"
            "CAST(release_date AS Uint16) AS release_date\n"
        "FROM AS_TABLE($seriesData);\n"
       "\n"
        "REPLACE INTO seasons\n"
        "SELECT\n"
            "series_id,\n"
            "season_id,\n"
            "title,\n"
            "CAST(first_aired AS Uint16) AS first_aired,\n"
            "CAST(last_aired AS Uint16) AS last_aired\n"
        "FROM AS_TABLE($seasonsData);\n"
       "\n"
        "REPLACE INTO episodes\n"
        "SELECT\n"
            "series_id,\n"
            "season_id,\n"
            "episode_id,\n"
            "title,\n"
            "CAST(air_date AS Uint16) AS air_date\n"
        "FROM AS_TABLE($episodesData);\n";

    YdbParams params = CreateParams();

    YdbTx tx = {.mode = YDB_TX_SERIALIZABLE_RW, .commit = true};
    return YdbAsStatus(YdbExecuteQuerySync(session, query, &tx, params));
}

YdbStatus SelectSimple(YdbSession session, void* data) {
    YdbResultSet* result_set = (YdbResultSet*) data;

    char* query = 
        "SELECT series_id, title, CAST(release_date AS Date) AS release_date\n"
        "FROM series\n"
        "WHERE series_id = 1;";

    YdbTx tx = {.mode = YDB_TX_SERIALIZABLE_RW, .commit = true};
    YdbQueryResult result = YdbExecuteQuerySync(session, query, &tx, YDB_NULL_PARAMS);
    if (YdbIsSuccess(YdbAsStatus(result))) {
        *result_set = YdbGetResultSet(result, 0);
        return YdbAsStatus(result);
    }
    return YdbAsStatus(result);
}

bool UnwrapStatus(YdbStatus status) {
    if (!YdbIsSuccess(status)) {
        char *error = YdbGetErrorMessage(status);
        fprintf(stderr, "fatal error: %s\n", error);
        free(error);
        return false;
    }
    YdbDestroyStatus(status);
    return true;
}

bool Run(YdbQueryClient client) {
    if (!(UnwrapStatus(YdbRetryQuerySync(client, &CreateSeries, NULL))
            && UnwrapStatus(YdbRetryQuerySync(client, &CreateSeasons, NULL))
            && UnwrapStatus(YdbRetryQuerySync(client, &CreateEpisodes, NULL)))) {
        return false;
    }

    if (!UnwrapStatus(YdbRetryQuerySync(client, &FillData, NULL))) {
        return false;
    }

    YdbResultSet result_set = {NULL};
    if (!UnwrapStatus(YdbRetryQuerySync(client, &SelectSimple, &result_set))) {
        return false;
    }

    YdbResultSetParser parser = YdbCreateResultSetParser(result_set);
    while (YdbNextRow(parser)) {
        printf("> SelectSimple:\nSeries");

        printf(", Id: ");
        bool id_exists;
        uint64_t id = YdbParseUint64(YdbColumnParser(parser, "series_id"), &id_exists);
        if (id_exists) {
            printf("%lu", id);
        } else {
            printf("(NULL)");
        }

        printf(", Title: ");
        char* title = YdbParseUtf8(YdbColumnParser(parser, "title"));
        if (title) {
            printf("%s", title);
        } else {
            printf("(NULL)");
        }

        printf(", Release date: ");
        YdbInstant release_date = YdbParseDate(YdbColumnParser(parser, "release_date"));
        if (release_date.data) {
            printf("%s", YdbFormatLocalTime(release_date, "%Y-%m-%d"));
        } else {
            printf("(NULL)");
        }

        printf("\n");
    }
    
    if (!(UnwrapStatus(YdbRetryQuerySync(client, &DropSeries, NULL))
            && UnwrapStatus(YdbRetryQuerySync(client, &DropSeasons, NULL))
            && UnwrapStatus(YdbRetryQuerySync(client, &DropEpisodes, NULL)))) {
        return false;
    }

    return true;
}

int main() {
    YdbDriverConfig config = YdbCreateDriverConfig();
    YdbDriverConfigSetEndpoint(config, "localhost:2136");
    YdbDriverConfigSetDatabase(config, "/Root/test");

    YdbDriver driver = YdbCreateDriver(config);
    YdbQueryClient client = YdbCreateQueryClient(driver);

    bool ok = Run(client);

    YdbDestroyQueryClient(client);
    YdbStopDriver(driver, true);
    YdbDestroyDriverConfig(config);

    return ok ? 0 : 2;
}
