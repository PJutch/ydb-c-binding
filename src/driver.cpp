#include <ydb-cpp-sdk/client/driver/driver.h>

#include "ydb-c-sdk/driver.h"

#include "helpers.hpp"

extern "C" {

YdbDriverConfig YdbCreateDriverConfig() { return {new NYdb::TDriverConfig{}}; }

YdbDriverConfig YdbCreateDriverConfigConnectionString(char* connectionString) {
    return {new NYdb::TDriverConfig{connectionString}};
}

void YdbDestroyDriverConfig(YdbDriverConfig config) {
    delete &YdbFromOpaque<NYdb::TDriverConfig>(config);
}

void YdbDriverConfigSetEndpoint(YdbDriverConfig config, char* endpoint) {
    YdbFromOpaque<NYdb::TDriverConfig>(config).SetEndpoint(endpoint);
}

void YdbDriverConfigSetDatabase(YdbDriverConfig config, char* database) {
    YdbFromOpaque<NYdb::TDriverConfig>(config).SetDatabase(database);
}

void YdbDriverConfigUseSecureConnection(YdbDriverConfig config, char* caCert) {
    YdbFromOpaque<NYdb::TDriverConfig>(config).UseSecureConnection(caCert);
}

YdbDriver YdbCreateDriver(YdbDriverConfig config) {
    return {new NYdb::TDriver{YdbFromOpaque<NYdb::TDriverConfig>(config)}};
}

void YdbStopDriver(YdbDriver driver, bool wait) {
    YdbFromOpaque<NYdb::TDriver>(driver).Stop(wait);
    delete &YdbFromOpaque<NYdb::TDriver>(driver);
}
}
