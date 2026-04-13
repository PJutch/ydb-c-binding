#ifndef YDB_C_SDK_DRIVER_H_
#define YDB_C_SDK_DRIVER_H_

#include "helpers.h"

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

YDB_C_SDK_OPAQUE_STRUCT(YdbDriverConfig)

YdbDriverConfig YdbCreateDriverConfig();
YdbDriverConfig YdbCreateDriverConfigConnectionString(char* connectionString);

void YdbDestroyDriverConfig(YdbDriverConfig config);

void YdbDriverConfigSetEndpoint(YdbDriverConfig config, char* endpoint);
void YdbDriverConfigSetDatabase(YdbDriverConfig config, char* database);
void YdbDriverConfigUseSecureConnection(YdbDriverConfig config, char* caCert);

YDB_C_SDK_OPAQUE_STRUCT(YdbDriver)

YdbDriver YdbCreateDriver(YdbDriverConfig config);
void YdbStopDriver(YdbDriver driver, bool wait);

#ifdef __cplusplus
}
#endif

#endif