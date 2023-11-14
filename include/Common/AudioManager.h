#pragma once
#include "Audio.h"

//namespace SoLoud{
  //class Soloud;
//}

namespace And{

class AudioManager{

  public:
    AudioManager();
    AudioManager(const AudioManager&) = default;
    AudioManager(AudioManager&&) = default;

    ~AudioManager();

    void play(Audio audio);
    void pause(Audio* audio);
    void stop(Audio* audio);


  private:
    void init();
    //ALCdevice* m_device;
    //ALCcontext* m_context;



};

}