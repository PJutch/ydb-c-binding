#ifndef YDB_C_SDK_DRIVER_H_
#define YDB_C_SDK_DRIVER_H_

#include "helpers.h"

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

YDB_C_SDK_OPAQUE_STRUCT(YdbDriverConfig)

YdbDriverConfig CreateDriverConfig();
YdbDriverConfig CreateDriverConfigConnectionString(char* connectionString);

void DestroyDriverConfig(YdbDriverConfig config);

void DriverConfigSetEndpoint(YdbDriverConfig config, char* endpoint);
void DriverConfigSetDatabase(YdbDriverConfig config, char* database);
void DriverConfigUseSecureConnection(YdbDriverConfig config, char* caCert);

YDB_C_SDK_OPAQUE_STRUCT(YdbDriver)

YdbDriver CreateDriver(YdbDriverConfig config);
void StopDriver(YdbDriver driver, bool wait);

#ifdef __cplusplus
}
#endif

#endif