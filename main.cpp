#include <iostream>
#include "libevdev-1.0/libevdev/libevdev-uinput.h"
#include "libevdev-1.0/libevdev/libevdev.h"
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <queue>
#include <numeric>
#include <algorithm>

struct libevdev* find_device_by_name(const std::string& requested_name) {
    struct libevdev *dev = nullptr;

    std::string path = "/dev/input/event9";
    int fd = open(path.c_str(), O_RDWR|O_CLOEXEC);
    if (fd == -1) return nullptr;

    if (libevdev_new_from_fd(fd, &dev) == 0) {
        std::string name = libevdev_get_name(dev);
        if (name == requested_name) return dev;
        libevdev_free(dev);
        dev = nullptr;
    }

    close(fd);
    return nullptr;
}

int get_y_average(const std::deque<int>& y_values) {
    for (auto y : y_values) {
        std::cout << y << " ";
    }
    return std::accumulate(y_values.begin(), y_values.end(), 0) / 3;
}

//todo account for 4 finger touch
void process_events_touchpad(struct libevdev *dev) {

    struct input_event ev = {};
    int status = 0;
    auto is_error = [](int v) { return v < 0 && v != -EAGAIN; };
    auto has_next_event = [](int v) { return v >= 0; };
    const auto flags = LIBEVDEV_READ_FLAG_NORMAL | LIBEVDEV_READ_FLAG_BLOCKING;

    std::deque<int> y_values;
    int average_y = 0;
    bool tripleTouching = false;
    int previous_average = 0;
    int volume = 50;
    // y_min 0, y_max 571

    while (status = libevdev_next_event(dev, flags, &ev), !is_error(status)) {
        if (!has_next_event(status)) continue;

        // come up with an equation in which the faster the average increases, the more volume increases.

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

            average_y = get_y_average(y_values);
            std::cout << " --> average: " << average_y;

            if (!previous_average) {
                previous_average = average_y;
                continue;
            }

            auto diff = average_y - previous_average;
            if (std::abs(diff) >= 10) {
                std::cout << std::endl;
                continue;
            }
            std::cout << " --> diff: " << diff;
            volume -= diff / 2;
            volume = std::clamp(volume, 0, 100);
            std::cout << " --> volume: " << volume << std::endl;

            previous_average = average_y;

//            std::cout << "y pos: ";
//            std::cout << ev.value << std::endl;
        }
        BTN_LEFT;

        //code 272 left click type1
        //code 273 right click type1

    }
}

void getDevices() {
    struct libevdev *dev = nullptr;

    for (int i = 0;; i++) {
        std::string path = "/dev/input/event" + std::to_string(i);
        int fd = open(path.c_str(), O_RDWR|O_CLOEXEC);
        if (fd == -1) {
            continue; // no more character devices
        }
        std::cout << i << ", " << fd << std::endl;
        if (libevdev_new_from_fd(fd, &dev) == 0) {
            if (libevdev_get_phys(dev) == nullptr) continue;
            std::string phys = libevdev_get_phys(dev);
            std::string name = libevdev_get_name(dev);

            std::cout << path << std::endl;
            std::cout << "- phys: " << phys << std::endl;
            std::cout << "- name: " << name << std::endl;
            std::cout << std::endl;

            libevdev_free(dev);
            dev = nullptr;
        }
        close(fd);
        if (i > 20) break;
    }
}

int main() {
//    getDevices();
    struct libevdev *dev = find_device_by_name("SYNA2B46:00 06CB:CD5F Touchpad");

    if (dev == nullptr) {
        std::cerr << "Couldn't find device!" << std::endl;
        return -1;
    }

    libevdev_grab(dev, LIBEVDEV_GRAB);

    process_events_touchpad(dev);

    libevdev_free(dev);
    return 0;
}


/* single top left
Got input_event type=3 code=47 value=0          multi-touch slot being modified
Got input_event type=3 code=57 value=193        track id
Got input_event type=3 code=53 value=44         x pos
Got input_event type=3 code=54 value=31         y pos
Got input_event type=1 code=330 value=1         single touch
Got input_event type=3 code=57 value=-1         let go
Got input_event type=1 code=330 value=0         let go
*/

/* single top right
Got input_event type=3 code=57 value=194
Got input_event type=3 code=53 value=1133
Got input_event type=3 code=54 value=18
Got input_event type=1 code=330 value=1
Got input_event type=3 code=0 value=1133        auto-repeat?
Got input_event type=3 code=1 value=18          auto-repeat?
Got input_event type=3 code=57 value=-1
Got input_event type=1 code=330 value=0
*/

/* double touch:
Got input_event type=3 code=47 value=0          multi-touch slot being modified
Got input_event type=3 code=57 value=197
Got input_event type=3 code=53 value=296
Got input_event type=3 code=54 value=339
Got input_event type=1 code=330 value=1
Got input_event type=1 code=325 value=1
Got input_event type=4 code=5 value=0           axes
Got input_event type=3 code=47 value=1          multi-touch slot being modified
Got input_event type=3 code=57 value=198
Got input_event type=3 code=53 value=743
Got input_event type=3 code=54 value=360
Got input_event type=1 code=325 value=0
Got input_event type=1 code=333 value=1         double tap
Got input_event type=4 code=5 value=7200
Got input_event type=0 code=0 value=0
Got input_event type=4 code=5 value=14400
Got input_event type=0 code=0 value=0
Got input_event type=4 code=5 value=21600
Got input_event type=0 code=0 value=0
Got input_event type=4 code=5 value=28800
Got input_event type=0 code=0 value=0
Got input_event type=3 code=47 value=0          multi-touch slot being modified
Got input_event type=3 code=53 value=290
Got input_event type=3 code=54 value=351
Got input_event type=3 code=47 value=1
Got input_event type=3 code=57 value=-1
Got input_event type=1 code=325 value=1
Got input_event type=1 code=333 value=0         double tap
Got input_event type=3 code=0 value=290
Got input_event type=3 code=1 value=351
Got input_event type=4 code=5 value=36000
Got input_event type=0 code=0 value=0
Got input_event type=3 code=47 value=0          multi-touch slot being modified
Got input_event type=3 code=57 value=-1
Got input_event type=1 code=330 value=0
Got input_event type=1 code=325 value=0
Got input_event type=4 code=5 value=43200
Got input_event type=0 code=0 value=0
*/