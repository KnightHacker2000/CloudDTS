# - Try to find GFLAGS
#
# The following variables are optionally searched for defaults
#  GFLAGS_ROOT_DIR:            Base directory where all GFLAGS components are found
#
# The following are set after configuration is done:
#  GFLAGS_FOUND
#  GFLAGS_INCLUDE_DIR
#  GFLAGS_LIBRARY_PATH
#  GFLAGS_LIBRARIES

include(FindPackageHandleStandardArgs)

set(GFLAGS_ROOT_DIR "" CACHE PATH "Folder contains Gflags")

# We are testing only a couple of files in the include directories
if(WIN32)
    find_path(GFLAGS_INCLUDE_DIR gflags/gflags.h
            PATHS ${GFLAGS_ROOT_DIR}/src/windows ${CMAKE_INSTALL_PREFIX}/include $ENV{GFLAGS_HOME}/include)
else()
    find_path(GFLAGS_INCLUDE_DIR gflags/gflags.h
            PATHS ${GFLAGS_ROOT_DIR} ${CMAKE_INSTALL_PREFIX}/include /usr/include $ENV{GFLAGS_HOME}/include)
endif()

if(MSVC)
    find_library(GFLAGS_LIBRARY_RELEASE
            NAMES libgflags
            PATHS ${GFLAGS_ROOT_DIR} ${CMAKE_INSTALL_PREFIX}/lib $ENV{GFLAGS_HOME}/lib
            PATH_SUFFIXES Release)

    find_library(GFLAGS_LIBRARY_DEBUG
            NAMES libgflags-debug
            PATHS ${GFLAGS_ROOT_DIR} ${CMAKE_INSTALL_PREFIX}/lib $ENV{GFLAGS_HOME}/lib
            PATH_SUFFIXES Debug)

    set(GFLAGS_LIBRARY_PATH optimized ${GFLAGS_LIBRARY_RELEASE} debug ${GFLAGS_LIBRARY_DEBUG})
else()
    find_library(GFLAGS_LIBRARIES gflags
            PATHS ${CMAKE_INSTALL_PREFIX} $ENV{GFLAGS_HOME}/lib /usr/lib /usr/lib64
            PATH_SUFFIXES lib lib64)
endif()

find_package_handle_standard_args(Gflags DEFAULT_MSG GFLAGS_INCLUDE_DIR GFLAGS_LIBRARIES)

if(GFLAGS_FOUND)
    mark_as_advanced(GFLAGS_LIBRARY_DEBUG GFLAGS_LIBRARY_RELEASE
            GFLAGS_LIBRARIES GFLAGS_INCLUDE_DIR GFLAGS_ROOT_DIR)
endif()
