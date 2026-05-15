#include "ydb-c-sdk/stream.h"

#include "helpers.hpp"
#include "result_helpers.hpp"
#include "tx_helprers.hpp"

#include <ydb-cpp-sdk/client/params/params.h>
#include <ydb-cpp-sdk/client/query/client.h>
#include <ydb-cpp-sdk/client/query/query.h>

extern "C" {
YDB_C_SDK_RESULT_IMPL(ExecuteQueryIterator, NYdb::NQuery::TExecuteQueryIterator,
                      NYdb::NQuery::TAsyncExecuteQueryIterator)

YdbAsyncExecuteQueryIterator YdbStreamExecuteQuery(YdbQueryClient client_,
                                                       char* query, YdbTx* tx,
                                                       YdbParams params_) {
    auto& client = YdbFromOpaque<NYdb::NQuery::TQueryClient>(client_);
    auto* params = YdbPtrFromOpaque<NYdb::TParams>(params_);
    auto future =
        params ? client.StreamExecuteQuery(query, YdbCreateTx(tx), *params)
               : client.StreamExecuteQuery(query, YdbCreateTx(tx));
    return TO_NEW_OPAQUE(NYdb::NQuery::TAsyncExecuteQueryIterator, future);
}

YDB_C_SDK_RESULT_IMPL(ExecuteQueryPart, NYdb::NQuery::TExecuteQueryPart,
                      NYdb::NQuery::TAsyncExecuteQueryPart)

YdbAsyncExecuteQueryPart YdbReadNext(YdbExecuteQueryIterator iterator) {
    return TO_NEW_OPAQUE(
        NYdb::NQuery::TAsyncExecuteQueryPart,
        YdbFromOpaque<NYdb::NQuery::TExecuteQueryIterator>(iterator)
            .ReadNext());
}

bool YdbIsEos(YdbExecuteQueryPart part) {
    return YdbFromOpaque<NYdb::NQuery::TExecuteQueryPart>(part).EOS();
}

bool YdbExecuteQueryPartHasResultSet(YdbExecuteQueryPart part) {
    return YdbFromOpaque<NYdb::NQuery::TExecuteQueryPart>(part).HasResultSet();
}

YdbResultSet YdbExecuteQueryPartGetResultSet(YdbExecuteQueryPart part) {
    return TO_NEW_OPAQUE(
        NYdb::TResultSet,
        YdbFromOpaque<NYdb::NQuery::TExecuteQueryPart>(part).GetResultSet());
}
}