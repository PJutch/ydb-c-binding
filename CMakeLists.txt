cmake_minimum_required(VERSION 3.5)

project(YDB-C-SDK VERSION 0.01 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED On)

add_library(ydb-c-sdk wrapper.cpp)

set(YDB_DEPS_PATH "~/ydb_deps" CACHE STRING 
"Path to where built from source YDB dependencies are installed")

list(APPEND CMAKE_PREFIX_PATH ${YDB_DEPS_PATH})

find_package(YDB-CPP-SDK REQUIRED)

target_link_libraries(ydb-c-sdk PUBLIC
  yutil
  YDB-CPP-SDK::Query
  YDB-CPP-SDK::Params
  YDB-CPP-SDK::Driver
)

find_package(re2 REQUIRED)
target_link_libraries(ydb-c-sdk PRIVATE ${RE2_LIBRARIES} re2::re2)

# if (CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" OR CMAKE_SYSTEM_PROCESSOR STREQUAL "AMD64")
#   target_link_libraries(basic_example PUBLIC
#     cpuid_check
#   )
# endif()

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
  target_link_options(ydb-c-sdk PRIVATE
    -ldl
    -lrt
    -Wl,--no-as-needed
    -lpthread
  )
elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  target_link_options(ydb-c-sdk PRIVATE
    -Wl,-platform_version,macos,11.0,11.0
    -framework
    CoreFoundation
  )
endif()
