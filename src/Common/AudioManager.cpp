#include "Common/AudioManager.h"
#include <AL/al.h>
#include <AL/alc.h>
#include <memory>
#include "imgui.h"


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
  //alGenSources(1, &(m_audio_data->source));
}

AudioManager::~AudioManager(){
  alcMakeContextCurrent(nullptr);
  alcDestroyContext(m_audio_data->context);
  alcCloseDevice(m_audio_data->device);
  delete m_audio_data;
}

bool AudioManager::isPlaying(Audio& audio){
  ALuint source = audio.get_source();
  ALint source_state;
  alGetSourcei(source, AL_SOURCE_STATE, &source_state);
  return (source_state == AL_PLAYING);
}

// El audio contiene el buffer a reproducir
void AudioManager::play(Audio& audio){
  ALuint src = audio.get_source();

  if(!isPlaying(audio)){
    alSourcei(src, AL_BUFFER, audio.get_buffer());
    alSourcePlay(src);
  }
}

void AudioManager::pause(Audio& audio){
  alSourcePause(audio.get_source());
}

void AudioManager::resume(Audio& audio){
  alSourcePlay(audio.get_source());
}

void AudioManager::stop(Audio& audio){
  alSourceStop(audio.get_source());
}


void AudioManager::show_imgui(Audio& audio){

  if(ImGui::CollapsingHeader(audio.get_name())){
    if(ImGui::Button("Play")){
      play(audio);
    }
    
    if(ImGui::Button("Pause")){
      pause(audio);
    }
   
    if(ImGui::Button("Resume")){
      resume(audio);
    }
    
    if(ImGui::Button("Stop")){
      stop(audio);
    }
  }
}
}
