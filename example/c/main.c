#include "data.h"

#include <ydb-c-sdk.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

YdbStatus CreateSeries(YdbSession session, void*) {
    char* query = "CREATE TABLE series ("
                  "    series_id Uint64,"
                  "    title Utf8,"
                  "    series_info Utf8,"
                  "    release_date Uint64,"
                  "    PRIMARY KEY (series_id)"
                  ");";

    YdbTx tx = YdbNoTx();
    YdbStatus status = YdbQueryResultToStatus(YdbGetSyncQueryResult(
        YdbExecuteQuery(session, query, tx, YDB_NULL_PARAMS)));
    YdbDestroyTx(tx);
    return status;
}

YdbStatus CreateSeasons(YdbSession session, void*) {
    char* query = "CREATE TABLE seasons ("
                  "    series_id Uint64,"
                  "    season_id Uint64,"
                  "    title Utf8,"
                  "    first_aired Uint64,"
                  "    last_aired Uint64,"
                  "    PRIMARY KEY (series_id, season_id)"
                  ");";

    YdbTx tx = YdbNoTx();
    YdbStatus status = YdbQueryResultToStatus(YdbGetSyncQueryResult(
        YdbExecuteQuery(session, query, tx, YDB_NULL_PARAMS)));
    YdbDestroyTx(tx);
    return status;
}

YdbStatus CreateEpisodes(YdbSession session, void*) {
    char* query = "CREATE TABLE episodes ("
                  "    series_id Uint64,"
                  "    season_id Uint64,"
                  "    episode_id Uint64,"
                  "    title Utf8,"
                  "    air_date Uint64,"
                  "    PRIMARY KEY (series_id, season_id, episode_id)"
                  ");";
    YdbTx tx = YdbNoTx();
    YdbStatus status = YdbQueryResultToStatus(YdbGetSyncQueryResult(
        YdbExecuteQuery(session, query, tx, YDB_NULL_PARAMS)));
    YdbDestroyTx(tx);
    return status;
}

YdbStatus DropSeries(YdbSession session, void*) {
    char* query = "DROP TABLE series";
    YdbTx tx = YdbNoTx();
    YdbStatus status = YdbQueryResultToStatus(YdbGetSyncQueryResult(
        YdbExecuteQuery(session, query, tx, YDB_NULL_PARAMS)));
    YdbDestroyTx(tx);
    return status;
}

YdbStatus DropSeasons(YdbSession session, void*) {
    char* query = "DROP TABLE seasons";
    YdbTx tx = YdbNoTx();
    YdbStatus status = YdbQueryResultToStatus(YdbGetSyncQueryResult(
        YdbExecuteQuery(session, query, tx, YDB_NULL_PARAMS)));
    YdbDestroyTx(tx);
    return status;
}

YdbStatus DropEpisodes(YdbSession session, void*) {
    char* query = "DROP TABLE episodes";
    YdbTx tx = YdbNoTx();
    YdbStatus status = YdbQueryResultToStatus(YdbGetSyncQueryResult(
        YdbExecuteQuery(session, query, tx, YDB_NULL_PARAMS)));
    YdbDestroyTx(tx);
    return status;
}

YdbStatus FillData(YdbSession session, void*) {
    char* query = "DECLARE $seriesData AS List<Struct<\n"
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

    YdbTx tx = YdbBeginTx(YDB_TX_SERIALIZABLE_RW, true, false);
    YdbStatus status = YdbQueryResultToStatus(
        YdbGetSyncQueryResult(YdbExecuteQuery(session, query, tx, params)));
    YdbDestroyTx(tx);

    YdbDestroyParams(params);

    return status;
}

YdbStatus SelectSimple(YdbSession session, void* data) {
    YdbResultSet* result_set = (YdbResultSet*)data;

    char* query =
        "SELECT series_id, title, CAST(release_date AS Date) AS release_date\n"
        "FROM series\n"
        "WHERE series_id = 1;";

    YdbTx tx = YdbBeginTx(YDB_TX_SERIALIZABLE_RW, true, false);
    YdbQueryResult result = YdbGetSyncQueryResult(
        YdbExecuteQuery(session, query, tx, YDB_NULL_PARAMS));
    YdbDestroyTx(tx);

    YdbStatus status = YdbQueryResultGetStatus(result);
    if (YdbIsSuccess(status)) {
        *result_set = YdbGetResultSet(result, 0);
    }
    YdbDestroyQueryResult(result);
    return status;
}

