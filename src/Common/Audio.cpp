#include "Common/Audio.h"
#include <AL/al.h>
#include <AL/alc.h>

namespace And{


  struct AudioData{
    ALuint buffer;
    ALenum format;
    ALvoid* data;
    ALsizei size;
    ALsizei frequency;
    ALboolean loop;
  }


Audio::Audio() : m_audio_data(new AudioData){

}

Audio::~Audio(){
  
  alDeleteBuffers(1, &(m_audio_data->buffer));
  free(m_audio_data->data);
  delete m_audio_data;
}


void Audio::load(const char* path){
  if(path){
    if (loadWAVFile(path, &(m_audio_data->format), &(m_audio_data->data),&( m_audio_data->size), &(m_audio_data->frequency), &(m_audio_data->loop))) {

      alGenBuffers(1, &(m_audio_data->buffer));
      alBufferData(&(m_audio_data->buffer), &(m_audio_data->format), &(m_audio_data->data), &(m_audio_data->size), &(m_audio_data->frequency));

    }
    
  }

}

}