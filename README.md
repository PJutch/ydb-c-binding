# YDB C Binding
## Сборка
1. Compile [ydb-cpp-sdk](https://github.com/ydb-platform/ydb-cpp-sdk) 
   adding `-DCMAKE_POSITION_INDEPENDENT_CODE=ON`
   to dependencies built from source
2. ```bash
   cmake --install . --prefix ~/ydb_deps/ydb-cpp-sdk
   ```
3. ```bash
   mkdir build
   cd build
   cmake .. -DYDB_DEPS_PATH=~/ydb_deps
   cmake --build .
   cmake --install . 
   ```
4. ```bash
   cd ../example/c
   mkdir build
   cd build
   cmake ..
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
