#ifndef YDB_STREAM_H_
#define YDB_STREAM_H_

#include "helpers.h"
#include "query.h"
#include "results.h"
#include "params.h"

#ifdef __cplusplus
extern "C" {
#endif

YDB_C_SDK_OPAQUE_STRUCT(YdbExecuteQueryIterator)

void YdbDestroyExecuteQueryIterator(YdbExecuteQueryIterator result);
YdbStatus YdbExecuteQueryIteratorAsStatus(YdbExecuteQueryIterator result);

YdbExecuteQueryIterator YdbStreamExecuteQuerySync(YdbQueryClient client,
                                                  char* query, YdbTx* tx,
                                                  YdbParams params);


YDB_C_SDK_OPAQUE_STRUCT(YdbExecuteQueryPart)

void YdbDestroyExecuteQueryPart(YdbExecuteQueryPart result);
YdbStatus YdbExecuteQueryPartAsStatus(YdbExecuteQueryPart result);

YdbExecuteQueryPart YdbReadNextSync(YdbExecuteQueryIterator iterator);

bool YdbIsEos(YdbExecuteQueryPart part);
bool YdbExecuteQueryPartHasResultSet(YdbExecuteQueryPart part);
YdbResultSet YdbExecuteQueryPartGetResultSet(YdbExecuteQueryPart part);

#ifdef __cplusplus
}
#endif

#endif