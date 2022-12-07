cmake_minimum_required(VERSION 3.10)

project(btdefexample VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(e1 e1.cpp)
target_link_libraries(e1 btdef)

add_executable(main6 main6.cpp)
target_link_libraries(main6 btdef)
