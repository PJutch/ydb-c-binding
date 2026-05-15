#ifndef YDB_STREAM_H_
#define YDB_STREAM_H_

#include "helpers.h"
#include "query.h"
#include "results.h"
#include "params.h"

#ifdef __cplusplus
extern "C" {
#endif

YDB_C_SDK_RESULT(ExecuteQueryIterator)

YdbExecuteQueryIterator YdbStreamExecuteQuerySync(YdbQueryClient client,
                                                  char* query, YdbTx* tx,
                                                  YdbParams params);


YDB_C_SDK_RESULT(ExecuteQueryPart)

YdbExecuteQueryPart YdbReadNextSync(YdbExecuteQueryIterator iterator);

bool YdbIsEos(YdbExecuteQueryPart part);
bool YdbExecuteQueryPartHasResultSet(YdbExecuteQueryPart part);
YdbResultSet YdbExecuteQueryPartGetResultSet(YdbExecuteQueryPart part);

#ifdef __cplusplus
}
#endif

#endif