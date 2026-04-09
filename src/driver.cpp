#include "driver.hpp"

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

}
