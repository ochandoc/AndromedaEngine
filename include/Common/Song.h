#pragma once

#include <vector>
#include "Audio.h"
#include "AudioManager.h"

namespace And{

class Song{

  public:

    Song(std::vector<Audio*> audios, AudioManager* manager, const char* name);
    Song(const Song&) = delete;
    Song(Song&&) = default;
    ~Song();


    void play();
    void show_imgui();

  private:

    std::vector<Audio*> m_audios;
    AudioManager* m_manager;
    const char* m_name;


    bool m_muted = false;

};
}