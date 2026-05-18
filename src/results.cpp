#include "ydb-c-sdk/results.h"

#include "helpers.hpp"
#include "result_helpers.hpp"

#include <__ydb_sdk_special_headers/util/datetime/base.h>
#include <__ydb_sdk_special_headers/util/generic/string.h>
#include <__ydb_sdk_special_headers/util/stream/str.h>
#include <ydb-cpp-sdk/client/query/client.h>
#include <ydb-cpp-sdk/client/query/query.h>
#include <ydb-cpp-sdk/client/result/result.h>

extern "C" {

YdbStatus YdbStatusOk() {
    return TO_NEW_OPAQUE(NYdb::TStatus, NYdb::TStatus(NYdb::EStatus::SUCCESS,
                                                      NYdb::NIssue::TIssues()));
}

YdbStatus YdbStatusError() {
    return TO_NEW_OPAQUE(
        NYdb::TStatus,
        NYdb::TStatus(NYdb::EStatus::GENERIC_ERROR, NYdb::NIssue::TIssues()));
}

void YdbDestroyStatus(YdbStatus status) {
    delete YdbPtrFromOpaque<NYdb::TStatus>(status);
}

bool YdbIsSuccess(YdbStatus status) {
    return YdbFromOpaque<NYdb::TStatus>(status).IsSuccess();
}

bool YdbIsTransportError(YdbStatus status) {
    return YdbFromOpaque<NYdb::TStatus>(status).IsTransportError();
}

char* YdbGetErrorMessage(YdbStatus status) {
    TString string;
    TStringOutput stream{string};

    stream << "status: " << YdbFromOpaque<NYdb::TStatus>(status).GetStatus()
           << '\n'
           << YdbFromOpaque<NYdb::TStatus>(status).GetIssues().ToString();

    return strdup(string.data());
}

void YdbDestroyAsyncStatus(YdbAsyncStatus status) {
    delete YdbPtrFromOpaque<NYdb::TAsyncStatus>(status);
}

YdbStatus YdbGetSyncStatus(YdbAsyncStatus status) {
    return TO_NEW_OPAQUE(
        NYdb::TStatus,
        YdbFromOpaque<NYdb::TAsyncStatus>(status).ExtractValueSync());
}

YDB_C_SDK_RESULT_IMPL(QueryResult, NYdb::NQuery::TExecuteQueryResult,
                      NYdb::NQuery::TAsyncExecuteQueryResult)

YdbResultSet YdbGetResultSet(YdbQueryResult result, int result_index) {
    return {new NYdb::TResultSet{
        YdbFromOpaque<NYdb::NQuery::TExecuteQueryResult>(result).GetResultSet(
            result_index)}};
}

void YdbDestroyResultSet(YdbResultSet result_set) {
    delete YdbPtrFromOpaque<NYdb::TResultSet>(result_set);
}

YdbResultSetParser YdbCreateResultSetParser(YdbResultSet result_set) {
    return {new NYdb::TResultSetParser{
        YdbFromOpaque<NYdb::TResultSet>(result_set)}};
}

void YdbDestroyResultSetParser(YdbResultSetParser result_set_parser) {
    delete YdbPtrFromOpaque<NYdb::TResultSetParser>(result_set_parser);
}

bool YdbNextRow(YdbResultSetParser result_set_parser) {
    return YdbFromOpaque<NYdb::TResultSetParser>(result_set_parser)
        .TryNextRow();
}

YdbValueParser YdbColumnParser(YdbResultSetParser result_set_parser,
                               char* column_name) {
    return {&YdbFromOpaque<NYdb::TResultSetParser>(result_set_parser)
                 .ColumnParser(column_name)};
}

#define YDB_C_SDK_PARSE(suffix, result_type)                                   \
    char* YdbParse##suffix(YdbValueParser parser, result_type* output) {       \
        try {                                                                  \
            *output = YdbFromOpaque<NYdb::TValueParser>(parser).Get##suffix(); \
            return nullptr;                                                    \
        } catch (std::exception& exception) {                                   \
            return strdup(exception.what());                                   \
        }                                                                      \
    }                                                                          \
                                                                               \
    char* YdbParseOptional##suffix(YdbValueParser parser, result_type* output, \
                                   bool* exists) {                             \
        try {                                                                  \
            if (auto result = YdbFromOpaque<NYdb::TValueParser>(parser)        \
                                  .GetOptional##suffix()) {                    \
                *output = *result;                                             \
                *exists = true;                                                \
            } else {                                                           \
                *exists = false;                                               \
            }                                                                  \
            return nullptr;                                                    \
        } catch (std::exception& exception) {                                   \
            return strdup(exception.what());                                   \
        }                                                                      \
    }

