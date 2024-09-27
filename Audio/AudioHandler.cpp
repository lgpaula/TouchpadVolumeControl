#include "AudioHandler.hpp"
#include <iostream>
#include <algorithm>
#include <chrono>

AudioHandler::AudioHandler() {
    initializeMixer();
}

void AudioHandler::checkError(int err) {
    if (err < 0) cleanUp(err);
}

void AudioHandler::initializeMixer() {
    checkError(snd_mixer_open(&mixer, 0));
    checkError(snd_mixer_attach(mixer, "default"));
    checkError(snd_mixer_selem_register(mixer, nullptr, nullptr));
    checkError(snd_mixer_load(mixer));

    snd_mixer_selem_id_malloc(&sid);
    snd_mixer_selem_id_set_index(sid, 0);
    snd_mixer_selem_id_set_name(sid, "Master");
    elem = snd_mixer_find_selem(mixer, sid);

    if (!elem) cleanUp();

    initializeParams();
}

void AudioHandler::initializeParams() {
    snd_mixer_selem_get_playback_volume(elem, SND_MIXER_SCHN_MONO, &volume);
    snd_mixer_selem_get_playback_volume_range(elem, &min, &max);
}

void AudioHandler::increaseVolume(long newVolume) {
    if (volume == max && newVolume > 0) return;
    if (volume == min && newVolume < 0) return;

    static auto lastUpdate = std::chrono::steady_clock::now();
    auto now = std::chrono::steady_clock::now();
    if (now - lastUpdate < std::chrono::milliseconds(50)) return;
    lastUpdate = now;

    snd_mixer_selem_set_playback_volume_all(elem, volume + newVolume * max / 100);
    volume += newVolume * max / 100;
    volume = std::clamp(volume, min, max);
}

void AudioHandler::toggleMute() {
    snd_mixer_selem_set_playback_switch_all(elem, isMuted);
    isMuted = !isMuted;
}

void AudioHandler::cleanUp(int error) {
    if (mixer) snd_mixer_close(mixer);
    if (sid) snd_mixer_selem_id_free(sid);
    if (elem) snd_mixer_elem_free(elem);

    if (error) throw std::runtime_error(snd_strerror(error));
}

AudioHandler::~AudioHandler() {
    cleanUp();
}