#include "Andromeda/Audio/AudioManager.h"
#include "Andromeda/Audio/Audio.h"
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>
#include <AL/alext.h>
#include "imgui.h"



// Definiciones de funciones de EFX
//AL_API ALvoid AL_APIENTRY alGenEffects(ALsizei n, ALuint* effects);
//AL_API ALvoid AL_APIENTRY alDeleteEffects(ALsizei n, const ALuint* effects);
//AL_API ALboolean AL_APIENTRY alIsEffect(ALuint effect);

namespace And{

struct AudioContext{
  ALCdevice* device;
  ALCcontext* context;
  ALuint source;
  ALuint buffer;
};




// RAII
AudioManager::AudioManager(){

  m_audio_data = std::make_shared<AudioContext>();

  const char * devicename = alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);
  m_audio_data->device = alcOpenDevice(devicename);
  m_audio_data->context = alcCreateContext(m_audio_data->device, nullptr);
  alcMakeContextCurrent(m_audio_data->context);


  alDistanceModel(AL_LINEAR_DISTANCE_CLAMPED);
  

  if(!m_audio_data->device || !m_audio_data->context){
    printf("\nError\n");
  }


  // Check version para los efectos
  ALCint major, minor;
  alcGetIntegerv(m_audio_data->device, ALC_MAJOR_VERSION, sizeof(ALint), &major);
  alcGetIntegerv(m_audio_data->device, ALC_MINOR_VERSION, sizeof(ALint), &minor);
  if (major == 0 && minor == 0) {
      printf("*** No es compatible ***\n");
  }
  else {
      printf("*** Es compatible **\n");
  }



  if (alcIsExtensionPresent(m_audio_data->device, "ALC_EXT_EFX") == AL_TRUE) {
      m_efx_available_ = true;
      printf("ALC_EXT_EFX found\n");
      //ALuint effect;
      //alGenEffects(1, &effect);

  }
  else {
      m_efx_available_ = false;
  }


}

AudioManager::~AudioManager(){
  alcMakeContextCurrent(nullptr); 
  alcDestroyContext(m_audio_data->context);
  alcCloseDevice(m_audio_data->device);
  //delete m_audio_data;
}

void AudioManager::Update() {
    alcProcessContext(m_audio_data->context);
}

bool AudioManager::isPlaying(Audio& audio){
  ALuint source = audio.get_source();
  ALint source_state;
  alGetSourcei(source, AL_SOURCE_STATE, &source_state);
  return (source_state == AL_PLAYING);
}

// El audio contiene el buffer a reproducir
void AudioManager::play(Audio& audio){
  ALuint src = audio.get_source();

  if(!isPlaying(audio)){
    audio.ApplyEffects();
    alSourcei(src, AL_BUFFER, audio.get_buffer());
    alSourcePlay(src);
  }
}

void AudioManager::pause(Audio& audio){
  alSourcePause(audio.get_source());
}

void AudioManager::resume(Audio& audio){
  alSourcePlay(audio.get_source());
}

void AudioManager::stop(Audio& audio){
  alSourceStop(audio.get_source());
}

void AudioManager::show_imgui(Audio& audio){

  if(ImGui::CollapsingHeader(audio.get_name())){

      {
        std::string ButtonName = "Play";
        ButtonName += "##";
        ButtonName += audio.get_name();
        if(ImGui::Button(ButtonName.c_str())){
          play(audio);
        }
      }

    ImGui::SameLine();
    
    {
        std::string ButtonName = "Pause";
        ButtonName += "##";
        ButtonName += audio.get_name();
        if(ImGui::Button(ButtonName.c_str())){
          pause(audio);
        }
    }
    ImGui::SameLine();
   
    {
        std::string ButtonName = "Resume";
        ButtonName += "##";
        ButtonName += audio.get_name();
        if(ImGui::Button(ButtonName.c_str())){
          resume(audio);
        }
    }
    ImGui::SameLine();
    {
        std::string ButtonName = "Stop";
        ButtonName += "##";
        ButtonName += audio.get_name();
        if(ImGui::Button(ButtonName.c_str())){
          stop(audio);
        }
    }

    ImGui::SameLine();

    if (ImGui::CollapsingHeader("Effects")) {


        {
            std::string ButtonName = "Pitch";
            ButtonName += "##";
            ButtonName += audio.get_name();
            float pitch = audio.GetPitch();
            ImGui::DragFloat(ButtonName.c_str(), &pitch, 0.05f, 0.5f, 2.0f);
            audio.SetPitch(pitch);
            //audio.ApplyEffects();
            
        }
        
        {
            std::string ButtonName = "Gain";
            ButtonName += "##";
            ButtonName += audio.get_name();
            float value = audio.GetGain();
            ImGui::DragFloat(ButtonName.c_str(), &value, 0.1f, 0.0f);
            audio.SetGain(value);
            //audio.ApplyEffects();
            
        }
        {
            std::string ButtonName = "Position";
            ButtonName += "##";
            ButtonName += audio.get_name();

            float position[3];
            audio.GetPosition(position[0], position[1], position[2]);
            ImGui::DragFloat3(ButtonName.c_str(), position, 0.1f, 0.0f);
            audio.SetPosition(position);
            //audio.ApplyEffects();

        }
        
        {
            std::string ButtonName = "Velocity";
            ButtonName += "##";
            ButtonName += audio.get_name();

            float v[3];
            audio.GetVelocity(v[0], v[1], v[2]);
            ImGui::DragFloat3(ButtonName.c_str(), v, 0.1f, 0.0f);
            audio.SetVelocity(v);
            //audio.ApplyEffects();

        }
        
        {
            std::string ButtonName = "Looping";
            ButtonName += "##";
            ButtonName += audio.get_name();
            bool value = audio.GetLooping();
            ImGui::Checkbox(ButtonName.c_str(),&value);
            audio.SetLooping(value);
            //audio.ApplyEffects();
            
        }

        if (ImGui::CollapsingHeader("Doppler")) {
            {
                std::string ButtonName = "Enabled";
                ButtonName += "##";
                ButtonName += audio.get_name();
                bool value = audio.GetDopplerEnabled();
                ImGui::Checkbox(ButtonName.c_str(), &value);
                audio.SetDoppler(value);
                //audio.ApplyEffects();

            }

            

            if (audio.GetDopplerEnabled()) {
                ImGui::SameLine();
                std::string ButtonName = "Doppler Factor";
                ButtonName += "##";
                ButtonName += audio.get_name();
                float value = audio.GetDopplerFactor();
                ImGui::DragFloat(ButtonName.c_str(), &value, 0.1f, 0.0f);
                audio.SetDopplerFactor(value);
                //audio.ApplyEffects();

               
            }

            
        }




        
    }
    
  }

  audio.ApplyEffects();
}
}
