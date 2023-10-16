
cmake_minimum_required(VERSION 3.15)
project(dts_contest)

set(CMAKE_CXX_STANDARD 17)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)

set(CMAKE_CXX_FLAGS "-ggdb ${CMAKE_CXX_FLAGS}")

# Dependencies
include(cmake/Dependencies.cmake)

# Build
include(cmake/Build.cmake)