#ifndef YDB_C_SDK_QUERY_H_
#define YDB_C_SDK_QUERY_H_

#include "driver.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct TQueryClient TQueryClient;

TQueryClient* CreateQueryClient(TDriver* driver);
void DestroyQueryClient(TQueryClient* client);

#ifdef __cplusplus
}
#endif

#endif