cmake_minimum_required(VERSION 3.26)
project(TouchpadVolumeControl)

set(CMAKE_CXX_STANDARD 17)

# Find libevdev package
find_package(PkgConfig REQUIRED)
pkg_search_module(LIBEVDEV REQUIRED libevdev)

add_executable(TouchpadVolumeControl main.cpp)

target_link_libraries(TouchpadVolumeControl ${LIBEVDEV_LIBRARIES})
target_include_directories(TouchpadVolumeControl PUBLIC ${LIBEVDEV_INCLUDE_DIRS})


## Specify the path to libevdev headers
#include_directories(/usr/include/libevdev-1.0)
#
## Link against libevdev library
#target_link_libraries(touchpad PRIVATE evdev)