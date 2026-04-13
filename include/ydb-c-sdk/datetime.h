#ifndef YDB_C_SDK_DATETIME_H_
#define YDB_C_SDK_DATETIME_H_

#include "helpers.h"

#ifdef __cplusplus
extern "C" {
#endif

YDB_C_SDK_OPAQUE_STRUCT(YdbInstant)

#define YDB_NULL_INSTANT (YdbInstant){NULL}

void YdbDestroyInstant(YdbInstant instant);

YdbInstant YdbInstantParseIso8601(char* date);

// Deallocate using free()
char* YdbFormatLocalTime(YdbInstant instant, char* format_string);

#ifdef __cplusplus
}
#endif

#endif