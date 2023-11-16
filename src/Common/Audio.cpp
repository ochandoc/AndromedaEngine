#include "Common/Audio.h"
#include <AL/al.h>
#include <AL/alc.h>

namespace And{


  struct AudioData{
    std::string filename;
    ALuint buffer;
    ALsizei size;
    ALsizei frequency; 
    ALenum format;
  };


Audio::Audio() : m_audio_data(new AudioData){

}

Audio::~Audio(){
  printf("*** Audio destructor *** \n");
  alDeleteBuffers(1, &(m_audio_data->buffer));
  delete m_audio_data;
}

/**
* Load wave file function. No need for ALUT or ALURE with this
* @return Wether the file was succesfully loaded
* @param filename Path of the WAV file
* @param buffer OpenAL Buffer to load the WAV file to
* @param size Uncompressed sample size
* @param frequency Sample frequency (e.g: 11025, 22050, 44100...) in hertz
* @param format OpenAL sample format
*/
bool LoadWavFile(const std::string filename, ALuint* buffer, ALsizei* size, ALsizei* frequency, ALenum* format) {
	//Local Declarations
	FILE* soundFile = NULL;
	WAVE_Format wave_format;
	RIFF_Header riff_header;
	WAVE_Data wave_data;
	unsigned char* data;
	
		soundFile = fopen(filename.c_str(), "rb");
    if (!soundFile)
      return false;			

		// Read in the first chunk into the struct
		fread(&riff_header, sizeof(RIFF_Header), 1, soundFile);

		//check for RIFF and WAVE tag in memeory
    if ((riff_header.chunkID[0] != 'R' ||
      riff_header.chunkID[1] != 'I' ||
      riff_header.chunkID[2] != 'F' ||
      riff_header.chunkID[3] != 'F') ||
      (riff_header.format[0] != 'W' ||
        riff_header.format[1] != 'A' ||
        riff_header.format[2] != 'V' ||
        riff_header.format[3] != 'E')) {
      fclose(soundFile);
      return false;
    }

		//Read in the 2nd chunk for the wave info
		fread(&wave_format, sizeof(WAVE_Format), 1, soundFile);
		//check for fmt tag in memory
    if (wave_format.subChunkID[0] != 'f' ||
      wave_format.subChunkID[1] != 'm' ||
      wave_format.subChunkID[2] != 't' ||
      wave_format.subChunkID[3] != ' ') {
      fclose(soundFile);
      return false;
    }

		//check for extra parameters;
		if (wave_format.subChunkSize > 16)
			fseek(soundFile, sizeof(short), SEEK_CUR);

		//Read in the the last byte of data before the sound file
		fread(&wave_data, sizeof(WAVE_Data), 1, soundFile);
		//check for data tag in memory
    if (wave_data.subChunkID[0] != 'd' ||
      wave_data.subChunkID[1] != 'a' ||
      wave_data.subChunkID[2] != 't' ||
      wave_data.subChunkID[3] != 'a') {
      fclose(soundFile);
      return false;
    }

		//Allocate memory for data
		data = new unsigned char[wave_data.subChunk2Size];

		// Read in the sound data into the soundData variable
    if (!fread(data, wave_data.subChunk2Size, 1, soundFile)) {
      fclose(soundFile);
      return false;
    }

		//Now we set the variables that we passed in with the
		//data from the structs
		*size = wave_data.subChunk2Size;
		*frequency = wave_format.sampleRate;
		//The format is worked out by looking at the number of
		//channels and the bits per sample.
		if (wave_format.numChannels == 1) {
			if (wave_format.bitsPerSample == 8)
				*format = AL_FORMAT_MONO8;
			else if (wave_format.bitsPerSample == 16)
				*format = AL_FORMAT_MONO16;
		}
		else if (wave_format.numChannels == 2) {
			if (wave_format.bitsPerSample == 8)
				*format = AL_FORMAT_STEREO8;
			else if (wave_format.bitsPerSample == 16)
				*format = AL_FORMAT_STEREO16;
		}
		//create our openAL buffer and check for success
		alGenBuffers(1, buffer);
		
		//now we put our data into the openAL buffer and
		//check for success
		alBufferData(*buffer, *format, (void*)data,
			*size, *frequency);
		
		//clean up and return true if successful
		fclose(soundFile);
    delete[] data;
		return true;  
}



bool Audio::load(const char* path){
  if(path){

    m_audio_data->filename = path;
    
    bool succes = LoadWavFile(m_audio_data->filename, &(m_audio_data->buffer), &(m_audio_data->size), &(m_audio_data->frequency), &(m_audio_data->format));

    return succes;
    
  }

}


unsigned int Audio::get_buffer(){

  return m_audio_data->buffer;
}

}