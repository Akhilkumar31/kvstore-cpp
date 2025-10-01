
cmake_minimum_required(VERSION 3.16)
project(kvstore_cpp)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -O2 -Wall -Wextra")
add_executable(kvstore
    src/main.cpp
    src/server.cpp
    src/store.cpp
    src/persistence.cpp
    src/util.cpp
)
add_executable(kvclient src/client.cpp src/util.cpp)
target_include_directories(kvstore PRIVATE include)
target_include_directories(kvclient PRIVATE include)
