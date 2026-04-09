#include "driver.hpp"

#include <ydb-cpp-sdk/client/query/query.h>
#include <ydb-cpp-sdk/client/query/client.h>

extern "C" {

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
