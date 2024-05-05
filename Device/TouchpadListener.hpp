#ifndef TOUCHPADVOLUMECONTROL_TOUCHPADLISTENER_HPP
#define TOUCHPADVOLUMECONTROL_TOUCHPADLISTENER_HPP

#include "libevdev-1.0/libevdev/libevdev-uinput.h"
#include "libevdev-1.0/libevdev/libevdev.h"
#include <string>
#include <functional>


class TouchpadListener {

public:
    using OnVolumeChange = std::function<void(int volume)>;

    explicit TouchpadListener(OnVolumeChange onVolumeChange);
    virtual ~TouchpadListener();
    void processEvents();

private:
    libevdev* getDevice();
    OnVolumeChange onVolumeChange;

private:
    std::string devicesPath = "/dev/input/event";
    libevdev* device;
};


#endif //TOUCHPADVOLUMECONTROL_TOUCHPADLISTENER_HPP
