#pragma once


namespace And{

  struct AudioData;

class Audio{

  public:
    Audio();
    Audio(const Audio&) = default;
    Audio(Audio&&) = default;

    ~Audio();

    void load(const char* path);


  private:

   AudioData* m_audio_data;
}; 

}