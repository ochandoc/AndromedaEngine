#include "Common/AudioManager.h"
#include <AL/al.h>
#include <AL/alc.h>
#include <memory>


//std::unique_ptr<SoLoud::Soloud> soloud_engine;

namespace And{

ALCdevice* device;
ALCcontext* context;

// : m_soloud(std::make_unique<SoLoud::Soloud>())
AudioManager::AudioManager(){
  //SoLoud::Soloud soloud_engine;
  device = alcOpenDevice(nullptr);  // nullptr para el dispositivo predeterminado
  context = alcCreateContext(device, nullptr);
  alcMakeContextCurrent(context);

  if(!device || !context){
    printf("\nError\n");
  }
  init();
}

AudioManager::~AudioManager(){
 // m_soloud->deinit();
}

void AudioManager::init(){
  //m_soloud->init();
}

void AudioManager::play(Audio audio){
  //if(audio){

    //auto audioSource = m_soloud->play(std::move(*audio->get_sample()));

    //m_soloud->play(audio->get_sample());
  //}
}

void AudioManager::pause(Audio* audio){

}

void AudioManager::stop(Audio* audio){

}
}
