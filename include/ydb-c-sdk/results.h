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
YdbStatus YdbStatusError();

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
    YdbStatus Ydb##name_no_ydb##GetStatus(Ydb##name_no_ydb result);            \
    /* Destroys result, don't call YdbDestroy* */                              \
    YdbStatus Ydb##name_no_ydb##ToStatus(Ydb##name_no_ydb result);             \
                                                                               \
    YDB_C_SDK_OPAQUE_STRUCT(YdbAsync##name_no_ydb)                             \
    void YdbDestroyAsync##name_no_ydb(YdbAsync##name_no_ydb result);           \
    /* Destroys async result, don't call  YdbDestroyAsync* */                  \
    Ydb##name_no_ydb YdbGetSync##name_no_ydb(YdbAsync##name_no_ydb result);

YDB_C_SDK_RESULT(QueryResult)

YDB_C_SDK_OPAQUE_STRUCT(YdbResultSet)

YdbResultSet YdbGetResultSet(YdbQueryResult result, int result_index);
void YdbDestroyResultSet(YdbResultSet result_set);

YDB_C_SDK_OPAQUE_STRUCT(YdbResultSetParser)

YdbResultSetParser YdbCreateResultSetParser(YdbResultSet result_set);
void YdbDestroyResultSetParser(YdbResultSetParser result_set_parser);

bool YdbNextRow(YdbResultSetParser result_set_parser);

YDB_C_SDK_OPAQUE_STRUCT(YdbValueParser)

// References result_set_parser
YdbValueParser YdbColumnParser(YdbResultSetParser result_set_parser,
                               char* column_name);

// deallocate error message using free()
// if it is null, no error occured
char* YdbParseBool(YdbValueParser parser, bool* result);
char* YdbParseInt8(YdbValueParser parser, int8_t* result);
char* YdbParseUint8(YdbValueParser parser, uint8_t* result);
char* YdbParseInt16(YdbValueParser parser, int16_t* result);
char* YdbParseUint16(YdbValueParser parser, uint16_t* result);
char* YdbParseInt32(YdbValueParser parser, int32_t* result);
char* YdbParseUint32(YdbValueParser parser, uint32_t* result);
char* YdbParseInt64(YdbValueParser parser, int64_t* result);
char* YdbParseUint64(YdbValueParser parser, uint64_t* result);
char* YdbParseFloat(YdbValueParser parser, float* result);
char* YdbParseDouble(YdbValueParser parser, double* result);
char* YdbParseDate(YdbValueParser parser, YdbInstant* result);
char* YdbParseInstant(YdbValueParser parser, YdbInstant* result);
char* YdbParseTimestamp(YdbValueParser parser, YdbInstant* result);

// deallocate result using free() too
char* YdbParseBytes(YdbValueParser parser, char** result);
char* YdbParseUtf8(YdbValueParser parser, char** result);
char* YdbParseText(YdbValueParser parser, char** result);
char* YdbParseYson(YdbValueParser parser, char** result);
char* YdbParseJson(YdbValueParser parser, char** result);

char* YdbParseOptionalBool(YdbValueParser parser, bool* result, bool* exists);
char* YdbParseOptionalInt8(YdbValueParser parser, int8_t* result, bool* exists);
char* YdbParseOptionalUint8(YdbValueParser parser, uint8_t* result, bool* exists);
char* YdbParseOptionalInt16(YdbValueParser parser, int16_t* result, bool* exists);
char* YdbParseOptionalUint16(YdbValueParser parser, uint16_t* result, bool* exists);
char* YdbParseOptionalInt32(YdbValueParser parser, int32_t* result, bool* exists);
char* YdbParseOptionalUint32(YdbValueParser parser, uint32_t* result, bool* exists);
char* YdbParseOptionalInt64(YdbValueParser parser, int64_t* result, bool* exists);
char* YdbParseOptionalUint64(YdbValueParser parser, uint64_t* result, bool* exists);
char* YdbParseOptionalFloat(YdbValueParser parser, float* result, bool* exists);
char* YdbParseOptionalDouble(YdbValueParser parser, double* result, bool* exists);
char* YdbParseOptionalDate(YdbValueParser parser, YdbInstant* result, bool* exists);
char* YdbParseOptionalInstant(YdbValueParser parser, YdbInstant* result, bool* exists);
char* YdbParseOptionalTimestamp(YdbValueParser parser, YdbInstant* result, bool* exists);

char* YdbParseOptionalBytes(YdbValueParser parser, char** result);
char* YdbParseOptionalUtf8(YdbValueParser parser, char** result);
char* YdbParseOptionalText(YdbValueParser parser, char** result);
char* YdbParseOptionalYson(YdbValueParser parser, char** result);
char* YdbParseOptionalJson(YdbValueParser parser, char** result);

#ifdef __cplusplus
}
#endif

#endif