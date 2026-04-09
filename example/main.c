#include <ydb-c-sdk.h>

#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

TStatus CreateSeries(TSession session, void *) {
    char *query =
        "CREATE TABLE series ("
        "    series_id Uint64,"
        "    title Utf8,"
        "    series_info Utf8,"
        "    release_date Uint64,"
        "    PRIMARY KEY (series_id)"
        ");";

    return ExecuteQuerySync(session, query);
}

TStatus CreateSeasons(TSession session, void *) {
    char *query =
        "CREATE TABLE seasons ("
        "    series_id Uint64,"
        "    season_id Uint64,"
        "    title Utf8,"
        "    first_aired Uint64,"
        "    last_aired Uint64,"
        "    PRIMARY KEY (series_id, season_id)"
        ");";

    return ExecuteQuerySync(session, query);
}

TStatus CreateEpisodes(TSession session, void *) {
    char *query =
        "CREATE TABLE episodes ("
        "    series_id Uint64,"
        "    season_id Uint64,"
        "    episode_id Uint64,"
        "    title Utf8,"
        "    air_date Uint64,"
        "    PRIMARY KEY (series_id, season_id, episode_id)"
        ");";
    return ExecuteQuerySync(session, query);
}

TStatus DropSeries(TSession session, void *) {
    char *query = "DROP TABLE series";
    return ExecuteQuerySync(session, query);
}

TStatus DropSeasons(TSession session, void *) {
    char *query = "DROP TABLE seasons";
    return ExecuteQuerySync(session, query);
}

TStatus DropEpisodes(TSession session, void *) {
    char *query = "DROP TABLE episodes";
    return ExecuteQuerySync(session, query);
}

bool UnwrapStatus(TStatus status) {
    if (!IsSuccess(status)) {
        char *error = GetErrorMessage(status);
        fprintf(stderr, "fatal error: %s\n", error);
        DestroyErrorMessage(error);
        return false;
    }
    DestroyStatus(status);
    return true;
}

bool Run(TQueryClient *client) {
    if (!(UnwrapStatus(RetryQuerySync(client, &CreateSeries, NULL))
            && UnwrapStatus(RetryQuerySync(client, &CreateSeasons, NULL))
            && UnwrapStatus(RetryQuerySync(client, &CreateEpisodes, NULL)))) {
        return false;
    }
    
    if (!(UnwrapStatus(RetryQuerySync(client, &DropSeries, NULL))
            && UnwrapStatus(RetryQuerySync(client, &DropSeasons, NULL))
            && UnwrapStatus(RetryQuerySync(client, &DropEpisodes, NULL)))) {
        return false;
    }

    return true;
}

int main() {
    TDriverConfig *config = CreateDriverConfig();
    DriverConfigSetEndpoint(config, "localhost:2136");
    DriverConfigSetDatabase(config, "/Root/test");

    TDriver *driver = CreateDriver(config);
    TQueryClient *client = CreateQueryClient(driver);

    bool ok = Run(client);

    DestroyQueryClient(client);
    StopDriver(driver, true);
    DestroyDriverConfig(config);

    return ok ? 0 : 2;
}
