#ifndef YDB_C_SDK_RETRY_H_
#define YDB_C_SDK_RETRY_H_

#include "helpers.h"
#include "results.h"

#ifdef __cplusplus
extern "C" {
#endif

YDB_C_SDK_OPAQUE_STRUCT(YdbRetrier)

YdbRetrier YdbCreateRetrier();

typedef enum YdbRetryNextStep {
    YDB_RETRY_RESET,
    YDB_RETRY_RETRY,
    YDB_RETRY_FINISH,
} YdbRetryNextStep;

YdbRetryNextStep YdbRetrierNext(YdbRetrier retrier, YdbStatus status);

uint32_t YdbGetRetryNumber(YdbRetrier);

void YdbDestroyRetrier(YdbRetrier retrier);

#ifdef __cplusplus
}
#endif

#endif