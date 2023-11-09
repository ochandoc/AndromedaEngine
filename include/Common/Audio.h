#pragma once

#include "soloud.h"
#include "soloud_wav.h"

namespace And{

class Audio{

  public:
    Audio();
    Audio(const Audio&) = default;
    Audio(Audio&&) = default;

    ~Audio();

    void load(const char* path);
    SoLoud::Wav m_sample;

  private:
}; 

}