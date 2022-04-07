cmake_minimum_required(VERSION 3.10)

project(btdef VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(btdef INTERFACE) 

target_include_directories(
    btdef INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# i don't know for what
# add_library(btdef::btdef ALIAS btdef)

option(BTDEF_BUILD_EXAMPLES "build examples" OFF)

if (BTDEF_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# Installation
# ------------
include(GNUInstallDirs)

# Install the library
install(
    TARGETS btdef
    EXPORT btdefTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# Install the headers
install(
    DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

include(CPack)
