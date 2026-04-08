#include <ydb-c-sdk.h>

int main() {
    TDriverConfig* config = CreateDriverConfig();
    DriverConfigSetEndpoint(config, "localhost:2136");
    DriverConfigSetDatabase(config, "/Root/Test");
    
    TDriver* driver = CreateDriver(config);
    TQueryClient* client = CreateQueryClient(driver);

    DestroyQueryClient(client);
    StopDriver(driver, true);
    DestroyDriverConfig(config);
}