#include "ydb-c-sdk/query.h"

#include "helpers.hpp"

#include <ydb-cpp-sdk/client/query/query.h>
#include <ydb-cpp-sdk/client/query/client.h>
#include <__ydb_sdk_special_headers/util/stream/str.h>
#include <__ydb_sdk_special_headers/util/generic/string.h>

extern "C" {

YdbQueryClient YdbCreateQueryClient(YdbDriver driver) {
    return {new NYdb::NQuery::TQueryClient{FROM_OPAQUE(NYdb::TDriver, driver)}};
}

void YdbDestroyQueryClient(YdbQueryClient client) {
    delete PTR_FROM_OPAQUE(NYdb::NQuery::TQueryClient, client);
}

void YdbDestroyStatus(YdbStatus status) {
    delete PTR_FROM_OPAQUE(NYdb::TStatus, status);
}

bool YdbIsSuccess(YdbStatus status) {
    return FROM_OPAQUE(NYdb::TStatus, status).IsSuccess();
}

bool YdbIsTransportError(YdbStatus status) {
    return FROM_OPAQUE(NYdb::TStatus, status).IsTransportError();
}

char* YdbGetErrorMessage(YdbStatus status) {
    TString string;
    TStringOutput stream{string};

    FROM_OPAQUE(NYdb::TStatus, status).Out(stream);

    return strdup(string.data());
}

void YdbDestroyErrorMessage(char* message) {
    free(message);
}

void YdbFreeResult(YdbQueryResult result) {
    free(result.data);
}

YdbStatus YdbAsStatus(YdbQueryResult result) {
    auto& as_status = static_cast<NYdb::TStatus&>(FROM_OPAQUE(NYdb::NQuery::TExecuteQueryResult, result));
    return {static_cast<void*>(&as_status)};
}

}

// expects mode != YdbX_TRANSACTION
static NYdb::NQuery::TTxSettings YdbCreateTxSettings(YdbxMode mode, bool allow_inconsistent_reads) {
    switch (mode) {
    case YDB_TX_SERIALIZABLE_RW:
        return NYdb::NQuery::TTxSettings::SerializableRW();
    case YDB_TX_ONLINE_RO:
        return NYdb::NQuery::TTxSettings::OnlineRO(
            NYdb::NQuery::TTxOnlineSettings{}.AllowInconsistentReads(allow_inconsistent_reads));
    case YDB_TX_STALE_RO:
        return NYdb::NQuery::TTxSettings::StaleRO();
    case YDB_TX_SNAPSHOT_RO:
        return NYdb::NQuery::TTxSettings::SnapshotRO();
    case YDB_TX_SNAPSHOT_RW:
        return NYdb::NQuery::TTxSettings::SnapshotRW();
    default:
        assert(false);
    }
}

static NYdb::NQuery::TTxControl YdbCreateTx(YdbTx* tx_) {
    if (!tx_) {
        return NYdb::NQuery::TTxControl::NoTx();
    } else if (tx_->mode == YDB_TX_TRANSACTION) {
        return NYdb::NQuery::TTxControl::Tx(FROM_OPAQUE(NYdb::NQuery::TTransaction, tx_->transaction));
    } else {
        return NYdb::NQuery::TTxControl::BeginTx(
                YdbCreateTxSettings(tx_->mode, tx_->allow_inconsistent_reads))
            .CommitTx(tx_->commit);
    }
}

extern "C" {

YdbQueryResult YdbExecuteQuerySync(YdbSession session_, char* query, YdbTx* tx_, YdbParams params_) {
    auto& session = FROM_OPAQUE(NYdb::NQuery::TSession, session_);
    auto* params = PTR_FROM_OPAQUE(NYdb::TParams, params_);
    auto future = params ? session.ExecuteQuery(query, YdbCreateTx(tx_), *params) 
                         : session.ExecuteQuery(query, YdbCreateTx(tx_));
    return {static_cast<void*>(new NYdb::NQuery::TExecuteQueryResult{future.GetValueSync()})};
}

YdbStatus YdbRetryQuerySync(YdbQueryClient client, YdbSyncRetryable query, void* data) {
    NYdb::TStatus status = FROM_OPAQUE(NYdb::NQuery::TQueryClient, client)
            .RetryQuerySync([query, data](NYdb::NQuery::TSession session) {
        NYdb::TStatus* status = PTR_FROM_OPAQUE(NYdb::TStatus, query({static_cast<void*>(&session)}, data));

        NYdb::TStatus status_value = std::move(*status);
        delete status;
        return status_value;
    });

    return {static_cast<void*>(new NYdb::TStatus(std::move(status)))};
}

}
