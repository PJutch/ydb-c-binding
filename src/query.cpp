#include "ydb-c-sdk/query.h"

#include "helpers.hpp"
#include "result_helpers.hpp"

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

void YdbDestroySession(YdbSession session) {
    delete YdbPtrFromOpaque<NYdb::NQuery::TSession>(session);
}

YDB_C_SDK_RESULT_IMPL(CreateSessionResult, NYdb::NQuery::TCreateSessionResult,
                      NYdb::NQuery::TAsyncCreateSessionResult)

YdbSession YdbCreateSessionResultGetSession(YdbCreateSessionResult result) {
    return TO_NEW_OPAQUE(
        NYdb::NQuery::TSession,
        YdbFromOpaque<NYdb::NQuery::TCreateSessionResult>(result).GetSession());
}

YdbAsyncCreateSessionResult YdbCreateSession(YdbQueryClient client) {
    return TO_NEW_OPAQUE(
        NYdb::NQuery::TAsyncCreateSessionResult,
        YdbFromOpaque<NYdb::NQuery::TQueryClient>(client).GetSession());
}
}

// expects mode != YdbX_TRANSACTION
NYdb::NQuery::TTxSettings YdbCreateTxSettings(YdbTxMode mode,
                                              bool allow_inconsistent_reads) {
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

void YdbDestroyTransaction(YdbTransaction transaction) {
    delete YdbPtrFromOpaque<NYdb::NQuery::TTransaction>(transaction);
}

YDB_C_SDK_RESULT_IMPL(BeginTransactionResult,
                      NYdb::NQuery::TBeginTransactionResult,
                      NYdb::NQuery::TAsyncBeginTransactionResult)

YdbTransaction
YdbBeginTransactionResultGetTransaction(YdbBeginTransactionResult result) {
    return TO_NEW_OPAQUE(
        NYdb::NQuery::TTransaction,
        YdbFromOpaque<NYdb::NQuery::TBeginTransactionResult>(result)
            .GetTransaction());
}

YdbAsyncBeginTransactionResult
YdbBeginTransaction(YdbSession session, YdbTxMode mode,
                    bool allow_inconsistent_reads) {
    return TO_NEW_OPAQUE(
        NYdb::NQuery::TAsyncBeginTransactionResult,
        YdbFromOpaque<NYdb::NQuery::TSession>(session).BeginTransaction(
            YdbCreateTxSettings(mode, allow_inconsistent_reads)));
}

YDB_C_SDK_RESULT_IMPL(CommitResult, NYdb::NQuery::TCommitTransactionResult,
                      NYdb::NQuery::TAsyncCommitTransactionResult)

YdbAsyncCommitResult YdbCommit(YdbTransaction transaction) {
    return TO_NEW_OPAQUE(
        NYdb::NQuery::TAsyncCommitTransactionResult,
        YdbFromOpaque<NYdb::NQuery::TTransaction>(transaction).Commit());
}

YdbAsyncQueryResult YdbExecuteQuery(YdbSession session_, char* query,
                                    YdbTx* tx_, YdbParams params_) {
    auto& session = YdbFromOpaque<NYdb::NQuery::TSession>(session_);
    auto* params = YdbPtrFromOpaque<NYdb::TParams>(params_);
    auto future = params
                      ? session.ExecuteQuery(query, YdbCreateTx(tx_), *params)
                      : session.ExecuteQuery(query, YdbCreateTx(tx_));
    return TO_NEW_OPAQUE(NYdb::NQuery::TAsyncExecuteQueryResult, future);
};

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

    return TO_NEW_OPAQUE(NYdb::TStatus, std::move(status));
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

    return TO_NEW_OPAQUE(NYdb::TStatus, std::move(status));
}

YdbAsyncStatus YdbRetryQuery(YdbQueryClient client, YdbRetryable query,
                             void* data) {
    NYdb::TAsyncStatus status =
        YdbFromOpaque<NYdb::NQuery::TQueryClient>(client).RetryQuery(
            [query, data](NYdb::NQuery::TSession session) {
                NYdb::TAsyncStatus* status =
                    YdbPtrFromOpaque<NYdb::TAsyncStatus>(
                        query({static_cast<void*>(&session)}, data));

                NYdb::TAsyncStatus status_value = std::move(*status);
                delete status;
                return status_value;
            });

    return TO_NEW_OPAQUE(NYdb::TAsyncStatus, std::move(status));
}

YdbAsyncStatus YdbRetryQueryNoSession(YdbQueryClient client,
                                      YdbRetryableNoSession query, void* data) {
    NYdb::TAsyncStatus status =
        YdbFromOpaque<NYdb::NQuery::TQueryClient>(client).RetryQuery(
            [query, data](NYdb::NQuery::TQueryClient client) {
                NYdb::TAsyncStatus* status =
                    YdbPtrFromOpaque<NYdb::TAsyncStatus>(
                        query({static_cast<void*>(&client)}, data));

                NYdb::TAsyncStatus status_value = std::move(*status);
                delete status;
                return status_value;
            });

    return TO_NEW_OPAQUE(NYdb::TAsyncStatus, std::move(status));
}
}
