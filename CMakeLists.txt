cmake_minimum_required(VERSION 3.20)
project(petal VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
  set(PETAL_IS_TOP_LEVEL ON)
else()
  set(PETAL_IS_TOP_LEVEL OFF)
endif()

option(PETAL_BUILD_EXAMPLES "Build petal examples" ${PETAL_IS_TOP_LEVEL})

add_library(petal src/petal.cpp)
add_library(petal::petal ALIAS petal)

target_include_directories(petal
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
target_compile_features(petal PUBLIC cxx_std_23)

if(PETAL_BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()
