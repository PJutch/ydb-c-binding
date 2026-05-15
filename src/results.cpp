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

YDB_C_SDK_RESULT_IMPL(QueryResult, NYdb::NQuery::TExecuteQueryResult)

YdbResultSet YdbGetResultSet(YdbQueryResult result, int result_index) {
    return {new NYdb::TResultSet{
        YdbFromOpaque<NYdb::NQuery::TExecuteQueryResult>(result).GetResultSet(
            result_index)}};
}

void DestroyResultSet(YdbResultSet result_set) {
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

uint64_t YdbParseUint64(YdbValueParser parser_, bool* exists) {
    auto& parser = YdbFromOpaque<NYdb::TValueParser>(parser_);
    if (exists != nullptr) {
        if (auto result = parser.GetOptionalUint64()) {
            *exists = true;
            return *result;
        } else {
            *exists = false;
            return 0;
        }
    } else {
        return parser.GetUint64();
    }
}

char* YdbParseUtf8(YdbValueParser parser) {
    if (auto result =
            YdbFromOpaque<NYdb::TValueParser>(parser).GetOptionalUtf8()) {
        return strdup(result->c_str());
    } else {
        return nullptr;
    }
}

YdbInstant YdbParseDate(YdbValueParser parser, bool* ok) {
    if (auto result =
            YdbFromOpaque<NYdb::TValueParser>(parser).GetOptionalDate()) {
        if (ok) {
            *ok = true;
        }
        return result->GetValue();
    } else {
        if (ok) {
            *ok = false;
        }
        return YDB_INSTANT_MAX;
    }
}
}
