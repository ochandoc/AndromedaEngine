#include "Andromeda/Audio/Audio.h"
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace And{


  struct AudioData{
    std::string filename;
    const char* name;
    ALuint buffer;
    ALsizei size;
    ALsizei frequency; 
    ALenum format;
    ALuint source;
  };


  struct AudioEffect{
      ALfloat source_pos[3] = { 0.0f };
    ALfloat source_dir[3] = { 0.0f };
    ALfloat source_vel[3] = { 0.0f };


    float pitch;
    float gain;
    unsigned int looping;

    bool doppler_enabled;
    float doppler_factor;
    ALfloat listener_vel[3] = { 0.0f };
    ALfloat listener_pos[3] = { 0.0f };
    ALfloat listener_orientation[6] = { 0.0f, 0.0f, 1.0f,
                                        0.0f, 1.0f,0.0f};
    
    // EFX extension
    float reverberation;
    float delay;
    float low_high;
    float distorsion;
    ALuint effect;

  };


Audio::Audio(){

    m_audio_data = std::make_shared<AudioData>();
    m_audio_effect = std::make_shared<AudioEffect>();

    m_audio_effect->pitch = 1.0f;
    m_audio_effect->gain = 1.0f;
    m_audio_effect->looping = 0;

    m_audio_effect->source_pos[0] = 0.0f;
    m_audio_effect->source_pos[1] = 0.0f;
    m_audio_effect->source_pos[2] = 0.0f;
    
    m_audio_effect->source_dir[0] = 1.0f;
    m_audio_effect->source_dir[1] = 1.0f;
    m_audio_effect->source_dir[2] = 1.0f;



    m_audio_effect->source_vel[0] = 0.0f;
    m_audio_effect->source_vel[1] = 0.0f;
    m_audio_effect->source_vel[2] = 0.0f;

    m_audio_effect->doppler_enabled = false;
    m_audio_effect->doppler_factor = 1.0f;
    m_audio_effect->listener_vel[0] = 0.0f;
    m_audio_effect->listener_vel[1] = 0.0f;
    m_audio_effect->listener_vel[2] = 0.0f;



}

Audio::~Audio(){
  alSourceStop(m_audio_data->source);
  alDeleteSources(1, &(m_audio_data->source));
  alDeleteBuffers(1, &(m_audio_data->buffer));
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
    // Local Declarations
    FILE* soundFile = nullptr;
    WAVE_Format wave_format;
    RIFF_Header riff_header;
    WAVE_Data wave_data;

    soundFile = fopen(filename.c_str(), "rb");
    if (!soundFile)
        return false;

    // Read in the first chunk into the struct
    fread(&riff_header, sizeof(RIFF_Header), 1, soundFile);

    // Check for RIFF and WAVE tag in memory
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

    // Read in the 2nd chunk for the wave info
    fread(&wave_format, sizeof(WAVE_Format), 1, soundFile);
    // Check for fmt tag in memory
    if (wave_format.subChunkID[0] != 'f' ||
        wave_format.subChunkID[1] != 'm' ||
        wave_format.subChunkID[2] != 't' ||
        wave_format.subChunkID[3] != ' ') {
        fclose(soundFile);
        return false;
    }

    // Check for extra parameters;
    if (wave_format.subChunkSize > 16)
        fseek(soundFile, sizeof(short), SEEK_CUR);

    // Read in the last byte of data before the sound file
    fread(&wave_data, sizeof(WAVE_Data), 1, soundFile);
    // Check for data tag in memory
    if (wave_data.subChunkID[0] != 'd' ||
        wave_data.subChunkID[1] != 'a' ||
        wave_data.subChunkID[2] != 't' ||
        wave_data.subChunkID[3] != 'a') {
        fclose(soundFile);
        return false;
    }

    // Allocate memory for data
    std::vector<unsigned char> data(wave_data.subChunk2Size);

    // Read in the sound data into the soundData variable
    if (!fread(data.data(), wave_data.subChunk2Size, 1, soundFile)) {
        fclose(soundFile);
        return false;
    }

    // Now we set the variables that we passed in with the data from the structs
    *size = wave_data.subChunk2Size;
    *frequency = wave_format.sampleRate;
    // The format is worked out by looking at the number of channels and the bits per sample.
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

    // Create our OpenAL buffer and check for success
    alGenBuffers(1, buffer);

    // Now we put our data into the OpenAL buffer and check for success
    alBufferData(*buffer, *format, data.data(), *size, *frequency);

    // Clean up and return true if successful
    fclose(soundFile);
    return true;
}



bool Audio::load(const char* path, const char* name){
  if(path){

    m_audio_data->filename = path;
    
    bool succes = LoadWavFile(m_audio_data->filename, &(m_audio_data->buffer), &(m_audio_data->size), &(m_audio_data->frequency), &(m_audio_data->format));

    if(!succes){
      printf("\n*** Error loading WAV file ***\n");
    
    }else{
      // Creamos la fuente
      alGenSources(1, &(m_audio_data->source));
      if(name){
        m_audio_data->name = name;
      }else{
        m_audio_data->name = {"Unnamed"};
      }
    }
    return succes;
    
  }
  return false;

}

// Called every frame for imgui 
void Audio::ApplyEffects() {
    
    alSourcef(m_audio_data->source, AL_PITCH, m_audio_effect->pitch);
    alSourcef(m_audio_data->source, AL_GAIN, m_audio_effect->gain);
    alSourcefv(m_audio_data->source, AL_POSITION, m_audio_effect->source_pos);
    //alSourcefv(m_audio_data->source, AL_DIRECTION, m_audio_effect->source_dir);
    alSourcei(m_audio_data->source, AL_LOOPING, m_audio_effect->looping);
    //alSourcefv(m_audio_data->source, AL_);


    if (m_audio_effect->doppler_enabled) {
        //printf("Aplicando doppler");

        alSourcefv(m_audio_data->source, AL_VELOCITY, m_audio_effect->source_vel);
        //alSource3f(m_audio_data->source, AL_VELOCITY, m_audio_effect->source_vel[0], m_audio_effect->source_vel[1], m_audio_effect->source_vel[2]);

        // 0.0f by default
        alListener3f(AL_VELOCITY, m_audio_effect->listener_vel[0], m_audio_effect->listener_vel[1], m_audio_effect->listener_vel[2]);

        // Apply doppler
        alDopplerFactor(m_audio_effect->doppler_factor);
        alDopplerVelocity(m_speedOfSound);

        // Set listener orientation
        //if (m_has_listener_orientation) {
            alListenerfv(AL_ORIENTATION, m_audio_effect->listener_orientation);
        //}

        alListenerfv(AL_POSITION, m_audio_effect->listener_pos);
        //alSource3f(m_audio_data->source, AL_POSITION, m_audio_effect->listener_pos[0], m_audio_effect->listener_pos[1], m_audio_effect->listener_pos[2]);

    }
    
}

void Audio::UpdateListenerPosition(const float x, const float y, const float z) {
    m_audio_effect->listener_pos[0] = x;
    m_audio_effect->listener_pos[1] = y;
    m_audio_effect->listener_pos[2] = z;


    //alSource3f(m_audio_data->source, AL_POSITION, m_audio_effect->source_pos[0], m_audio_effect->source_pos[1], m_audio_effect->source_pos[2]);
}

void Audio::UpdateListenerPosition(const float p[3]) {
    m_audio_effect->listener_pos[0] = p[0];
    m_audio_effect->listener_pos[1] = p[1];
    m_audio_effect->listener_pos[2] = p[2];

    

    //alSource3f(m_audio_data->source, AL_POSITION, m_audio_effect->source_pos[0], m_audio_effect->source_pos[1], m_audio_effect->source_pos[2]);
}

void Audio::UpdateListenerDirection(const float pos[3]){
    glm::vec3 forward(pos[0], pos[1], pos[2]);
    glm::vec3 up = glm::cross(glm::vec3(1.0f, 0.0f, 0.0f), forward);
    if (glm::length(up) < 0.01f) {
        up = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), forward);
    }
    up = glm::normalize(up);
    glm::vec3 right = glm::cross(forward, up);

    m_audio_effect->listener_orientation[0] = forward.x;
    m_audio_effect->listener_orientation[1] = forward.y;
    m_audio_effect->listener_orientation[2] = forward.z;
    m_audio_effect->listener_orientation[3] = up.x;
    m_audio_effect->listener_orientation[4] = up.y;
    m_audio_effect->listener_orientation[5] = up.z;

    m_audio_effect->source_dir[0] = pos[0];
    m_audio_effect->source_dir[1] = pos[1];
    m_audio_effect->source_dir[2] = pos[2];

    m_has_listener_orientation = true;
}

