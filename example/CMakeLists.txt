cmake_minimum_required(VERSION 3.5)

project(YDB-C-SDK-EXAMPLE VERSION 0.01 LANGUAGES C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED On)

set(YDB_DEPS_PATH "~/ydb_deps" CACHE STRING 
"Path to where built from source YDB dependencies are installed")
list(APPEND CMAKE_PREFIX_PATH ${YDB_DEPS_PATH})

add_executable(example main.c)

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
