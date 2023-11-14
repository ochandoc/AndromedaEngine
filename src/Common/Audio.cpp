#include "Common/Audio.h"
#include <AL/al.h>
#include <AL/alc.h>

namespace And{

// : m_sample(std::make_shared<SoLoud::Wav>())
Audio::Audio() {
  //m_sample->createInstance();
}

Audio::~Audio(){}

//SoLoud::Wav* Audio::get_sample(){
  //return m_sample.get();
  //return nullptr;
//}

void Audio::load(const char* path){
  if(path){
    //m_sample->load(path);
  }

}

}