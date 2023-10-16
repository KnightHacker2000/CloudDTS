
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(CORE_DIR ${SRC_DIR}/core)
set(UTIL_DIR ${SRC_DIR}/util)

include_directories(${SRC_DIR})

file(
        GLOB_RECURSE
        SRC_FILES
        ${CORE_DIR}/*.cpp
        ${UTIL_DIR}/*.cpp
)

add_executable(
        dts
        ${SRC_FILES}
        ${SRC_DIR}/main.cpp
)