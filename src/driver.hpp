#ifndef YDB_C_SDK_DRIVER_IMPL_H_
#define YDB_C_SDL_DRIVER_IMPL_H_

#include "ydb-c-sdk/driver.h"
#include <ydb-cpp-sdk/client/driver/driver.h>

extern "C" {

struct TDriver {
    NYdb::TDriver driver;
};

}

#endif