YdbStatus UpsertSimple(YdbSession session, void* data) {
    char* query = "UPSERT INTO episodes (series_id, season_id, episode_id, "
                  "title) VALUES\n"
                  "(2, 6, 1, \"TBD\");";

    YdbTx tx = YdbBeginTx(YDB_TX_SERIALIZABLE_RW, true, false);
    YdbStatus status = YdbQueryResultToStatus(YdbGetSyncQueryResult(
        YdbExecuteQuery(session, query, tx, YDB_NULL_PARAMS)));
    YdbDestroyTx(tx);
    return status;
}

YdbStatus SelectWithParams(YdbSession session, void* data) {
    YdbResultSet* result_set = (YdbResultSet*)data;

    uint64_t seriesId = 2;
    uint64_t seasonId = 3;
    char* query =
        "DECLARE $seriesId AS Uint64;\n"
        "DECLARE $seasonId AS Uint64;\n"
        "SELECT sa.title AS season_title, sr.title AS series_title\n"
        "FROM seasons AS sa\n"
        "INNER JOIN series AS sr\n"
        "ON sa.series_id = sr.series_id\n"
        "WHERE sa.series_id = $seriesId AND sa.season_id = $seasonId;\n";

    YdbParamsBuilder params_builder = YdbCreateParamsBuilder();
    YdbParamValueBuilder series_id_param =
        YdbAddParam(params_builder, "$seriesId");
    YdbParamUint64(series_id_param, seriesId);
    YdbBuildParamValue(series_id_param);
    YdbParamValueBuilder season_id_param =
        YdbAddParam(params_builder, "$seasonId");
    YdbParamUint64(season_id_param, seasonId);
    YdbBuildParamValue(season_id_param);
    YdbParams params = YdbBuildParams(params_builder);

    YdbTx tx = YdbBeginTx(YDB_TX_SERIALIZABLE_RW, true, false);
    YdbQueryResult result =
        YdbGetSyncQueryResult(YdbExecuteQuery(session, query, tx, params));
    YdbDestroyTx(tx);

    YdbStatus status = YdbQueryResultGetStatus(result);
    if (YdbIsSuccess(status)) {
        *result_set = YdbGetResultSet(result, 0);
    }
    YdbDestroyStatus(status);

    YdbDestroyParams(params);
    return YdbQueryResultToStatus(result);
}

