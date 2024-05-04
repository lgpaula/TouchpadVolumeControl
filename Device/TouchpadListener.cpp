#include "TouchpadListener.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <queue>
#include <numeric>
#include <algorithm>


TouchpadListener::TouchpadListener() : device(getDevice()) {
    if (device == nullptr) {
        std::cerr << "Couldn't find device!" << std::endl;
        return;
    }

//    processEvents(device);
}

libevdev* TouchpadListener::find_device_by_name(const std::string& requested_name, int event) {
    libevdev *dev = nullptr;

    std::string path = devicePath + std::to_string(event);
    int fd = open(path.c_str(), O_RDWR|O_CLOEXEC);
    if (fd == -1) return nullptr;

    if (libevdev_new_from_fd(fd, &dev) == 0) {
        std::string name = libevdev_get_name(dev);
        if (name == requested_name) return dev;
        libevdev_free(dev);
        dev = nullptr;
        return dev;
    }

    close(fd);
    return nullptr;
}

libevdev* TouchpadListener::getDevice() {
    libevdev *dev = nullptr;

    for (int i = 0;; i++) {
        std::string path = devicePath + std::to_string(i);
        int fd = open(path.c_str(), O_RDWR|O_CLOEXEC);

        if (fd == -1) break;

        if (libevdev_new_from_fd(fd, &dev) == 0) {
            if (libevdev_get_phys(dev) == nullptr) continue;
            std::string phys = libevdev_get_phys(dev);
            std::string name = libevdev_get_name(dev);

            libevdev_free(dev);
            dev = nullptr;

            if (name.find("Touchpad") != std::string::npos) return find_device_by_name(name, i);
        }
        close(fd);
        if (i > 20) break;
    }

    return nullptr;
}

// todo keep track of each individual touch and average each last value
void TouchpadListener::processEvents() {

    input_event ev = {};
    int status = 0;
    auto is_error = [](int v) { return v < 0 && v != -EAGAIN; };
    auto has_next_event = [](int v) { return v >= 0; };
    const auto flags = LIBEVDEV_READ_FLAG_NORMAL | LIBEVDEV_READ_FLAG_BLOCKING;

    std::deque<int> y_values;
    int average_y = 0;
    bool tripleTouching = false;
    int previous_average = 0;
    int volume = 50;

    while (status = libevdev_next_event(device, flags, &ev), !is_error(status)) {
        if (!has_next_event(status)) continue;

        if (ev.type == 1 && ev.code == 334) {
            std::cout << "Got triple touch" << std::endl;
//            std::cout << " type=" << ev.type;
//            std::cout << " code=" << ev.code;
            std::cout << " value=" << (ev.value == 1 ? " on" : " off") << std::endl;
            tripleTouching = ev.value;
        }
//        if (ev.type == 3 && ev.code == 53) {
//            std::cout << "x pos: ";
//            std::cout << ev.value << std::endl;
//        }

        if (!tripleTouching) {
            volume = 50;
            previous_average = 0;
        }

        if (ev.type == 3 && ev.code == 54 && tripleTouching) {
            y_values.push_back(ev.value);

            if (y_values.size() > 3) y_values.pop_front();
            else continue;

            for (auto y : y_values) {
                std::cout << y << " ";
            }
            average_y = std::accumulate(y_values.begin(), y_values.end(), 0) / 3;
            std::cout << " --> average: " << average_y;

            if (!previous_average) {
                previous_average = average_y;
                continue;
            }

            auto diff = average_y - previous_average;
            diff = std::clamp(diff, -2, 2);
            std::cout << " --> " << (diff > 0 ? "up" : diff < 0 ? "down" : "eq");
            volume -= diff;
            volume = std::clamp(volume, 0, 100);
            std::cout << " --> volume: " << volume << std::endl;
            onVolumeChange(volume);

            previous_average = average_y;
        }
    }
}

TouchpadListener::~TouchpadListener() {
    libevdev_free(device);
}
