#pragma once


namespace SoLoud{
  class Wav;
}

namespace And{

class Audio{

  public:
    Audio();
    Audio(const Audio&) = default;
    Audio(Audio&&) = default;

    ~Audio();

    void load(const char* path);

    SoLoud::Wav* get_sample();


  private:
    std::shared_ptr<SoLoud::Wav> m_sample;
}; 

}