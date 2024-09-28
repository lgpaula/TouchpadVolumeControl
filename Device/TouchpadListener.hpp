#ifndef TOUCHPADVOLUMECONTROL_TOUCHPADLISTENER_HPP
#define TOUCHPADVOLUMECONTROL_TOUCHPADLISTENER_HPP

#include "libevdev-1.0/libevdev/libevdev.h"
#include <string>
#include <functional>
#include <limits>

struct Finger {
    int id = -1;
    int currentY = std::numeric_limits<int>::max();

    void normalizeY(int y);
};

class TouchpadListener {

public:
    using OnVolumeChange = std::function<void(int volume)>;
    using OnMute = std::function<void()>;

    explicit TouchpadListener(OnVolumeChange onVolumeChange, OnMute onMute);
    ~TouchpadListener() noexcept;
    void processEvents();

private:
    libevdev* getDevice();
    void countDevices();
    static int updateVolume(Finger& finger, int y);

private:
    OnVolumeChange onVolumeChange;
    OnMute onMute;
    const std::string devicesPath = "/dev/input/";
    const std::string deviceName = "event";
    libevdev *device;
    std::array<Finger, 3> fingers;
    int currFinger = -1;
    int deviceCount{};
};


#endif //TOUCHPADVOLUMECONTROL_TOUCHPADLISTENER_HPP