void Audio::UpdateListenerDirection(const float x, const float y, const float z){

    glm::vec3 forward(x, y, z);
    glm::vec3 up = glm::cross(glm::vec3(1.0f, 0.0f, 0.0f), forward);
    if (glm::length(up) < 0.01f) {
        up = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), forward);
    }
    up = glm::normalize(up);
    glm::vec3 right = glm::cross(forward, up);

    m_audio_effect->listener_orientation[0] = forward.x;
    m_audio_effect->listener_orientation[1] = forward.y;
    m_audio_effect->listener_orientation[2] = forward.z;
    m_audio_effect->listener_orientation[3] = up.x;
    m_audio_effect->listener_orientation[4] = up.y;
    m_audio_effect->listener_orientation[5] = up.z;

    m_audio_effect->source_dir[0] = x;
    m_audio_effect->source_dir[1] = y;
    m_audio_effect->source_dir[2] = z;

    m_has_listener_orientation = true;
}

void Audio::SetPitch(float pitch){
    if (pitch > 2.5f)pitch = 2.5f;
    if (pitch < 0.5f) pitch = 0.5f;

    m_audio_effect->pitch = pitch;
}

void Audio::SetGain(float gain) {
    if (gain < 0.0f) gain = 0.0f;
    if (gain > 1.0f) gain = 1.0f;
    m_audio_effect->gain = gain;
}

