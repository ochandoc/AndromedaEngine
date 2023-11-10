#include "Common/Audio.h"
#include "soloud.h"
#include "soloud_wav.h"

namespace And{


Audio::Audio() : m_sample(std::make_shared<SoLoud::Wav>()){

}

Audio::~Audio(){}

SoLoud::Wav* Audio::get_sample(){
  return m_sample.get();
}

void Audio::load(const char* path){
  if(path){
    m_sample->load(path);
  }

}

}