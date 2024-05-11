#ifndef TOUCHPADVOLUMECONTROL_TOUCHPADLISTENER_HPP
#define TOUCHPADVOLUMECONTROL_TOUCHPADLISTENER_HPP

#include "libevdev-1.0/libevdev/libevdev.h"
#include <string>
#include <functional>
#include <limits>

struct Finger {
    int id = -1;
    int currentY = std::numeric_limits<int>::max();

    void updateY(int y);
};

class TouchpadListener {

public:
    using OnVolumeChange = std::function<void(int volume)>;

    explicit TouchpadListener(OnVolumeChange onVolumeChange);
    ~TouchpadListener() noexcept;
    void processEvents();

private:
    libevdev* getDevice();
    static int updateVolume(Finger finger, int y);
    OnVolumeChange onVolumeChange;

private:
    std::string devicesPath = "/dev/input/event";
    libevdev *device;
    std::array<Finger, 3> fingers;
    int currFinger = -1;
};


#endif //TOUCHPADVOLUMECONTROL_TOUCHPADLISTENER_HPP
