#include "ydb-c-sdk/params.h"
#include "ydb-c-sdk/datetime.h"

#include "helpers.hpp"

#include <ydb-cpp-sdk/client/params/params.h>
#include <__ydb_sdk_special_headers/util/datetime/base.h>

extern "C" {

YdbParamsBuilder YdbCreateParamsBuilder() {
    return {static_cast<void*>(new NYdb::TParamsBuilder{})};
}

YdbParamValueBuilder YdbAddParam(YdbParamsBuilder builder, char* name) {
    return {static_cast<void*>(&FROM_OPAQUE(NYdb::TParamsBuilder, builder).AddParam(name))};
}

#define YDB_C_SDK_PARAM_ACTION(name) \
    void YdbParam##name(YdbParamValueBuilder builder) { \
        FROM_OPAQUE(NYdb::TParamValueBuilder, builder).name(); \
    }

YDB_C_SDK_PARAM_ACTION(BeginList)
YDB_C_SDK_PARAM_ACTION(AddListItem)
YDB_C_SDK_PARAM_ACTION(EndList)
YDB_C_SDK_PARAM_ACTION(BeginStruct)
YDB_C_SDK_PARAM_ACTION(EndStruct)

#define YDB_C_SDK_PARAM_ACTION_ARG(name, arg_type) \
    void YdbParam##name(YdbParamValueBuilder builder, arg_type value) { \
        FROM_OPAQUE(NYdb::TParamValueBuilder, builder).name(value); \
    }

YDB_C_SDK_PARAM_ACTION_ARG(AddMember, char*)
YDB_C_SDK_PARAM_ACTION_ARG(Utf8, char*)
YDB_C_SDK_PARAM_ACTION_ARG(Uint8, uint8_t)
YDB_C_SDK_PARAM_ACTION_ARG(Uint64, uint64_t)

void YdbParamDate(YdbParamValueBuilder builder, YdbInstant value) { 
    FROM_OPAQUE(NYdb::TParamValueBuilder, builder).Date(TInstant::FromValue(value));
}

void YdbBuildParamValue(YdbParamValueBuilder builder) {
    FROM_OPAQUE(NYdb::TParamValueBuilder, builder).Build();
}

YdbParams YdbBuildParams(YdbParamsBuilder builder) {
    return {static_cast<void*>(new NYdb::TParams{FROM_OPAQUE(NYdb::TParamsBuilder, builder).Build()})};
}

void YdbDestroyParams(YdbParams params) {
    delete PTR_FROM_OPAQUE(NYdb::TParams, params);
}

}
