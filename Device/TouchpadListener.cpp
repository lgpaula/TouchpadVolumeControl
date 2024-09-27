#include "TouchpadListener.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <queue>
#include <algorithm>
#include <utility>
#include <filesystem>

TouchpadListener::TouchpadListener(TouchpadListener::OnVolumeChange onVolumeChange, TouchpadListener::OnMute onMute) :
        onVolumeChange(std::move(onVolumeChange)), onMute(std::move(onMute)), device(getDevice()) {
    if (device == nullptr) {
        throw std::runtime_error("Couldn't find device!");
    }

    processEvents();
}

void TouchpadListener::countDevices() {
    using namespace std::filesystem;
    for (const auto& entry : directory_iterator(devicesPath)) {
        std::string path = entry.path().filename().string();
        if (path.find("event") == 0) ++deviceCount;
    }
}

libevdev *TouchpadListener::getDevice() {
    countDevices();
    libevdev *dev = nullptr;

    for (size_t i = 0; i < deviceCount; ++i) {
        std::string path = devicesPath + deviceName + std::to_string(i);
        int fd = open(path.c_str(), O_RDWR | O_CLOEXEC);

        if (fd == -1) break;

        if (libevdev_new_from_fd(fd, &dev) == 0) {
            if (libevdev_get_phys(dev) == nullptr) continue;
            std::string phys = libevdev_get_phys(dev);
            std::string name = libevdev_get_name(dev);

            if (name.find("Touchpad") != std::string::npos) return dev;

            libevdev_free(dev);
            dev = nullptr;
        }
        close(fd);
    }

    return nullptr;
}

void TouchpadListener::processEvents() {

    input_event ev = {};
    int status = 0;
    const auto is_error = [](int v) { return v < 0 && v != -EAGAIN; };
    const auto has_next_event = [](int v) { return v >= 0; };
    const auto flags = LIBEVDEV_READ_FLAG_NORMAL | LIBEVDEV_READ_FLAG_BLOCKING;

    bool tripleTouching = false;

    std::chrono::steady_clock::time_point tripleTapTime;
    bool pendingMute = false;
    const int tripleTapReleaseTimeout = 200;

    while (status = libevdev_next_event(device, flags, &ev), !is_error(status)) {
        if (!has_next_event(status)) continue;

        switch (ev.code) {
            case ABS_MT_SLOT:
                currFinger = ev.value;
                break;

            case BTN_TOOL_TRIPLETAP:
                tripleTouching = ev.value;

                if (ev.value == 1) {
                    pendingMute = true;
                    tripleTapTime = std::chrono::steady_clock::now();
                } else if (ev.value == 0 && pendingMute) {
                    auto now = std::chrono::steady_clock::now();
                    if (std::chrono::duration_cast<std::chrono::milliseconds>(now - tripleTapTime).count() <= tripleTapReleaseTimeout) {
                        onMute();
                    }

                    pendingMute = false;
                }
                break;

            case ABS_MT_TRACKING_ID: {
                if (currFinger == -1) break;

                Finger finger;
                if (ev.value != -1) finger.id = currFinger;

                fingers[currFinger] = finger;
                break;
            }

            case ABS_MT_POSITION_Y: {
                if (currFinger == -1 || !tripleTouching) break;
                fingers[currFinger].normalizeY(ev.value);
                int volume = updateVolume(fingers[currFinger], ev.value);
                onVolumeChange(-volume);
                break;
            }

            default:
                break;
        }
    }
}

TouchpadListener::~TouchpadListener() noexcept {
    libevdev_free(device);
}

int TouchpadListener::updateVolume(Finger& finger, int y) {
    const auto diff = y - finger.currentY;

    if (std::abs(diff) < 15) return 0;

    finger.currentY = y;
    return diff / 15;
}

void Finger::normalizeY(int y) {
    if (currentY == std::numeric_limits<int>::max()) currentY = y;
}