#ifndef TOUCHPADVOLUMECONTROL_TOUCHPADLISTENER_HPP
#define TOUCHPADVOLUMECONTROL_TOUCHPADLISTENER_HPP

#include "libevdev-1.0/libevdev/libevdev-uinput.h"
#include "libevdev-1.0/libevdev/libevdev.h"
#include <string>

class TouchpadListener {

public:
    TouchpadListener();
    ~TouchpadListener();

private:
    libevdev* getDevice();
    libevdev* find_device_by_name(const std::string& requested_name, int event);
    void processEvents(libevdev *dev);

private:
    std::string devicePath = "/dev/input/event";
    libevdev* device;
};


#endif //TOUCHPADVOLUMECONTROL_TOUCHPADLISTENER_HPP
