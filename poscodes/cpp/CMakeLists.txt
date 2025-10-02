cmake_minimum_required(VERSION 3.10)
project(poscode
VERSION 1.0
LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()
add_compile_options(-O3 -DNDEBUG)

add_executable(test test.cpp src/utils.cpp src/poscode.cpp)
target_include_directories(test PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_executable(bench bench.cpp src/utils.cpp src/poscode.cpp)
target_include_directories(bench PUBLIC ${PROJECT_SOURCE_DIR}/include)