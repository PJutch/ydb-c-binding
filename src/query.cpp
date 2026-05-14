#include "ydb-c-sdk/query.h"

#include "helpers.hpp"

#include <ydb-cpp-sdk/client/query/client.h>
#include <ydb-cpp-sdk/client/query/query.h>

extern "C" {

YdbQueryClient YdbCreateQueryClient(YdbDriver driver) {
    return {
        new NYdb::NQuery::TQueryClient{YdbFromOpaque<NYdb::TDriver>(driver)}};
}

void YdbDestroyQueryClient(YdbQueryClient client) {
    delete YdbPtrFromOpaque<NYdb::NQuery::TQueryClient>(client);
}

void YdbDestroyCreateSessionResult(YdbCreateSessionResult result) {
    delete YdbPtrFromOpaque<NYdb::NQuery::TQueryClient>(result);
}

YdbStatus YdbCreateSessionResultAsStatus(YdbCreateSessionResult result) {
    auto* as_status = static_cast<NYdb::TStatus*>(
        YdbPtrFromOpaque<NYdb::NQuery::TExecuteQueryResult>(result));
    return PTR_TO_OPAQUE(as_status);
}

YdbSession YdbCreateSessionResultGetSession(YdbCreateSessionResult result) {
    return TO_NEW_OPAQUE(
        NYdb::NQuery::TSession,
        YdbFromOpaque<NYdb::NQuery::TCreateSessionResult>(result).GetSession());
}

YdbCreateSessionResult YdbCreateSessionSync(YdbQueryClient client) {
    return TO_NEW_OPAQUE(NYdb::NQuery::TCreateSessionResult,
                         YdbFromOpaque<NYdb::NQuery::TQueryClient>(client)
                             .GetSession()
                             .GetValueSync());
}
}

// expects mode != YdbX_TRANSACTION
NYdb::NQuery::TTxSettings
YdbCreateTxSettings(YdbTxMode mode, bool allow_inconsistent_reads) {
    switch (mode) {
    case YDB_TX_SERIALIZABLE_RW:
        return NYdb::NQuery::TTxSettings::SerializableRW();
    case YDB_TX_ONLINE_RO:
        return NYdb::NQuery::TTxSettings::OnlineRO(
            NYdb::NQuery::TTxOnlineSettings{}.AllowInconsistentReads(
                allow_inconsistent_reads));
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

NYdb::NQuery::TTxControl YdbCreateTx(YdbTx* tx_) {
    if (!tx_) {
        return NYdb::NQuery::TTxControl::NoTx();
    } else if (tx_->mode == YDB_TX_TRANSACTION) {
        return NYdb::NQuery::TTxControl::Tx(
            YdbFromOpaque<NYdb::NQuery::TTransaction>(tx_->transaction));
    } else {
        return NYdb::NQuery::TTxControl::BeginTx(
                   YdbCreateTxSettings(tx_->mode,
                                       tx_->allow_inconsistent_reads))
            .CommitTx(tx_->commit);
    }
}

extern "C" {

void YdbDestroyBeginTransactionResult(YdbBeginTransactionResult result) {
    delete YdbPtrFromOpaque<NYdb::NQuery::TBeginTransactionResult>(result);
}

YdbStatus
YdbBeginTransactionResultAsStatus(YdbBeginTransactionResult result) {
    auto* as_status = static_cast<NYdb::TStatus*>(
        YdbPtrFromOpaque<NYdb::NQuery::TExecuteQueryResult>(result));
    return PTR_TO_OPAQUE(as_status);
}

YdbTransaction YdbBeginTransactionResultGetTransaction(
    YdbBeginTransactionResult result) {
    return TO_NEW_OPAQUE(
        NYdb::NQuery::TTransaction,
        YdbFromOpaque<NYdb::NQuery::TBeginTransactionResult>(result)
            .GetTransaction());
}

YdbBeginTransactionResult YdbBeginTransactionSync(YdbSession session,
                                               YdbTxMode mode,
                                               bool allow_inconsistent_reads) {
    return TO_NEW_OPAQUE(NYdb::NQuery::TBeginTransactionResult,
                         YdbFromOpaque<NYdb::NQuery::TSession>(session)
                             .BeginTransaction(YdbCreateTxSettings(
                                 mode, allow_inconsistent_reads))
                             .GetValueSync());
}

YdbStatus YdbCommitSync(YdbTransaction transaction) {
    return PTR_TO_OPAQUE(
        static_cast<NYdb::TStatus*>(new NYdb::NQuery::TCommitTransactionResult{
            YdbFromOpaque<NYdb::NQuery::TTransaction>(transaction)
                .Commit()
                .GetValueSync()}));
}

YdbQueryResult YdbExecuteQuerySync(YdbSession session_, char* query, YdbTx* tx_,
                                   YdbParams params_) {
    auto& session = YdbFromOpaque<NYdb::NQuery::TSession>(session_);
    auto* params = YdbPtrFromOpaque<NYdb::TParams>(params_);
    auto future = params
                      ? session.ExecuteQuery(query, YdbCreateTx(tx_), *params)
                      : session.ExecuteQuery(query, YdbCreateTx(tx_));
    return {static_cast<void*>(
        new NYdb::NQuery::TExecuteQueryResult{future.GetValueSync()})};
}

YdbTransaction YdbQueryTransaction(YdbQueryResult result) {
    if (auto transaction =
            YdbFromOpaque<NYdb::NQuery::TExecuteQueryResult>(result)
                .GetTransaction()) {
        return TO_NEW_OPAQUE(NYdb::NQuery::TTransaction, *transaction);
    } else {
        return NULL_TRANSACTION;
    }
}

YdbStatus YdbRetryQuerySync(YdbQueryClient client, YdbSyncRetryable query,
                            void* data) {
    NYdb::TStatus status =
        YdbFromOpaque<NYdb::NQuery::TQueryClient>(client).RetryQuerySync(
            [query, data](NYdb::NQuery::TSession session) {
                NYdb::TStatus* status = YdbPtrFromOpaque<NYdb::TStatus>(
                    query({static_cast<void*>(&session)}, data));

                NYdb::TStatus status_value = std::move(*status);
                delete status;
                return status_value;
            });

    return {static_cast<void*>(new NYdb::TStatus(std::move(status)))};
}

YdbStatus YdbRetryQuerySyncNoSession(YdbQueryClient client,
                                     YdbSyncRetryableNoSession query,
                                     void* data) {
    NYdb::TStatus status =
        YdbFromOpaque<NYdb::NQuery::TQueryClient>(client).RetryQuerySync(
            [query, data](NYdb::NQuery::TQueryClient client) {
                NYdb::TStatus* status = YdbPtrFromOpaque<NYdb::TStatus>(
                    query({static_cast<void*>(&client)}, data));

                NYdb::TStatus status_value = std::move(*status);
                delete status;
                return status_value;
            });

    return {static_cast<void*>(new NYdb::TStatus(std::move(status)))};
}
}
