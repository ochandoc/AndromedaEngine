#pragma once
#include "Audio.h"


namespace And{

struct AudioContext;


class AudioManager{

  public:
    AudioManager();
    AudioManager(const AudioManager&) = default;
    AudioManager(AudioManager&&) = default;

    ~AudioManager();

    void play(Audio& audio);
    void pause(Audio& audio);
    void stop(Audio& audio);


  private:
    AudioContext* m_audio_data;
 
};

}