#ifndef YDB_C_SDK_PARAMS_H_
#define YDB_C_SDK_PARAMS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "datetime.h"

#include <stdint.h>

YDB_C_SDK_OPAQUE_STRUCT(YdbParamsBuilder)

YdbParamsBuilder CreateParamsBuilder();

YDB_C_SDK_OPAQUE_STRUCT(YdbParamValueBuilder)

YdbParamValueBuilder AddParam(YdbParamsBuilder builder, char* name);

void ParamBeginList(YdbParamValueBuilder builder);
void ParamAddListItem(YdbParamValueBuilder builder);
void ParamEndList(YdbParamValueBuilder builder);

void ParamBeginStruct(YdbParamValueBuilder builder);
void ParamAddMember(YdbParamValueBuilder builder, char* name);
void ParamEndStruct(YdbParamValueBuilder builder);

void ParamUtf8(YdbParamValueBuilder builder, char* value);
void ParamUint8(YdbParamValueBuilder builder, uint8_t value);
void ParamUint64(YdbParamValueBuilder builder, uint64_t value);
void ParamDate(YdbParamValueBuilder builder, YdbInstant date);

YDB_C_SDK_OPAQUE_STRUCT(YdbParams)

#define YDB_NULL_PARAMS ((YdbParams){NULL})

void BuildParamValue(YdbParamValueBuilder builder);
YdbParams BuildParams(YdbParamsBuilder builder);

void DestroyParams(YdbParams);

#ifdef __cplusplus
}
#endif

#endif