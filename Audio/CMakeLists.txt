cmake_minimum_required(VERSION 3.18)
project(AudioLib)

set(CMAKE_CXX_STANDARD 17)

find_package(ALSA REQUIRED)

set(AUDIO_SOURCE_FILES AudioHandler.cpp)

include_directories(${ALSA_INCLUDE_DIRS})

add_library(${PROJECT_NAME} ${AUDIO_SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} ${ALSA_LIBRARIES})
