#ifndef YDB_C_SDK_PARAMS_H_
#define YDB_C_SDK_PARAMS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "datetime.h"

#include <stdint.h>

YDB_C_SDK_OPAQUE_STRUCT(YdbParamsBuilder)

YdbParamsBuilder YdbCreateParamsBuilder();

YDB_C_SDK_OPAQUE_STRUCT(YdbParamValueBuilder)

YdbParamValueBuilder YdbAddParam(YdbParamsBuilder builder, char* name);

void YdbParamBeginList(YdbParamValueBuilder builder);
void YdbParamAddListItem(YdbParamValueBuilder builder);
void YdbParamEndList(YdbParamValueBuilder builder);

void YdbParamBeginStruct(YdbParamValueBuilder builder);
void YdbParamAddMember(YdbParamValueBuilder builder, char* name);
void YdbParamEndStruct(YdbParamValueBuilder builder);

void YdbParamUtf8(YdbParamValueBuilder builder, char* value);
void YdbParamUint8(YdbParamValueBuilder builder, uint8_t value);
void YdbParamUint64(YdbParamValueBuilder builder, uint64_t value);
void YdbParamDate(YdbParamValueBuilder builder, YdbInstant date);

YDB_C_SDK_OPAQUE_STRUCT(YdbParams)

#define YDB_NULL_PARAMS ((YdbParams){NULL})

void YdbBuildParamValue(YdbParamValueBuilder builder);
YdbParams YdbBuildParams(YdbParamsBuilder builder);

void YdbDestroyParams(YdbParams);

#ifdef __cplusplus
}
#endif

#endif