#include "Common/Song.h"
#include "imgui.h"

namespace And{

Song::Song(std::vector<Audio*> audios, AudioManager* manager, const char* name) : m_audios(audios){
  
  // size > 0
  m_manager = manager;
  m_name = name;
}

Song::~Song(){

}

void Song::play(){

  for(auto& audio : m_audios){
    if(!m_manager->isPlaying(*audio)){
      m_manager->play(*audio);
    }
  }

}

void Song::stop(){

  for(auto& audio : m_audios){
    if(m_manager->isPlaying(*audio)){
      m_manager->stop(*audio);
    }
  }

}

void Song::pause(){

  for(auto& audio : m_audios){
    if(m_manager->isPlaying(*audio)){
      m_manager->pause(*audio);
    }
  }

}

void Song::resume(){

  for(auto& audio : m_audios){
    if(!m_manager->isPlaying(*audio)){
      m_manager->resume(*audio);
    }
  }

}
// Drums, Guitar, Rytm, Vocal
void Song::show_imgui(){
  if(ImGui::CollapsingHeader(m_name)){
    if(ImGui::Button("Play All")){
      play();
    }
    ImGui::SameLine();
    if(ImGui::Button("Stop All")){
      stop();
    }
    ImGui::SameLine();
    if(ImGui::Button("Pause All")){
      pause();
    }
    ImGui::SameLine();
    if(ImGui::Button("Resume All")){
      resume();
    }

    // Explore mode plays rythm song
    if(ImGui::Button("Explore mode")){
      m_audios[2]->SetGain(1.0f);
      m_audios[0]->SetGain(0.0f);
      m_audios[1]->SetGain(0.0f);
      m_audios[3]->SetGain(0.0f);
    }

    // Exited mode plays all excep vocal
    if(ImGui::Button("Exited mode")){
     m_audios[0]->SetGain(1.0f); 
     m_audios[1]->SetGain(1.0f); 
     m_audios[2]->SetGain(1.0f); 
     m_audios[3]->SetGain(0.0f); 
    }

    // Fight mode plays all songs
    if(ImGui::Button("Fight mode")){
      for(auto& audio : m_audios){
        audio->SetGain(1.0f);
      }
    }

    for(auto& audio : m_audios){
      if(ImGui::CollapsingHeader(audio->get_name())){
        /*{
          std::string ButtonName = "Play";
          ButtonName += "##";
          ButtonName += audio->get_name();
          if(ImGui::Button(ButtonName.c_str())){
            m_manager->play(*audio);
          }
        }
        ImGui::SameLine();
        {
          std::string ButtonName = "Pause";
          ButtonName += "##";
          ButtonName += audio->get_name();
          if(ImGui::Button(ButtonName.c_str())){
            m_manager->pause(*audio);
          }
        }
        ImGui::SameLine();
        {
          std::string ButtonName = "Resume";
          ButtonName += "##";
          ButtonName += audio->get_name();
          if(ImGui::Button(ButtonName.c_str())){
            m_manager->resume(*audio);
          }
        }
        ImGui::SameLine();
        {
          std::string ButtonName = "Stop";
          ButtonName += "##";
          ButtonName += audio->get_name();
          if(ImGui::Button(ButtonName.c_str())){
            m_manager->stop(*audio);
          }
        }*/
        
        {
          std::string ButtonName = "Mute";
          ButtonName += "##";
          ButtonName += audio->get_name();
          if(ImGui::Checkbox(ButtonName.c_str(), &audio->m_muted)){
            
            if(audio->m_muted){
              audio->SetGain(0.0f);
            }else{
              audio->SetGain(1.0f);
            }
          }
        }

        
        {
            std::string ButtonName = "Gain";
            ButtonName += "##";
            ButtonName += audio->get_name();
            float value = audio->GetGain();
            ImGui::DragFloat(ButtonName.c_str(), &value, 0.1f, 0.0f);
            audio->SetGain(value);
        }

        audio->ApplyEffects();


      }
    }
  }

}
}
