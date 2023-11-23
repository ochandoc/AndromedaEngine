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

    bool load(const char* path, const char* name = nullptr);

    unsigned int get_buffer();
    unsigned int get_source();

	const float m_speedOfSound = 343.3f;
	
	
	// Must be 0.0f to 1.0f
	void SetPitch(float pitch);

	// Must be 0.0f to 1.0f
	void SetGain(float gain);
	
	void SetPosition(float x, float y, float z);
	void SetPosition(float position[3]);

	void SetVelocity(float x, float y, float z);
	void SetVelocity(float velocity[3]);

	void SetLooping(bool loop);

	void SetDoppler(bool active);

	// Must be 0 - 10.0f. 1.0f to neutral
	void SetDopplerFactor(float factor);


	void UpdateSourcePosition(float x, float y, float z);
	void UpdateSourcePosition(float pos[3]);


	float GetPitch();
	float GetGain();
	void GetPosition(float& x, float& y, float& z);
	void GetPosition(float pos[3]);
	void GetVelocity(float& x, float& y, float& z);
	void GetVelocity(float pos[3]);
	bool GetLooping();

	bool GetDopplerEnabled();
	float GetDopplerFactor();
	void GetSourcePosition(float& x, float& y, float& z);

    const char* get_name();

	friend class AudioManager;
	friend class Song;

protected:
	void ApplyEffects();
	AudioEffect* m_audio_effect;
	AudioData* m_audio_data;
	bool m_muted = false;

  private:


}; 

}