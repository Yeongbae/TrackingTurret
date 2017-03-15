cmake_minimum_required (VERSION 2.0)
project (TrackingTurret)
find_package (OpenCV REQUIRED)

set (TrackingTurret_VERSION_MAJOR 0)
set (TrackingTurret_VERSION_MINOR 1)

configure_file (
  "src/TrackingTurretConfig.h.in"
  "include/TrackingTurretConfig.h"
)

include_directories("include")

add_executable(TrackingTurret src/main.cpp)
target_link_libraries(TrackingTurret ${OpenCV_LIBS})
