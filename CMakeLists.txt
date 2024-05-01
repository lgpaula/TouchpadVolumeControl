cmake_minimum_required(VERSION 3.26)
project(TouchpadVolumeControl)

set(CMAKE_CXX_STANDARD 17)

# Find libevdev package
find_package(PkgConfig REQUIRED)
pkg_search_module(LIBEVDEV REQUIRED libevdev)
find_package(ALSA REQUIRED)

file(GLOB SOURCES "Audio/*.cpp")
add_executable(TouchpadVolumeControl main.cpp ${SOURCES})

if (ALSA_FOUND)
    include_directories(${ALSA_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${ALSA_LIBRARIES})
endif()

target_link_libraries(${PROJECT_NAME} ${LIBEVDEV_LIBRARIES})
target_include_directories(${PROJECT_NAME} PUBLIC ${LIBEVDEV_INCLUDE_DIRS})