YdbStatus MultiStep(YdbSession session, void* data) {
    YdbResultSet* result_set = (YdbResultSet*)data;

    uint64_t seriesId = 2;
    uint64_t seasonId = 5;
    char* query1 = "DECLARE $seriesId AS Uint64;\n"
                   "DECLARE $seasonId AS Uint64;\n"
                   "SELECT first_aired AS from_date FROM seasons\n"
                   "WHERE series_id = $seriesId AND season_id = $seasonId;\n";

    YdbParamsBuilder params1_builder = YdbCreateParamsBuilder();
    YdbParamValueBuilder series_id_param =
        YdbAddParam(params1_builder, "$seriesId");
    YdbParamUint64(series_id_param, seriesId);
    YdbBuildParamValue(series_id_param);
    YdbParamValueBuilder season_id_param =
        YdbAddParam(params1_builder, "$seasonId");
    YdbParamUint64(season_id_param, seasonId);
    YdbBuildParamValue(season_id_param);
    YdbParams params1 = YdbBuildParams(params1_builder);

    // Execute the first query to retrieve the required values for the client.
    // Transaction control settings do not set the CommitTx flag, allowing the
    // transaction to remain active after query execution.
    YdbTx tx1 = YdbBeginTx(YDB_TX_SERIALIZABLE_RW, false, false);
    YdbQueryResult result1 =
        YdbGetSyncQueryResult(YdbExecuteQuery(session, query1, tx1, params1));
    YdbDestroyTx(tx1);

    YdbStatus status1 = YdbQueryResultGetStatus(result1);
    if (!YdbIsSuccess(status1)) {
        YdbDestroyStatus(status1);
        YdbDestroyParams(params1);
        return YdbQueryResultToStatus(result1);
    }
    YdbDestroyStatus(status1);

    // Get the active transaction id
    YdbTransaction transaction = YdbQueryTransaction(result1);

    // Processing the request result
    YdbResultSet temp_result_set = YdbGetResultSet(result1, 0);
    YdbResultSetParser parser = YdbCreateResultSetParser(temp_result_set);
    YdbNextRow(parser);

    bool date_exists;
    YdbInstant from_date = YdbInstantFromDays(
        YdbParseUint64(YdbColumnParser(parser, "from_date"), &date_exists));
    YdbInstant to_date = from_date + YdbDurationFromDays(15);

    YdbDestroyResultSet(temp_result_set);
    YdbDestroyResultSetParser(parser);
    YdbDestroyQueryResult(result1);

    // Construct next query based on the results of client logic
    char* query2 =
        "DECLARE $seriesId AS Uint64;\n"
        "DECLARE $fromDate AS Uint64;\n"
        "DECLARE $toDate AS Uint64;\n"
        "SELECT season_id, episode_id, title, air_date FROM episodes\n"
        "WHERE series_id = $seriesId AND air_date >= $fromDate AND air_date <= "
        "$toDate;\n";

    YdbParamsBuilder params2_builder = YdbCreateParamsBuilder();
    YdbParamValueBuilder series_id_param2 =
        YdbAddParam(params2_builder, "$seriesId");
    YdbParamUint64(series_id_param2, seriesId);
    YdbBuildParamValue(series_id_param2);
    YdbParamValueBuilder from_date_param =
        YdbAddParam(params2_builder, "$fromDate");
    YdbParamUint64(from_date_param, YdbInstantToDays(from_date));
    YdbBuildParamValue(from_date_param);
    YdbParamValueBuilder to_date_param =
        YdbAddParam(params2_builder, "$toDate");
    YdbParamUint64(to_date_param, YdbInstantToDays(to_date));
    YdbBuildParamValue(to_date_param);
    YdbParams params2 = YdbBuildParams(params2_builder);

    // Execute the second query.
    // The transaction control settings continue the active transaction (tx)
    // and commit it at the end of the second query execution.
    YdbTx tx2 = YdbTransactionTx(transaction, true);
    YdbQueryResult result2 =
        YdbGetSyncQueryResult(YdbExecuteQuery(session, query2, tx2, params2));
    YdbDestroyTx(tx2);

    YdbStatus status = YdbQueryResultGetStatus(result2);
    if (YdbIsSuccess(status)) {
        *result_set = YdbGetResultSet(result2, 0);
    }

    YdbDestroyQueryResult(result2);
    YdbDestroyParams(params1);
    YdbDestroyParams(params2);
    YdbDestroyTransaction(transaction);

    return status;
}

YdbStatus ExplicitTcl(YdbQueryClient client, void* data) {
    YdbInstant air_date = YdbInstantNow();

    YdbCreateSessionResult session_result =
        YdbGetSyncCreateSessionResult(YdbCreateSession(client));

    YdbStatus status = YdbCreateSessionResultGetStatus(session_result);
    if (!YdbIsSuccess(status)) {
        YdbDestroyStatus(status);
        return YdbCreateSessionResultToStatus(session_result);
    }
    YdbDestroyStatus(status);

    YdbSession session = YdbCreateSessionResultGetSession(session_result);
    YdbDestroyCreateSessionResult(session_result);

    YdbBeginTransactionResult transaction_result =
        YdbGetSyncBeginTransactionResult(
            YdbBeginTransaction(session, YDB_TX_SERIALIZABLE_RW, false));

    status = YdbBeginTransactionResultGetStatus(transaction_result);
    if (!YdbIsSuccess(status)) {
        return YdbBeginTransactionResultToStatus(transaction_result);
    }
    YdbDestroyStatus(status);

    YdbTransaction transaction =
        YdbBeginTransactionResultGetTransaction(transaction_result);
    YdbDestroyBeginTransactionResult(transaction_result);

    char* query = "DECLARE $airDate AS Date;\n"
                  "UPDATE episodes SET air_date = CAST($airDate AS Uint16)\n"
                  "WHERE title = \"TBD\";\n";

    YdbParamsBuilder params_builder = YdbCreateParamsBuilder();
    YdbParamValueBuilder air_date_param =
        YdbAddParam(params_builder, "$airDate");
    YdbParamDate(air_date_param, air_date);
    YdbBuildParamValue(air_date_param);
    YdbParams params = YdbBuildParams(params_builder);

    YdbTx tx = YdbTransactionTx(transaction, false);
    YdbQueryResult update_result =
        YdbGetSyncQueryResult(YdbExecuteQuery(session, query, tx, params));
    YdbDestroyTx(tx);

    YdbStatus status1 = YdbQueryResultGetStatus(update_result);
    if (!YdbIsSuccess(status1)) {
        YdbDestroyParams(params);
        YdbDestroyTransaction(transaction);
        YdbDestroySession(session);

        return status1;
    }
    YdbDestroyStatus(status1);
    YdbDestroyQueryResult(update_result);

    YdbStatus status2 =
        YdbCommitResultToStatus(YdbGetSyncCommitResult(YdbCommit(transaction)));

    YdbDestroyParams(params);
    YdbDestroyTransaction(transaction);
    YdbDestroySession(session);

    return status2;
}

