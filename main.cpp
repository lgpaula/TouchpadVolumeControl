#include "Audio/AudioHandler.hpp"
#include "Device/TouchpadListener.hpp"

int main() {

    AudioHandler audioHandler;

    TouchpadListener touchpadListener([&audioHandler](int volume) {
        if (volume) {
            audioHandler.increaseVolume(volume);
        }
    }, [&audioHandler]() {
        audioHandler.toggleMute();
    });

    return 0;
}