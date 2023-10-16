# Glog
find_package(Glog REQUIRED)
include_directories(${GLOG_INCLUDE_DIR})
link_libraries(${GLOG_LIBRARIES})

# Gflags
find_package(Gflags REQUIRED)
include_directories(${GFLAGS_INCLUDE_DIR})
link_libraries(${GFLAGS_LIBRARIES})

# pthread
link_libraries(
        pthread
)
