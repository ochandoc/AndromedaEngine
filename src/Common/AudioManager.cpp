#include "Common/AudioManager.h"
#include "soloud_c.h"
#include "soloud_wav.h"


namespace And{

AudioManager::AudioManager() : m_soloud(std::make_unique<SoLoud::Soloud>()){
  init();
}

AudioManager::~AudioManager(){
  m_soloud->deinit();
}

void AudioManager::init(){
  m_soloud->init();
}

void AudioManager::play(Audio* audio){
  if(audio){

    //auto audioSource = m_soloud->play(std::move(*audio->get_sample()));

    //m_soloud->play(audio->get_sample());
  }
}

void AudioManager::pause(Audio* audio){

}

void AudioManager::stop(Audio* audio){

}
}