YdbStatus StreamQuerySelect(YdbQueryClient client, void* data) {
    char* query = "DECLARE $series AS List<UInt64>;\n"
                  "SELECT series_id, season_id, title, CAST(first_aired AS "
                  "Date) AS first_aired\n"
                  "FROM seasons\n"
                  "WHERE series_id IN $series\n"
                  "ORDER BY season_id;\n";

    YdbParamsBuilder params_builder = YdbCreateParamsBuilder();
    YdbParamValueBuilder list_param = YdbAddParam(params_builder, "$series");
    YdbParamBeginList(list_param);

    YdbParamAddListItem(list_param);
    YdbParamUint64(list_param, 1);

    YdbParamAddListItem(list_param);
    YdbParamUint64(list_param, 10);

    YdbParamEndList(list_param);
    YdbBuildParamValue(list_param);
    YdbParams params = YdbBuildParams(params_builder);

    YdbTx tx = YdbNoTx();
    YdbExecuteQueryIterator resultStreamQuery = YdbGetSyncExecuteQueryIterator(
        YdbStreamExecuteQuery(client, query, tx, params));
    YdbDestroyTx(tx);

    YdbStatus status = YdbExecuteQueryIteratorGetStatus(resultStreamQuery);
    if (!YdbIsSuccess(status)) {
        YdbDestroyStatus(status);
        YdbDestroyParams(params);
        return YdbExecuteQueryIteratorToStatus(resultStreamQuery);
    }
    YdbDestroyStatus(status);

    // Iterates over results
    bool eos = false;

    while (!eos) {
        YdbExecuteQueryPart streamPart =
            YdbGetSyncExecuteQueryPart(YdbReadNext(resultStreamQuery));

        YdbStatus status = YdbExecuteQueryPartGetStatus(streamPart);
        if (!YdbIsSuccess(status)) {
            eos = true;
            if (!YdbIsEos(streamPart)) {
                YdbDestroyExecuteQueryIterator(resultStreamQuery);
                YdbDestroyExecuteQueryPart(streamPart);
                return status;
            }
            YdbDestroyExecuteQueryPart(streamPart);
            YdbDestroyStatus(status);
            continue;
        }
        YdbDestroyStatus(status);

        // It is possible to duplicate lines in the output stream due to an
        // external retryer.
        if (YdbExecuteQueryPartHasResultSet(streamPart)) {
            YdbResultSet rs = YdbExecuteQueryPartGetResultSet(streamPart);
            YdbResultSetParser parser = YdbCreateResultSetParser(rs);
            while (YdbNextRow(parser)) {
                printf("Season");

                printf(", SeriesId: ");
                bool has_series_id;
                uint64_t series_id = YdbParseUint64(
                    YdbColumnParser(parser, "series_id"), &has_series_id);
                if (has_series_id) {
                    printf("%lu", series_id);
                } else {
                    printf("(NULL)");
                }

                printf(", SeasonId: ");
                bool has_season_id;
                uint64_t season_id = YdbParseUint64(
                    YdbColumnParser(parser, "season_id"), &has_season_id);
                if (has_season_id) {
                    printf("%lu", season_id);
                } else {
                    printf("(NULL)");
                }

                printf(", Title: ");
                char* title = YdbParseUtf8(YdbColumnParser(parser, "title"));
                if (title != NULL) {
                    printf("%s", title);
                } else {
                    printf("(NULL)");
                }
                free(title);

                printf(", Air date: ");
                bool has_air_date;
                uint64_t air_date = YdbParseDate(
                    YdbColumnParser(parser, "first_aired"), &has_air_date);
                if (has_air_date) {
                    char* formatted_date =
                        YdbFormatLocalTime(air_date, "%Y-%m-%d");
                    printf("%s", formatted_date);
                    free(formatted_date);
                } else {
                    printf("(NULL)");
                }

                printf("\n");
            }

            YdbDestroyResultSet(rs);
            YdbDestroyResultSetParser(parser);
        }

        YdbDestroyExecuteQueryPart(streamPart);
    }

    YdbDestroyExecuteQueryIterator(resultStreamQuery);
    YdbDestroyParams(params);
    return YdbStatusOk();
}

