#include "ydb-c-sdk/params.h"
#include "ydb-c-sdk/datetime.h"

#include <ydb-cpp-sdk/client/params/params.h>
#include <__ydb_sdk_special_headers/util/datetime/base.h>

extern "C" {

TParamsBuilder CreateParamsBuilder() {
    return static_cast<void*>(new NYdb::TParamsBuilder{});
}

TParamValueBuilder AddParam(TParamsBuilder builder, char* name) {
    return static_cast<void*>(&static_cast<NYdb::TParamsBuilder*>(builder)->AddParam(name));
}

#define YDB_C_SDK_PARAM_ACTION(name) \
    void Param##name(TParamValueBuilder builder) { \
        static_cast<NYdb::TParamValueBuilder*>(builder)->name(); \
    }

YDB_C_SDK_PARAM_ACTION(BeginList)
YDB_C_SDK_PARAM_ACTION(AddListItem)
YDB_C_SDK_PARAM_ACTION(EndList)
YDB_C_SDK_PARAM_ACTION(BeginStruct)
YDB_C_SDK_PARAM_ACTION(EndStruct)

#define YDB_C_SDK_PARAM_ACTION_ARG(name, arg_type) \
    void Param##name(TParamValueBuilder builder, arg_type value) { \
        static_cast<NYdb::TParamValueBuilder*>(builder)->name(value); \
    }

YDB_C_SDK_PARAM_ACTION_ARG(AddMember, char*)
YDB_C_SDK_PARAM_ACTION_ARG(Utf8, char*)
YDB_C_SDK_PARAM_ACTION_ARG(Uint8, uint8_t)
YDB_C_SDK_PARAM_ACTION_ARG(Uint64, uint64_t)

void ParamDate(TParamValueBuilder builder, YdbInstant value) { 
    static_cast<NYdb::TParamValueBuilder*>(builder)->Date(*static_cast<TInstant*>(value)); 
}

void BuildParamValue(TParamValueBuilder builder) {
    static_cast<NYdb::TParamValueBuilder*>(builder)->Build();
}

TParams BuildParams(TParamsBuilder builder) {
    return static_cast<void*>(new NYdb::TParams{static_cast<NYdb::TParamsBuilder*>(builder)->Build()});
}

void DestroyParams(TParams params) {
    delete static_cast<NYdb::TParams*>(params);
}

}
