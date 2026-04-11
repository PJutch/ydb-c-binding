#ifndef YDB_C_SDK_DATETIME_H_
#define YDB_C_SDK_DATETIME_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef void* YdbInstant;

void DestroyInstant(YdbInstant instant);

YdbInstant ParseIso8601(char* date);

#ifdef __cplusplus
}
#endif

#endif