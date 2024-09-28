#ifndef TOUCHPADVOLUMECONTROL_AUDIOHANDLER_HPP
#define TOUCHPADVOLUMECONTROL_AUDIOHANDLER_HPP

#include <alsa/asoundlib.h>

class AudioHandler {

public:
    AudioHandler();
    void increaseVolume(long newVolume);

    ~AudioHandler();

private:
    void initializeMixer();
    void initializeParams();
    void cleanUp(int error = 0);
    void checkError(int err);

private:
    snd_mixer_t *mixer = nullptr;
    snd_mixer_selem_id_t *sid = nullptr;
    snd_mixer_elem_t *elem = nullptr;
    long volume{};
    long min{};
    long max{};
};


#endif //TOUCHPADVOLUMECONTROL_AUDIOHANDLER_HPP
