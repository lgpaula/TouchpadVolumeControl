cmake_minimum_required(VERSION 3.18)
project(DeviceLib)

set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig REQUIRED)
pkg_search_module(LIBEVDEV REQUIRED libevdev)

set(DEVICE_SOURCE_FILES TouchpadListener.cpp)

add_library(${PROJECT_NAME} ${DEVICE_SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} ${LIBEVDEV_LIBRARIES})
target_include_directories(${PROJECT_NAME} PUBLIC ${LIBEVDEV_INCLUDE_DIRS})