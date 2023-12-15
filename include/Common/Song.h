#pragma once

#include <vector>
#include "Audio.h"
#include "AudioManager.h"

namespace And{

class Song{

  public:

    /**
     * @brief Construct a new Song object
     * 
     * @param vector audio objects of the song 
     * @param AudioManager* 
     * @param char* name of the sound 
     */
    Song(std::vector<Audio*> audios, AudioManager* manager, const char* name);
    Song(const Song&) = delete;
    Song(Song&&) = default;
    ~Song();

    /**
     * @brief Play all audios of the sound at same time
     * 
     */
    void play();
    void stop();
    void pause();
    void resume();

    void update(float speed, float dt);

    bool m_change_to_explore = false;
    bool m_change_to_exited = false;
    bool m_change_to_fight = false;

    /**
     * @brief Show imgui UI with all options available
     * 
     */
    void show_imgui();

  private:

    std::vector<Audio*> m_audios;
    AudioManager* m_manager;
    const char* m_name;


    bool m_muted = false;

};
}