YDB_C_SDK_PARSE(Bool, bool)
YDB_C_SDK_PARSE(Int8, int8_t)
YDB_C_SDK_PARSE(Uint8, uint8_t)
YDB_C_SDK_PARSE(Int16, int16_t)
YDB_C_SDK_PARSE(Uint16, uint16_t)
YDB_C_SDK_PARSE(Int32, int32_t)
YDB_C_SDK_PARSE(Uint32, uint32_t)
YDB_C_SDK_PARSE(Int64, int64_t)
YDB_C_SDK_PARSE(Uint64, uint64_t)

#define YDB_C_SDK_PARSE_INSTANT(suffix)                                        \
    char* YdbParse##suffix(YdbValueParser parser, YdbInstant* output) {        \
        try {                                                                  \
            *output = YdbFromOpaque<NYdb::TValueParser>(parser)                \
                          .Get##suffix()                                       \
                          .GetValue();                                         \
            return nullptr;                                                    \
        } catch (std::exception& exception) {                                   \
            return strdup(exception.what());                                   \
        }                                                                      \
    }                                                                          \
                                                                               \
    char* YdbParseOptional##suffix(YdbValueParser parser, YdbInstant* output,  \
                                   bool* exists) {                             \
        try {                                                                  \
            if (auto result = YdbFromOpaque<NYdb::TValueParser>(parser)        \
                                  .GetOptional##suffix()) {                    \
                *output = result->GetValue();                                  \
                *exists = true;                                                \
            } else {                                                           \
                *exists = false;                                               \
            }                                                                  \
            return nullptr;                                                    \
        } catch (std::exception& exception) {                                   \
            return strdup(exception.what());                                   \
        }                                                                      \
    }

YDB_C_SDK_PARSE_INSTANT(Date)
YDB_C_SDK_PARSE_INSTANT(Datetime)
YDB_C_SDK_PARSE_INSTANT(Timestamp)

#define YDB_C_SDK_PARSE_STRING(suffix)                                         \
    char* YdbParse##suffix(YdbValueParser parser, char** output) {             \
        try {                                                                  \
            *output = strdup(YdbFromOpaque<NYdb::TValueParser>(parser)         \
                                 .Get##suffix()                                \
                                 .c_str());                                    \
            return nullptr;                                                    \
        } catch (std::exception& exception) {                                   \
            return strdup(exception.what());                                   \
        }                                                                      \
    }                                                                          \
                                                                               \
    char* YdbParseOptional##suffix(YdbValueParser parser, char** output) {     \
        try {                                                                  \
            if (auto result = YdbFromOpaque<NYdb::TValueParser>(parser)        \
                                  .GetOptional##suffix()) {                    \
                *output = strdup(result->c_str());                             \
            } else {                                                           \
                *output = nullptr;                                             \
            }                                                                  \
            return nullptr;                                                    \
        } catch (std::exception& exception) {                                   \
            return strdup(exception.what());                                   \
        }                                                                      \
    }

YDB_C_SDK_PARSE_STRING(Bytes)
YDB_C_SDK_PARSE_STRING(Utf8)
YDB_C_SDK_PARSE_STRING(Text)
YDB_C_SDK_PARSE_STRING(Yson)
YDB_C_SDK_PARSE_STRING(Json)
}