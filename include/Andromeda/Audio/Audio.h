#pragma once



namespace And{
class AudioManager;
class Song;


struct AudioData;
struct AudioEffect;

/*
* Struct that holds the RIFF data of the Wave file.
* The RIFF data is the meta data information that holds,
* the ID, size and format of the wave file
*/
struct RIFF_Header {
	char chunkID[4];
	long chunkSize;  //size not including chunkSize or chunkID
	char format[4];
};

/*
* Struct to hold fmt subchunk data for WAVE files.
*/
struct WAVE_Format {
	char subChunkID[4];
	long subChunkSize;
	short audioFormat;
	short numChannels;
	long sampleRate;
	long byteRate;
	short blockAlign;
	short bitsPerSample;
};

/*
* Struct to hold the data of the wave file
*/
struct WAVE_Data {
	char subChunkID[4];  //should contain the word data
	long subChunk2Size;  //Stores the size of the data block
};


class Audio{



  public:
    Audio();
    Audio(const Audio&) = default;
    Audio(Audio&&) = default;

    ~Audio();

		/**
    * @brief Load audio file
    * @param Path of the file
    * @param Name of the file (optional)
    */
    bool load(const char* path, const char* name = nullptr);

		/**
    * @return buffer id og file
    */
    unsigned int get_buffer();

		/**
    * @return raw source of file
    */
    unsigned int get_source();

	const float m_speedOfSound = 343.3f;
	
	/**
	* @brief Set pitch of the Audio
	* @param float pitch value 0.0f to 1.0f
	*/
	// Must be 0.0f to 2.5f
	void SetPitch(float pitch);

	/**
	* @brief Set gain of the Audio
	* @param float gain value 0.0f to 1.0f
	*/
	// Must be 0.0f to 1.0f
	void SetGain(float gain);
	
	/**
	* @brief Set position of the Audio in world
	* @param float x position
	* @param float y position
	* @param float z position
	*/
	void SetPosition(float x, float y, float z);
	/**
	* @brief Set position of the Audio in world
	* @param float* of positions
	*/
	void SetPosition(float position[3]);

	/**
	* @brief Set velocity of the Audio in world
	* @param float x velocity
	* @param float y velocity
	* @param float z velocity
	*/
	void SetVelocity(float x, float y, float z);
	/**
	* @brief Set velocity of the Audio in world
	* @param float* of velocities
	*/
	void SetVelocity(float velocity[3]);

	/**
	 * @brief Set the Looping object
	 * 
	 * @param bool loop 
	 */
	void SetLooping(bool loop);

	/**
	 * @brief Set the Doppler object active or inactive
	 * 
	 * @param bool active 
	 */
	void SetDoppler(bool active);

	/**
	 * @brief Set the Doppler Factor object
	 * 
	 * @param float doppler factor 0.0f to 10.0f, 1.0f to neutral 
	 */
	// Must be 0 - 10.0f. 1.0f to neutral
	void SetDopplerFactor(float factor);

	/**
	 * @brief Updates source position every frame
	 * 
	 * @param float x position 
	 * @param float y position 
	 * @param float z position 
	 */
	void UpdateListenerPosition(const float x, const float y, const float z);
	/**
	 * @brief Updates source position every frame
	 * 
	 * @param float* positions  
	 */
	void UpdateListenerPosition(const float pos[3]);

	void UpdateListenerDirection(const float pos[3]);
	void UpdateListenerDirection(const float x, const float y, const float z);

	/**
	 * @brief Get the Pitch value
	 * 
	 * @return float pitch value
	 */
	float GetPitch();
	/**
	 * @brief Get the Gain value
	 * 
	 * @return float gain value
	 */
	float GetGain();

	/**
	 * @brief Get the Position object
	 * 
	 * @param float& x position
	 * @param float& y position
	 * @param float& z position
	 */
	void GetPosition(float& x, float& y, float& z);
	/**
	 * @brief Get the Position object
	 * 
	 * @param float* positions
	 */
	void GetPosition(float pos[3]);

	/**
	 * @brief Get the Velocity object
	 * 
	 * @param float& x velocity
	 * @param float& y velocity
	 * @param float& z velocity
	 */
	void GetVelocity(float& x, float& y, float& z);
	/**
	 * @brief Get the Velocity object
	 * 
	 * @param float* velocities
	 */
	void GetVelocity(float pos[3]);

	/**
	 * @brief Get the Looping value
	 * 
	 * @return true 
	 * @return false 
	 */
	bool GetLooping();

	/**
	 * @brief Get the Doppler Enabled value
	 * 
	 * @return true 
	 * @return false 
	 */
	bool GetDopplerEnabled();

	/**
	 * @brief Get the Doppler Factor value
	 * 
	 * @return float doppler factor 0.0f to 10.0f
	 */
	float GetDopplerFactor();

	/**
	 * @brief Get the Source Position object
	 * 
	 * @param float& x position 
	 * @param float& y position 
	 * @param float& z position 
	 */
	void GetSourcePosition(float& x, float& y, float& z);

	/**
	 * @brief Get the name object
	 * 
	 * @return const char* 
	 */
  const char* get_name();

  /**
    * @brief Set maxium distance audio will be listened
	*
	* @param const float max distance
  */
   void SetMaxDistance(const float distance);



	/**
 	* @brief Apply effects setted before, must be called every frame
 	*/
	void ApplyEffects();
	
	
	friend class AudioManager;
	friend class Song;

protected:

	/**
	 * @brief AudioEffect* struct
	 * 
	 */
	AudioEffect* m_audio_effect;

	/**
	 * @brief AudioData* struct
	 * 
	 */
	AudioData* m_audio_data;

	/**
	 * @brief mutted sound
	 * 
	 */
	bool m_muted = false;

  private:


}; 

}