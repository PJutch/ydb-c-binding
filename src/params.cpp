#include "ydb-c-sdk/params.h"
#include "ydb-c-sdk/datetime.h"

#include "helpers.hpp"

#include <__ydb_sdk_special_headers/util/datetime/base.h>
#include <ydb-cpp-sdk/client/params/params.h>

extern "C" {

YdbParamsBuilder YdbCreateParamsBuilder() {
    return {static_cast<void*>(new NYdb::TParamsBuilder{})};
}

YdbParamValueBuilder YdbAddParam(YdbParamsBuilder builder, char* name) {
    return {static_cast<void*>(
        &YdbFromOpaque<NYdb::TParamsBuilder>(builder).AddParam(name))};
}

#define YDB_C_SDK_PARAM_ACTION(name)                                           \
    void YdbParam##name(YdbParamValueBuilder builder) {                        \
        YdbFromOpaque<NYdb::TParamValueBuilder>(builder).name();               \
    }

YDB_C_SDK_PARAM_ACTION(BeginList)
YDB_C_SDK_PARAM_ACTION(AddListItem)
YDB_C_SDK_PARAM_ACTION(EndList)
YDB_C_SDK_PARAM_ACTION(BeginStruct)
YDB_C_SDK_PARAM_ACTION(EndStruct)

#define YDB_C_SDK_PARAM_ACTION_ARG(name, arg_type)                             \
    void YdbParam##name(YdbParamValueBuilder builder_, arg_type value) {       \
        auto& builder = YdbFromOpaque<NYdb::TParamValueBuilder>(builder_);     \
        builder.name(value);                                                   \
    }

YDB_C_SDK_PARAM_ACTION_ARG(AddMember, char*)
YDB_C_SDK_PARAM_ACTION_ARG(Utf8, char*)
YDB_C_SDK_PARAM_ACTION_ARG(Uint8, uint8_t)
YDB_C_SDK_PARAM_ACTION_ARG(Uint64, uint64_t)

void YdbParamDate(YdbParamValueBuilder builder, YdbInstant value) {
    YdbFromOpaque<NYdb::TParamValueBuilder>(builder).Date(
        TInstant::FromValue(value));
}

void YdbBuildParamValue(YdbParamValueBuilder builder) {
    YdbFromOpaque<NYdb::TParamValueBuilder>(builder).Build();
}

YdbParams YdbBuildParams(YdbParamsBuilder builder) {
    NYdb::TParams params = YdbFromOpaque<NYdb::TParamsBuilder>(builder).Build();
    delete YdbPtrFromOpaque<NYdb::TParamsBuilder>(builder);
    return TO_NEW_OPAQUE(NYdb::TParams, std::move(params));
}

void YdbDestroyParams(YdbParams params) {
    delete YdbPtrFromOpaque<NYdb::TParams>(params);
}
}
