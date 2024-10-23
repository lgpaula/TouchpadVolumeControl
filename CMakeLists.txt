cmake_minimum_required(VERSION 3.26)
project(TouchpadVolumeControl)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(Audio)
add_subdirectory(Device)

file(COPY icons DESTINATION ${CMAKE_BINARY_DIR})

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE AudioLib DeviceLib)