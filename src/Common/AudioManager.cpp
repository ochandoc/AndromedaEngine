#include "Common/AudioManager.h"


namespace And{

AudioManager::AudioManager(){

}

AudioManager::~AudioManager(){}

void AudioManager::init(){
  m_soloud.init();
}

void AudioManager::play(Audio* audio){

  m_soloud.play(audio->m_sample);
}

void AudioManager::pause(Audio* audio){

}

void AudioManager::stop(Audio* audio){

}
}
