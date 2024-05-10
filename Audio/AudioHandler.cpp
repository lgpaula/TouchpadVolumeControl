#include "AudioHandler.hpp"
#include <iostream>
#include <algorithm>

AudioHandler::AudioHandler() {
    initializeMixer();
}

void AudioHandler::initializeMixer() {
    int err;

    if ((err = snd_mixer_open(&mixer, 0)) < 0) {
        cleanUp(err);
        return;
    }

    if ((err = snd_mixer_attach(mixer, "default")) < 0) {
        cleanUp(err);
        return;
    }

    if ((err = snd_mixer_selem_register(mixer, nullptr, nullptr)) < 0) {
        cleanUp(err);
        return;
    }

    if ((err = snd_mixer_load(mixer)) < 0) {
        cleanUp(err);
        return;
    }

    snd_mixer_selem_id_malloc(&sid);
    snd_mixer_selem_id_set_index(sid, 0);
    snd_mixer_selem_id_set_name(sid, "Master");
    elem = snd_mixer_find_selem(mixer, sid);

    if (!elem) {
        std::cerr << "Cannot find simple element." << std::endl;
        cleanUp();
        return;
    }

    initializeParams();
}

void AudioHandler::initializeParams() {
    snd_mixer_selem_get_playback_volume(elem, SND_MIXER_SCHN_MONO, &volume);
    snd_mixer_selem_get_playback_volume_range(elem, &min, &max);
}

void AudioHandler::increaseVolume(long newVolume) {
    if (volume == max && newVolume > 0) return;
    if (volume == min && newVolume < 0) return;

    snd_mixer_selem_set_playback_volume_all(elem, volume + newVolume * max / 100);
    volume += newVolume * max / 100;
    volume = std::clamp(volume, min, max);
}

void AudioHandler::cleanUp(int error) {
    if (mixer) snd_mixer_close(mixer);
    if (sid) snd_mixer_selem_id_free(sid);
    if (elem) snd_mixer_elem_free(elem);

    if (error) std::cerr << "Error occurred: " << snd_strerror(error) << std::endl;
}
