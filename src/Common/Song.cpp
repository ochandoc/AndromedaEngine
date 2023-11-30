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

void Song::show_imgui(){
  if(ImGui::CollapsingHeader(m_name)){
    if(ImGui::Button("Play All")){
      play();
    }
    for(auto& audio : m_audios){
      if(ImGui::CollapsingHeader(audio->get_name())){
        {
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
        }
        
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
