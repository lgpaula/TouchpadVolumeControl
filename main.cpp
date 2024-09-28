#include "Audio/AudioHandler.hpp"
#include "Device/TouchpadListener.hpp"
#include <iostream>

int main() {

    AudioHandler audioHandler;

    TouchpadListener touchpadListener([&audioHandler](int volume) {
        if (volume) {
            audioHandler.increaseVolume(volume);
        }
    });

    return 0;
}