# YDB C Binding
## Сборка
1. Compile [ydb-cpp-sdk](https://github.com/ydb-platform/ydb-cpp-sdk) 
   following their instruction
   adding `-DCMAKE_POSITION_INDEPENDENT_CODE=ON`
   to it and dependencies built from source
2. ```bash
   cmake --install . --prefix ~/ydb_deps/ydb-cpp-sdk
   ```
3. ```bash
   mkdir build
   cd build
   cmake .. -DCMAKE_PREFIX_PATH="~/ydb_deps/protobuf;~/ydb_deps/absl;~/ydb_deps/ydb_cpp_sdk;~/ydb_deps/grpc;~/ydb_deps/jwt-cpp"
   cmake --build .
   cmake --install . --prefix ~/ydb_deps/ydb-c-sdk
   ```
4. ```bash
   cd ../example/c
   mkdir build
   cd build
   cmake .. -DCMAKE_PREFIX_PATH=~/ydb_deps/ydb-c-sdk 
   cmake --build .
   ./example
   ```
5. ```bash
   cd ../../php
   mkdir build
   cd build
   cmake ..
   cd ..
   php main.php
   ```
