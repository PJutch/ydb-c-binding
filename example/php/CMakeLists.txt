cmake_minimum_required(VERSION 3.5)

project(ydb-php-sdk VERSION 0.01 LANGUAGES C)

# generates ffi data in configuration time

find_package(YDB-C-SDK REQUIRED)

get_target_property(YDB_C_SDK_SO_LOCATION ydb-c-sdk::ydb-c-sdk LOCATION)
message("-- ydb-c-sdk .so location: ${YDB_C_SDK_SO_LOCATION}")
get_target_property(_include_directories ydb-c-sdk::ydb-c-sdk INTERFACE_INCLUDE_DIRECTORIES)
message("-- ydb-c-sdk includes: ${_include_directories}")

foreach(_include_directory ${_include_directories})
    set(YDB_C_SDK_MAIN_HEADER ${_include_directory}/ydb-c-sdk.h)
    if (EXISTS ${YDB_C_SDK_MAIN_HEADER})
        break()
    endif()
endforeach()
message("-- ydb-c-sdk main header: ${YDB_C_SDK_MAIN_HEADER}")

set(YDB_PHP_SDK_FFI_HEADER ${CMAKE_CURRENT_BINARY_DIR}/ydb-c-sdk.i)

execute_process(COMMAND ${CMAKE_C_COMPILER} -E ${YDB_C_SDK_MAIN_HEADER} -o ${YDB_PHP_SDK_FFI_HEADER})

file(READ ${YDB_PHP_SDK_FFI_HEADER} _preprocessed_contents)
file(WRITE ${YDB_PHP_SDK_FFI_HEADER} "#define FFI_LIB \"${YDB_C_SDK_SO_LOCATION}\"\n\nvoid free(void*);\n\n${_preprocessed_contents}\n")
