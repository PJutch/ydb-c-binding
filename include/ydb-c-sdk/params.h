#ifndef YDB_C_SDK_PARAMS_H_
#define YDB_C_SDK_PARAMS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "datetime.h"

#include <stdint.h>

typedef void* TParamsBuilder;

TParamsBuilder CreateParamsBuilder();

typedef void* TParamValueBuilder;

TParamValueBuilder AddParam(TParamsBuilder builder, char* name);

void ParamBeginList(TParamValueBuilder builder);
void ParamAddListItem(TParamValueBuilder builder);
void ParamEndList(TParamValueBuilder builder);

void ParamBeginStruct(TParamValueBuilder builder);
void ParamAddMember(TParamValueBuilder builder, char* name);
void ParamEndStruct(TParamValueBuilder builder);

void ParamUtf8(TParamValueBuilder builder, char* value);
void ParamUint8(TParamValueBuilder builder, uint8_t value);
void ParamUint64(TParamValueBuilder builder, uint64_t value);
void ParamDate(TParamValueBuilder builder, YdbInstant date);

typedef void* TParams;

void BuildParamValue(TParamValueBuilder builder);
TParams BuildParams(TParamsBuilder builder);

void DestroyParams(TParams);

#ifdef __cplusplus
}
#endif

#endif