#ifndef TOUCHPADVOLUMECONTROL_TOUCHPADLISTENER_HPP
#define TOUCHPADVOLUMECONTROL_TOUCHPADLISTENER_HPP

#include "libevdev-1.0/libevdev/libevdev-uinput.h"
#include "libevdev-1.0/libevdev/libevdev.h"
#include <string>
#include <functional>

std::function<void(int)> onVolumeChange;

class TouchpadListener {

public:
    TouchpadListener();
    ~TouchpadListener();
    void processEvents();

    void setVolumeChangeCallback(const std::function<void(int)>& callback) {
        onVolumeChange = callback;
    }

private:
    libevdev* getDevice();
    libevdev* find_device_by_name(const std::string& requested_name, int event);

private:
    std::string devicePath = "/dev/input/event";
    libevdev* device;
};


#endif //TOUCHPADVOLUMECONTROL_TOUCHPADLISTENER_HPP
