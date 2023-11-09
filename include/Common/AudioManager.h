#pragma once

#include "soloud.h"
#include "soloud_wav.h"
#include "Audio.h"

namespace And{

class AudioManager{

  public:
    AudioManager();
    AudioManager(const AudioManager&) = delete;
    AudioManager(AudioManager&&) = default;

    ~AudioManager();

    void init();
    void play(Audio* audio);
    void pause(Audio* audio);
    void stop(Audio* audio);


  private:

  SoLoud::Soloud m_soloud; // Engine core



};

}