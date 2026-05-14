cmake_minimum_required(VERSION 3.5)

project(YDB-C-SDK-EXAMPLE VERSION 0.01 LANGUAGES C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED On)


add_executable(example main.c data.c)

if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
  target_link_libraries(example PRIVATE stdc++)
elseif(CMAKE_C_COMPILER_ID STREQUAL "Clang")
    target_link_libraries(example PRIVATE c++)
endif()

set(YDB_DEPS_PATH "~/ydb_deps" CACHE STRING 
"Path to where built from source YDB dependencies are installed")
list(APPEND CMAKE_PREFIX_PATH 
  ${YDB_DEPS_PATH}/ydb_cpp_sdk ${YDB_DEPS_PATH}/absl 
  ${YDB_DEPS_PATH}/protobuf ${YDB_DEPS_PATH}/grpc ${YDB_DEPS_PATH}/jwt-cpp)

find_library(RE2_LIBRARY NAMES re2 REQUIRED PATHS /usr/lib/x86_64-linux-gnu)
target_link_libraries(example PRIVATE ${RE2_LIBRARY})

# prevent gRPC from searching for re2 config on Windows filesystem
# set(re2_FOUND YES)
# add_library(re2::re2 UNKNOWN IMPORTED)
# set_target_properties(re2::re2 PROPERTIES IMPORTED_LOCATION ${RE2_LIBRARY})

find_package(YDB-C-SDK REQUIRED)

target_link_libraries(example PRIVATE ydb-c-sdk::ydb-c-sdk)


# if (CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" OR CMAKE_SYSTEM_PROCESSOR STREQUAL "AMD64")
#   target_link_libraries(basic_example PUBLIC
#     cpuid_check
#   )
# endif()

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
  target_link_options(example PRIVATE
    -ldl
    -lrt
    -Wl,--no-as-needed
    -lpthread
  )
elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  target_link_options(example PRIVATE
    -Wl,-platform_version,macos,11.0,11.0
    -framework
    CoreFoundation
  )
endif()
