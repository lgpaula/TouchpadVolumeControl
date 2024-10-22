cmake_minimum_required(VERSION 3.26)
project(TouchpadVolumeControl)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(Audio)
add_subdirectory(Device)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE AudioLib DeviceLib)

find_program(APPIMAGE_TOOL appimagetool)
if (APPIMAGE_TOOL)
    set(CPACK_APPIMAGE_GENERATOR "${APPIMAGE_TOOL}")
    message(STATUS "AppImage generator found: ${APPIMAGE_TOOL}")
else()
    message(WARNING "AppImage generator not found: AppImage package will not be generated.")
endif()

# Install the executable
install(TARGETS ${PROJECT_NAME} DESTINATION bin)

# Install license files
install(DIRECTORY licenses/ DESTINATION share/licenses/${PROJECT_NAME})

# Install documentation
install(FILES README.md DESTINATION share/doc/${PROJECT_NAME})

# Set package metadata
set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_CONTACT "lgpaula <https://github.com/lgpaula>")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Volume control through touchpad input")
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-linux")
set(CPACK_GENERATOR "RPM")
set(CPACK_RPM_PACKAGE_LICENSE "MIT")
set(CPACK_RPM_PACKAGE_REQUIRES "ALSA")
set(CPACK_RPM_PACKAGE_REQUIRES "libevdev")
include(CPack)

# Set AppImage as an additional generator
list(APPEND CPACK_GENERATOR "AppImage")

set(CPACK_APPIMAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}.AppImage")
set(CPACK_APPIMAGE_ICON "${CMAKE_CURRENT_SOURCE_DIR}/icon.png")
include(CPack)