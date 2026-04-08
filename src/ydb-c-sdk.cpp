#include "ydb-c-sdk.h"

#include <ydb-cpp-sdk/client/driver/driver.h>
#include <ydb-cpp-sdk/client/query/query.h>
#include <ydb-cpp-sdk/client/query/client.h>

extern "C" {

struct TDriverConfig {
    NYdb::TDriverConfig config{};
};

TDriverConfig* CreateDriverConfig() {
    return new TDriverConfig{};
}

TDriverConfig* CreateDriverConfigConnectionString(char* connectionString) {
    return new TDriverConfig{{connectionString}};
}

void DestroyDriverConfig(TDriverConfig* config) {
    delete config;
}

void DriverConfigSetEndpoint(TDriverConfig* config, char* endpoint) {
    config->config.SetEndpoint(endpoint);
}

void DriverConfigSetDatabase(TDriverConfig* config, char* database) {
    config->config.SetDatabase(database);
}

void DriverConfigUseSecureConnection(TDriverConfig* config, char* caCert) {
    config->config.UseSecureConnection(caCert);
}

struct TDriver {
    NYdb::TDriver driver;
};

TDriver* CreateDriver(TDriverConfig* config) {
    return new TDriver{{config->config}};
}

void DestroyDriver(TDriver* driver) {
    delete driver;
}

void StopDriver(TDriver* driver, bool wait) {
    driver->driver.Stop(wait);
    delete driver;
}

struct TQueryClient {
    NYdb::NQuery::TQueryClient client;
};

TQueryClient* CreateQueryClient(TDriver* driver) {
    return new TQueryClient{{driver->driver}};
}

void DestroyQueryClient(TQueryClient* client) {
    delete client;
}

}
