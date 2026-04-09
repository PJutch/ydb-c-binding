#include "driver.hpp"
#include "ydb-c-sdk/query.h"

#include <ydb-cpp-sdk/client/query/query.h>
#include <ydb-cpp-sdk/client/query/client.h>
#include <__ydb_sdk_special_headers/util/stream/str.h>
#include <__ydb_sdk_special_headers/util/generic/string.h>

extern "C" {

struct TQueryClient {
    NYdb::NQuery::TQueryClient client;
};

TQueryClient* CreateQueryClient(TDriver* driver) {
    return new TQueryClient{{driver->driver}};
}

void DestroyQueryClient(TQueryClient* client) {
    delete client;
}

void DestroyStatus(TStatus status) {
    delete static_cast<NYdb::TStatus*>(status);
}

bool IsSuccess(TStatus status) {
    return static_cast<NYdb::TStatus*>(status)->IsSuccess();
}

bool IsTransportError(TStatus status) {
    return static_cast<NYdb::TStatus*>(status)->IsTransportError();
}

char* GetErrorMessage(TStatus status) {
    TString string;
    TStringOutput stream{string};

    static_cast<NYdb::TStatus*>(status)->Out(stream);

    return strdup(string.data());
}

void DestroyErrorMessage(char* message) {
    free(message);
}

void FreeResult(TQueryResult* result) {
    free(result);
}

void* AsStatus(TQueryResult result) {
    auto* as_result = static_cast<NYdb::NQuery::TExecuteQueryResult*>(result);
    auto* as_status = static_cast<NYdb::TStatus*>(as_result);
    return static_cast<void*>(as_status);
}

TQueryResult ExecuteQuerySync(TSession session_, char* query) {
    auto* session = static_cast<NYdb::NQuery::TSession*>(session_);
    auto result = session->ExecuteQuery(query, NYdb::NQuery::TTxControl::NoTx()).GetValueSync();
    return static_cast<void*>(new NYdb::NQuery::TExecuteQueryResult{std::move(result)});
}

TStatus RetryQuerySync(TQueryClient* client, SyncRetryable query, void* data) {
    NYdb::TStatus status = client->client.RetryQuerySync([query, data](NYdb::NQuery::TSession session) {
        NYdb::TStatus* status = static_cast<NYdb::TStatus*>(query(static_cast<TSession>(&session), data));

        NYdb::TStatus status_value = std::move(*status);
        delete status;
        return status_value;
    });

    return static_cast<void*>(new NYdb::TStatus(std::move(status)));
}

}
