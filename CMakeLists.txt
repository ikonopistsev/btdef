cmake_minimum_required(VERSION 3.19)

project(btdef VERSION 1.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(btdef INTERFACE include/)

target_include_directories(btdef INTERFACE include/)

option(BTDEF_BUILD_EXAMPLES "build examples" OFF)

if (BTDEF_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
