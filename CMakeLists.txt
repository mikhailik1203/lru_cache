cmake_minimum_required(VERSION 3.23)
project(CacheData)

set(CMAKE_CXX_STANDARD 17)
add_compile_definitions(__GCC__)

add_executable(CacheData ./src/main.cpp)

add_executable(CacheDataTest ./test/test.cpp ./test/testCacheCont.cpp ./test/testHashVector.cpp)

add_executable(CacheDataBenchmark ./benchmarks/bench_test.cpp)

target_link_libraries(CacheDataTest gtest pthread)

