#pragma once
#include "Audio.h"


namespace And{

struct AudioContext;


class AudioManager{

  public:
    /**
    * @brief Create audio object
    */
    AudioManager();
    AudioManager(const AudioManager&) = default;
    AudioManager(AudioManager&&) = default;

    ~AudioManager();

    /**
    * @brief Reproduce audio object
    * @param Audio object
    */
    void play(Audio& audio);

    /**
    * @brief Pause audio object
    * @param Audio object
    */
    void pause(Audio& audio);

    /**
    * @brief Resume audio file
    * @param Audio object
    */
    void resume(Audio& audio);

    /**
    * @brief Stop audio file
    * @param Audio object
    */
    void stop(Audio& audio);

    /**
    * @brief Show all funcitons on ImGui UI
    * @param Audio object
    */
    void show_imgui(Audio& audio);

    /**
    * @brief Check if audio is currently playing
    * @param Audio object
    * @return true if is playing
    * @return false if not playing
    */
    bool isPlaying(Audio& audio);

    bool m_efx_available_;

    /**
    * @brief Update efects, position and velocity of Audio object
    * @param Audio object
    */
    void Update();
  private:
    AudioContext* m_audio_data;
 
};

}