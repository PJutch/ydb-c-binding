#ifndef YDB_C_SDK_DATETIME_H_
#define YDB_C_SDK_DATETIME_H_

#include "helpers.h"

#ifdef __cplusplus
extern "C" {
#endif

YDB_C_SDK_OPAQUE_STRUCT(YdbInstant)

void YdbDestroyInstant(YdbInstant instant);

YdbInstant YdbParseIso8601(char* date);

#ifdef __cplusplus
}
#endif

#endif