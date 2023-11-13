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
    //std::unique_ptr<SoLoud::Soloud> m_soloud;
    //SoLoud::Soloud m_soloud; // Engine core



};

}