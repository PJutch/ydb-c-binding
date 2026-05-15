#include "ydb-c-sdk/stream.h"

#include "helpers.hpp"
#include "result_helpers.hpp"
#include "tx_helprers.hpp"

#include <ydb-cpp-sdk/client/params/params.h>
#include <ydb-cpp-sdk/client/query/client.h>
#include <ydb-cpp-sdk/client/query/query.h>

extern "C" {
YDB_C_SDK_RESULT_IMPL(ExecuteQueryIterator, NYdb::NQuery::TExecuteQueryIterator)

YdbExecuteQueryIterator YdbStreamExecuteQuerySync(YdbQueryClient client_,
                                                  char* query, YdbTx* tx,
                                                  YdbParams params_) {
    auto& client = YdbFromOpaque<NYdb::NQuery::TQueryClient>(client_);
    auto* params = YdbPtrFromOpaque<NYdb::TParams>(params_);
    auto future =
        params ? client.StreamExecuteQuery(query, YdbCreateTx(tx), *params)
               : client.StreamExecuteQuery(query, YdbCreateTx(tx));
    return {static_cast<void*>(
        new NYdb::NQuery::TExecuteQueryIterator{future.GetValueSync()})};
}

YDB_C_SDK_RESULT_IMPL(ExecuteQueryPart, NYdb::NQuery::TExecuteQueryPart)

YdbExecuteQueryPart YdbReadNextSync(YdbExecuteQueryIterator iterator) {
    return TO_NEW_OPAQUE(
        NYdb::NQuery::TExecuteQueryPart,
        YdbFromOpaque<NYdb::NQuery::TExecuteQueryIterator>(iterator)
            .ReadNext()
            .GetValueSync());
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