#include "TouchpadListener.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <queue>
#include <numeric>
#include <algorithm>
#include <utility>

TouchpadListener::TouchpadListener(TouchpadListener::OnVolumeChange onVolumeChange) :
    onVolumeChange(std::move(onVolumeChange)), device(getDevice()) {
    if (device == nullptr) {
        std::cerr << "Couldn't find device!" << std::endl;
        return;
    }

    processEvents();
}

libevdev* TouchpadListener::getDevice() {
    libevdev *dev = nullptr;

    for (int i = 0;; i++) {
        std::string path = devicesPath + std::to_string(i);
        int fd = open(path.c_str(), O_RDWR|O_CLOEXEC);

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
        if (i > 20) break;
    }

    return nullptr;
}

void TouchpadListener::processEvents() {

    input_event ev = {};
    int status = 0;
    auto is_error = [](int v) { return v < 0 && v != -EAGAIN; };
    auto has_next_event = [](int v) { return v >= 0; };
    const auto flags = LIBEVDEV_READ_FLAG_NORMAL | LIBEVDEV_READ_FLAG_BLOCKING;

    bool tripleTouching = false;

    while (status = libevdev_next_event(device, flags, &ev), !is_error(status)) {
        if (!has_next_event(status)) continue;

        switch (ev.code) {
            case 47: // slot id
                currFinger = ev.value;
                break;

            case 334: // triple touch
                tripleTouching = ev.value;
                break;

            case 57: // touch id
                if (currFinger == -1) break;
                if (ev.value != -1) {
                    Finger finger;
                    finger.id = currFinger;
                    fingers[currFinger] = finger;
                } else {
                    fingers[currFinger].reset();
                }
                break;

            case 54: { // y pos
                if (currFinger == -1 || !tripleTouching) break;

                int volume = fingers[currFinger].updateY(ev.value);
                onVolumeChange(-volume);
                break;
            }

            default:
                break;
        }
    }
}

TouchpadListener::~TouchpadListener() {
    libevdev_free(device);
}

int Finger::updateY(int y) {
    if (id) return 0;
    if (currentY == std::numeric_limits<int>::max()) {
        currentY = y;
        return 0;
    }

    auto diff = y - currentY;
    if (std::abs(diff) >= 5) {
        currentY = y;
        return diff / 5;
    }

    return 0;
}