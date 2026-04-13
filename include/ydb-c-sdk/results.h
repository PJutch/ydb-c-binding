#ifndef YDB_C_SDK_RESULTS_H_
#define YDB_C_SDK_RESULTS_H_

#include "helpers.h"
#include "datetime.h"

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

YDB_C_SDK_OPAQUE_STRUCT(YdbQueryResult)

void YdbDestroyResult(YdbQueryResult result);

YDB_C_SDK_OPAQUE_STRUCT(YdbStatus)

void YdbDestroyStatus(YdbStatus status);

bool YdbIsSuccess(YdbStatus status);
bool YdbIsTransportError(YdbStatus status);

// Deallocate using free()
char* YdbGetErrorMessage(YdbStatus status);

// Doesn't copy, don't destroy both
YdbStatus YdbAsStatus(YdbQueryResult result);

YDB_C_SDK_OPAQUE_STRUCT(YdbResultSet)

YdbResultSet YdbGetResultSet(YdbQueryResult result, int result_index);
void DestroyResultSet(YdbResultSet result_set);

YDB_C_SDK_OPAQUE_STRUCT(YdbResultSetParser)

YdbResultSetParser YdbCreateResultSetParser(YdbResultSet result_set);
void YdbDestroyResultSetParser(YdbResultSetParser result_set_parser);

bool YdbNextRow(YdbResultSetParser result_set_parser);

YDB_C_SDK_OPAQUE_STRUCT(YdbValueParser)

// References result_set_parser
YdbValueParser YdbColumnParser(YdbResultSetParser result_set_parser, char* column_name);

// If exists is NULL treats it as required
uint64_t YdbParseUint64(YdbValueParser parser, bool* exists);

// May return NULL
char* YdbParseUtf8(YdbValueParser parser);

// May return NULL_INSTANT
YdbInstant YdbParseDate(YdbValueParser parser);

#ifdef __cplusplus
}
#endif

#endif