bool UnwrapStatus(YdbStatus status) {
    if (!YdbIsSuccess(status)) {
        char* error = YdbGetErrorMessage(status);
        fprintf(stderr, "fatal error:\n%s\n", error);
        free(error);
        YdbDestroyStatus(status);
        return false;
    }
    YdbDestroyStatus(status);
    return true;
}

typedef YdbStatus (*SyncRetryable)(YdbSession session, void* data);

YdbStatus RetryQuerySync(YdbQueryClient client, SyncRetryable retriable,
                         void* data) {
    YdbRetrier retrier = YdbCreateRetrier();

    YdbSession session = {};
    while (true) {
        if (session.data == NULL) {
            YdbCreateSessionResult session_result =
                YdbGetSyncCreateSessionResult(YdbCreateSession(client));

            YdbStatus session_status =
                YdbCreateSessionResultGetStatus(session_result);
            if (!YdbIsSuccess(session_status)) {
                YdbDestroyCreateSessionResult(session_result);
                return session_status;
            }
            YdbDestroyStatus(session_status);

            session = YdbCreateSessionResultGetSession(session_result);
            YdbDestroyCreateSessionResult(session_result);
        }

        YdbStatus status = retriable(session, data);

        YdbRetryNextStep next_step = YdbRetrierNext(retrier, status);
        switch (next_step) {
        case YDB_RETRY_RETRY:
            break;
        case YDB_RETRY_RESET:
            YdbDestroySession(session);
            session.data = NULL;
            break;
        case YDB_RETRY_FINISH:
            return status;
        }
    }
}

typedef YdbStatus (*SyncNoSessionRetryable)(YdbQueryClient client, void* data);

YdbStatus RetryQuerySyncNoSession(YdbQueryClient client,
                                  SyncNoSessionRetryable retriable,
                                  void* data) {
    YdbRetrier retrier = YdbCreateRetrier();

    YdbCreateSessionResult session_result =
        YdbGetSyncCreateSessionResult(YdbCreateSession(client));

    YdbStatus status = YdbCreateSessionResultGetStatus(session_result);
    if (!YdbIsSuccess(status)) {
        YdbDestroyStatus(status);
        YdbDestroyCreateSessionResult(session_result);
        return status;
    }
    YdbDestroyStatus(status);

    YdbSession session = YdbCreateSessionResultGetSession(session_result);
    YdbDestroyCreateSessionResult(session_result);

    while (true) {
        status = retriable(client, data);
        
        YdbRetryNextStep next_step = YdbRetrierNext(retrier, status);
        switch (next_step) {
        case YDB_RETRY_RETRY:
            break;
        case YDB_RETRY_RESET:
            YdbDestroySession(session);
            session.data = NULL;
            break;
        case YDB_RETRY_FINISH:
            return status;
        }
    }
    return status;
}

