#include "Common/Audio.h"

namespace And{


Audio::Audio(){

}

Audio::~Audio(){}

void Audio::load(const char* path){
  if(path){
    m_sample.load(path);
  }

}

}