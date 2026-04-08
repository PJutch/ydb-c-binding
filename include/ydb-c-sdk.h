#ifndef WRAPPER_H_
#define WRAPPER_H_

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct TDriverConfig TDriverConfig;

TDriverConfig* CreateDriverConfig();
TDriverConfig* CreateDriverConfigConnectionString(char* connectionString);

void DestroyDriverConfig(TDriverConfig* config);

void DriverConfigSetEndpoint(TDriverConfig* config, char* endpoint);
void DriverConfigSetDatabase(TDriverConfig* config, char* database);
void DriverConfigUseSecureConnection(TDriverConfig* config, char* caCert);

typedef struct TDriver TDriver;

TDriver* CreateDriver(TDriverConfig* config);
void StopDriver(TDriver* driver, bool wait);

typedef struct TQueryClient TQueryClient;

TQueryClient* CreateQueryClient(TDriver* driver);
void DestroyQueryClient(TQueryClient* client);

#ifdef __cplusplus
}
#endif

#endif