project(binance_cpp CXX)

cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_C_COMPILER "gcc")
SET(CMAKE_C_FLAGS_DEBUG "-g")

find_package(GFLAGS REQUIRED)
find_package(GLOG REQUIRED)
find_package(ZLIB REQUIRED)
find_package(CURL REQUIRED)
find_package(LIBWEBSOCKETS REQUIRED)
find_package(OpenSSL REQUIRED)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_subdirectory(binance)
add_subdirectory(example)
add_subdirectory(test)

message("end build")