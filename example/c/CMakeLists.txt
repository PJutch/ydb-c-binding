cmake_minimum_required(VERSION 3.5)

project(YDB-C-SDK-EXAMPLE VERSION 0.01 LANGUAGES C)

add_executable(example main.c data.c)

find_package(YDB-C-SDK REQUIRED)

get_target_property(_ydb_c_sdk_so_location ydb-c-sdk::ydb-c-sdk LOCATION)
message("-- ydb-c-sdk .so location: ${_ydb_c_sdk_so_location}")

target_link_libraries(example PRIVATE ydb-c-sdk::ydb-c-sdk)