void Audio::SetPosition(float x, float y, float z){
    m_audio_effect->source_pos[0] = x;
    m_audio_effect->source_pos[1] = y;
    m_audio_effect->source_pos[2] = z;
}

void Audio::SetPosition(float p[3]) {
    m_audio_effect->source_pos[0] = p[0];
    m_audio_effect->source_pos[1] = p[1];
    m_audio_effect->source_pos[2] = p[2];
}

void Audio::SetVelocity(float x, float y, float z){
    m_audio_effect->source_vel[0] = x;
    m_audio_effect->source_vel[1] = y;
    m_audio_effect->source_vel[2] = z;
}

void Audio::SetVelocity(float v[3]) {
    m_audio_effect->source_vel[0] = v[0];
    m_audio_effect->source_vel[1] = v[1];
    m_audio_effect->source_vel[2] = v[2];
}

void Audio::SetLooping(bool loop){

    if (loop) {
        m_audio_effect->looping = 1;
    }else {
        m_audio_effect->looping = 0;
    }
}

void Audio::SetDoppler(bool active) {
    m_audio_effect->doppler_enabled = active;
}

void Audio::SetDopplerFactor(float factor) {

    if (factor > 10.0f) factor = 10.0f;
    if (factor < 0.0f) factor = 0.0f;

    m_audio_effect->doppler_factor = factor;
}

unsigned int Audio::get_buffer(){

  return m_audio_data->buffer;
}

unsigned int Audio::get_source(){

  return m_audio_data->source;
}

const char* Audio::get_name(){

  return m_audio_data->name;
}

void Audio::SetMaxDistance(const float distance){

    alSourcef(m_audio_data->source, AL_MAX_DISTANCE, distance);
}

float Audio::GetPitch() {
    return m_audio_effect->pitch;
}

float Audio::GetGain() {
    return m_audio_effect->gain;
}

void Audio::GetPosition(float& x, float& y, float& z) {
    x = m_audio_effect->source_pos[0];
    y = m_audio_effect->source_pos[1];
    z = m_audio_effect->source_pos[2];
}

void Audio::GetPosition(float pos[3]) {
    pos[0] = m_audio_effect->source_pos[0];
    pos[1] = m_audio_effect->source_pos[1];
    pos[2] = m_audio_effect->source_pos[2];
}

void Audio::GetVelocity(float& x, float& y, float& z) {
    x = m_audio_effect->source_vel[0];
    y = m_audio_effect->source_vel[1];
    z = m_audio_effect->source_vel[2];
}

void Audio::GetVelocity(float pos[3]) {
    pos[0] = m_audio_effect->source_vel[0];
    pos[1] = m_audio_effect->source_vel[1];
    pos[2] = m_audio_effect->source_vel[2];
}

bool Audio::GetLooping(){
    return m_audio_effect->looping;
}

bool Audio::GetDopplerEnabled() {
    return m_audio_effect->doppler_enabled;
}

float Audio::GetDopplerFactor(){
    return m_audio_effect->doppler_factor;
}

void Audio::GetSourcePosition(float& x, float& y, float& z){

    x = m_audio_effect->source_pos[0];
    y = m_audio_effect->source_pos[1];
    z = m_audio_effect->source_pos[2];
}

}