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

}

// expects mode != TX_TRANSACTION
NYdb::NQuery::TTxSettings CreateTxSettings(TxMode mode, bool allow_inconsistent_reads) {
    switch (mode) {
    case TX_SERIALIZABLE_RW:
        return NYdb::NQuery::TTxSettings::SerializableRW();
    case TX_ONLINE_RO:
        return NYdb::NQuery::TTxSettings::OnlineRO(
            NYdb::NQuery::TTxOnlineSettings{}.AllowInconsistentReads(allow_inconsistent_reads));
    case TX_STALE_RO:
        return NYdb::NQuery::TTxSettings::StaleRO();
    case TX_SNAPSHOT_RO:
        return NYdb::NQuery::TTxSettings::SnapshotRO();
    case TX_SNAPSHOT_RW:
        return NYdb::NQuery::TTxSettings::SnapshotRW();
    default:
        assert(false);
    }
}

NYdb::NQuery::TTxControl CreateTx(TTx* tx_) {
    if (!tx_) {
        return NYdb::NQuery::TTxControl::NoTx();
    } else if (tx_->mode == TX_TRANSACTION) {
        return NYdb::NQuery::TTxControl::Tx(
            *static_cast<NYdb::NQuery::TTransaction*>(tx_->transaction));
    } else {
        return NYdb::NQuery::TTxControl::BeginTx(
                CreateTxSettings(tx_->mode, tx_->allow_inconsistent_reads))
            .CommitTx(tx_->commit);
    }
}

extern "C" {

TQueryResult ExecuteQuerySync(TSession session_, char* query, TTx* tx_, TParams params_) {
    auto* session = static_cast<NYdb::NQuery::TSession*>(session_);
    auto* params = static_cast<NYdb::TParams*>(params_);
    auto future = params ? session->ExecuteQuery(query, CreateTx(tx_), *params) 
                         : session->ExecuteQuery(query, CreateTx(tx_));
    return static_cast<void*>(new NYdb::NQuery::TExecuteQueryResult{future.GetValueSync()});
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
