#include <ydb-cpp-sdk/client/driver/driver.h>

#include "ydb-c-sdk/driver.h"

#include "helpers.hpp"

extern "C" {

YdbDriverConfig YdbCreateDriverConfig() {
    return {new NYdb::TDriverConfig{}};
}

YdbDriverConfig YdbCreateDriverConfigConnectionString(char* connectionString) {
    return {new NYdb::TDriverConfig{connectionString}};
}

void YdbDestroyDriverConfig(YdbDriverConfig config) {
    delete PTR_FROM_OPAQUE(NYdb::TDriverConfig, config);
}

void YdbDriverConfigSetEndpoint(YdbDriverConfig config, char* endpoint) {
    FROM_OPAQUE(NYdb::TDriverConfig, config).SetEndpoint(endpoint);
}

void YdbDriverConfigSetDatabase(YdbDriverConfig config, char* database) {
    FROM_OPAQUE(NYdb::TDriverConfig, config).SetDatabase(database);
}

void YdbDriverConfigUseSecureConnection(YdbDriverConfig config, char* caCert) {
    FROM_OPAQUE(NYdb::TDriverConfig, config).UseSecureConnection(caCert);
}

YdbDriver YdbCreateDriver(YdbDriverConfig config) {
    return {new NYdb::TDriver{FROM_OPAQUE(NYdb::TDriverConfig, config)}};
}

void YdbStopDriver(YdbDriver driver, bool wait) {
    FROM_OPAQUE(NYdb::TDriver, driver).Stop(wait);
    delete PTR_FROM_OPAQUE(NYdb::TDriver, driver);
}

}
