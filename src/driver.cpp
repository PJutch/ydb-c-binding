#include <ydb-cpp-sdk/client/driver/driver.h>

#include "ydb-c-sdk/driver.h"

#include "helpers.hpp"

extern "C" {

YdbDriverConfig CreateDriverConfig() {
    return {new NYdb::TDriverConfig{}};
}

YdbDriverConfig CreateDriverConfigConnectionString(char* connectionString) {
    return {new NYdb::TDriverConfig{connectionString}};
}

void DestroyDriverConfig(YdbDriverConfig config) {
    delete PTR_FROM_OPAQUE(NYdb::TDriverConfig, config);
}

void DriverConfigSetEndpoint(YdbDriverConfig config, char* endpoint) {
    FROM_OPAQUE(NYdb::TDriverConfig, config).SetEndpoint(endpoint);
}

void DriverConfigSetDatabase(YdbDriverConfig config, char* database) {
    FROM_OPAQUE(NYdb::TDriverConfig, config).SetDatabase(database);
}

void DriverConfigUseSecureConnection(YdbDriverConfig config, char* caCert) {
    FROM_OPAQUE(NYdb::TDriverConfig, config).UseSecureConnection(caCert);
}

YdbDriver CreateDriver(YdbDriverConfig config) {
    return {new NYdb::TDriver{FROM_OPAQUE(NYdb::TDriverConfig, config)}};
}

void StopDriver(YdbDriver driver, bool wait) {
    FROM_OPAQUE(NYdb::TDriver, driver).Stop(wait);
    delete PTR_FROM_OPAQUE(NYdb::TDriver, driver);
}

}