bool Run(YdbQueryClient client) {
    if (!(UnwrapStatus(RetryQuerySync(client, &CreateSeries, NULL)) &&
          UnwrapStatus(RetryQuerySync(client, &CreateSeasons, NULL)) &&
          UnwrapStatus(RetryQuerySync(client, &CreateEpisodes, NULL)))) {
        return false;
    }

    if (!UnwrapStatus(RetryQuerySync(client, &FillData, NULL))) {
        return false;
    }

    YdbResultSet result_set = {NULL};
    if (!UnwrapStatus(RetryQuerySync(client, &SelectSimple, &result_set))) {
        return false;
    }

    YdbResultSetParser parser = YdbCreateResultSetParser(result_set);
    while (YdbNextRow(parser)) {
        printf("> SelectSimple:\nSeries");

        printf(", Id: ");
        bool id_exists;
        uint64_t id =
            YdbParseUint64(YdbColumnParser(parser, "series_id"), &id_exists);
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
        free(title);

        printf(", Release date: ");
        bool parsed_date;
        YdbInstant release_date =
            YdbParseDate(YdbColumnParser(parser, "release_date"), &parsed_date);
        if (parsed_date) {
            char* formatted_date = YdbFormatLocalTime(release_date, "%Y-%m-%d");
            printf("%s", formatted_date);
            free(formatted_date);
        } else {
            printf("(NULL)");
        }

        printf("\n");
    }
    YdbDestroyResultSet(result_set);
    YdbDestroyResultSetParser(parser);

    if (!UnwrapStatus(RetryQuerySync(client, &UpsertSimple, NULL))) {
        return false;
    }

    if (!UnwrapStatus(RetryQuerySync(client, &SelectWithParams, &result_set))) {
        return false;
    }

    parser = YdbCreateResultSetParser(result_set);
    if (YdbNextRow(parser)) {
        printf("> SelectWithParams:\nSeason");

        printf(", Title: ");
        char* title = YdbParseUtf8(YdbColumnParser(parser, "season_title"));
        if (title) {
            printf("%s", title);
        } else {
            printf("(NULL)");
        }
        free(title);

        printf(", Series title: ");
        char* series_title =
            YdbParseUtf8(YdbColumnParser(parser, "series_title"));
        if (series_title) {
            printf("%s", series_title);
        } else {
            printf("(NULL)");
        }
        free(series_title);

        printf("\n");
    }
    YdbDestroyResultSet(result_set);
    YdbDestroyResultSetParser(parser);

    if (!UnwrapStatus(RetryQuerySync(client, &MultiStep, &result_set))) {
        return false;
    }

    parser = YdbCreateResultSetParser(result_set);
    printf("> MultiStep:\n");
    while (YdbNextRow(parser)) {
        printf("Episode: ");
        bool episode_id_exits;
        uint64_t episode_id = YdbParseUint64(
            YdbColumnParser(parser, "episode_id"), &episode_id_exits);
        printf("%lu", episode_id);

        printf(", Season: ");
        bool season_id_exits;
        uint64_t season_id = YdbParseUint64(
            YdbColumnParser(parser, "season_id"), &season_id_exits);
        printf("%lu", season_id);

        printf(", Title: ");
        char* title = YdbParseUtf8(YdbColumnParser(parser, "title"));
        if (title) {
            printf("%s", title);
        } else {
            printf("(NULL)");
        }
        free(title);

        bool air_date_exists;
        YdbInstant air_date = YdbInstantFromDays(YdbParseUint64(
            YdbColumnParser(parser, "air_date"), &air_date_exists));

        char* formatted_date = YdbFormatLocalTime(air_date, "%a %b %d, %Y");
        printf(", Air date: %s\n", formatted_date);
        free(formatted_date);
    }
    YdbDestroyResultSet(result_set);
    YdbDestroyResultSetParser(parser);

    if (!UnwrapStatus(RetryQuerySyncNoSession(client, &ExplicitTcl, NULL))) {
        return false;
    }

    printf("> StreamQuery:\n");
    if (!UnwrapStatus(
            RetryQuerySyncNoSession(client, &StreamQuerySelect, NULL))) {
        return false;
    }

    if (!(UnwrapStatus(RetryQuerySync(client, &DropSeries, NULL)) &&
          UnwrapStatus(RetryQuerySync(client, &DropSeasons, NULL)) &&
          UnwrapStatus(RetryQuerySync(client, &DropEpisodes, NULL)))) {
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
