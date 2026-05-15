#ifndef YDB_C_SDK_RESULTS_H_
#define YDB_C_SDK_RESULTS_H_

#include "datetime.h"
#include "helpers.h"

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

YDB_C_SDK_OPAQUE_STRUCT(YdbStatus)

YdbStatus YdbStatusOk();

void YdbDestroyStatus(YdbStatus status);

bool YdbIsSuccess(YdbStatus status);
bool YdbIsTransportError(YdbStatus status);

// Deallocate using free()
char* YdbGetErrorMessage(YdbStatus status);

YDB_C_SDK_OPAQUE_STRUCT(YdbAsyncStatus)

void YdbDestroyAsyncStatus(YdbAsyncStatus status);
YdbStatus YdbGetSyncStatus(YdbAsyncStatus status);

#define YDB_C_SDK_RESULT(name_no_ydb)                                          \
    YDB_C_SDK_OPAQUE_STRUCT(Ydb##name_no_ydb)                                  \
    void YdbDestroy##name_no_ydb(Ydb##name_no_ydb result);                     \
    /* Doesn't copy, don't destroy both */                                     \
    YdbStatus Ydb##name_no_ydb##AsStatus(Ydb##name_no_ydb result);             \
                                                                               \
    YDB_C_SDK_OPAQUE_STRUCT(YdbAsync##name_no_ydb)                             \
    void YdbDestroyAsync##name_no_ydb(YdbAsync##name_no_ydb result);           \
    /* Destroys async result, no need to call  YdbDestroyAsync* */             \
    Ydb##name_no_ydb YdbGetSync##name_no_ydb(YdbAsync##name_no_ydb result);

YDB_C_SDK_RESULT(QueryResult)

YDB_C_SDK_OPAQUE_STRUCT(YdbResultSet)

YdbResultSet YdbGetResultSet(YdbQueryResult result, int result_index);
void DestroyResultSet(YdbResultSet result_set);

YDB_C_SDK_OPAQUE_STRUCT(YdbResultSetParser)

YdbResultSetParser YdbCreateResultSetParser(YdbResultSet result_set);
void YdbDestroyResultSetParser(YdbResultSetParser result_set_parser);

bool YdbNextRow(YdbResultSetParser result_set_parser);

YDB_C_SDK_OPAQUE_STRUCT(YdbValueParser)

// References result_set_parser
YdbValueParser YdbColumnParser(YdbResultSetParser result_set_parser,
                               char* column_name);

// If exists is NULL treats it as required
uint64_t YdbParseUint64(YdbValueParser parser, bool* exists);

// May return NULL
char* YdbParseUtf8(YdbValueParser parser);

// ok may be NULL to ignore errors
YdbInstant YdbParseDate(YdbValueParser parser, bool* ok);

#ifdef __cplusplus
}
#endif

#endif