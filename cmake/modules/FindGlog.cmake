# - Try to find Glog
#
# The following variables are optionally searched for defaults
#  GLOG_ROOT_DIR:            Base directory where all GLOG components are found
#
# The following are set after configuration is done:
#  GLOG_FOUND
#  GLOG_INCLUDE_DIR
#  GLOG_LIBRARY_PATH
#  GLOG_LIBRARIES

include(FindPackageHandleStandardArgs)

set(GLOG_ROOT_DIR "" CACHE PATH "Folder contains Google glog")

if(WIN32)
    find_path(GLOG_INCLUDE_DIR glog/logging.h
            PATHS ${GLOG_ROOT_DIR}/src/windows ${CMAKE_INSTALL_PREFIX}/include $ENV{GLOG_HOME}/include)
else()
    find_path(GLOG_INCLUDE_DIR glog/logging.h
            PATHS ${GLOG_ROOT_DIR} ${CMAKE_INSTALL_PREFIX}/include /usr/include $ENV{GLOG_HOME}/include)
endif()

if(MSVC)
    find_library(GLOG_LIBRARY_RELEASE libglog_static
            PATHS ${GLOG_ROOT_DIR} ${CMAKE_INSTALL_PREFIX}/lib $ENV{GLOG_HOME}/lib
            PATH_SUFFIXES Release)

    find_library(GLOG_LIBRARY_DEBUG libglog_static
            PATHS ${GLOG_ROOT_DIR} ${CMAKE_INSTALL_PREFIX}/lib $ENV{GLOG_HOME}/lib
            PATH_SUFFIXES Debug)

    set(GLOG_LIBRARY_PATH optimized ${GLOG_LIBRARY_RELEASE} debug ${GLOG_LIBRARY_DEBUG})
else()
    find_library(GLOG_LIBRARIES glog
            PATHS ${GLOG_ROOT_DIR} ${CMAKE_INSTALL_PREFIX} $ENV{GLOG_HOME}/lib /usr/lib /usr/lib64
            PATH_SUFFIXES lib lib64)
endif()

find_package_handle_standard_args(Glog DEFAULT_MSG GLOG_INCLUDE_DIR GLOG_LIBRARIES)

if(GLOG_FOUND)
    mark_as_advanced(GLOG_ROOT_DIR GLOG_LIBRARY_RELEASE GLOG_LIBRARY_DEBUG
            GLOG_LIBRARIES GLOG_INCLUDE_DIR)
endif()
