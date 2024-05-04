#ifndef TOUCHPADVOLUMECONTROL_TOUCHPADLISTENER_HPP
#define TOUCHPADVOLUMECONTROL_TOUCHPADLISTENER_HPP

#include "libevdev-1.0/libevdev/libevdev-uinput.h"
#include "libevdev-1.0/libevdev/libevdev.h"
#include <string>
#include <functional>


class TouchpadListener {

public:
    TouchpadListener();
    ~TouchpadListener();
    void processEvents();

//    std::function<void(int)> onVolumeChange;
//    void setVolumeChangeCallback(const std::function<void(int)>& callback) {
//        onVolumeChange = callback;
//    }

private:
    libevdev* getDevice();

private:
    std::string devicesPath = "/dev/input/event";
    libevdev* device;
};


#endif //TOUCHPADVOLUMECONTROL_TOUCHPADLISTENER_HPP
