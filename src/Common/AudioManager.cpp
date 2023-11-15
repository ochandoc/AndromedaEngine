#include "Common/AudioManager.h"
#include <AL/al.h>
#include <AL/alc.h>
#include <memory>


namespace And{

struct AudioContext{
  ALCdevice* device;
  ALCcontext* context;
  ALuint source;
  ALuint buffer;
};

// RAII
AudioManager::AudioManager() : m_audio_data(new AudioContext){

  const char * devicename = alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);
  m_audio_data->device = alcOpenDevice(devicename);
  m_audio_data->context = alcCreateContext(m_audio_data->device, nullptr);
  alcMakeContextCurrent(m_audio_data->context);

  if(!m_audio_data->device || !m_audio_data->context){
    printf("\nError\n");
  }

  // Creamos la fuente
  alGenSources(1, &(m_audio_data->source));
  
  // Creamos el buffer
  alGenBuffers(1, &(m_audio_data->buffer)); 

  // Asociamos la fuente al buffer
  alSourcei(m_audio_data->source, AL_BUFFER, m_audio_data->buffer);

  // Reproducimos la fuente
  alSourcePlay(m_audio_data->source);


  init();
}

AudioManager::~AudioManager(){
  alcMakeContextCurrent(nullptr);
  alcDestroyContext(m_audio_data->context);
  alcCloseDevice(m_audio_data->device);
  delete m_audio_data;
}

void AudioManager::init(){

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
