#include "GameInfoWindow.h"
#include "Andromeda/HAL/Window.h"
#include "imgui.h"

namespace And{

  GameInfoWindow::GameInfoWindow() : EditorWindow("Game Info Window"){

  }

  GameInfoWindow::~GameInfoWindow(){
    
  }


  void GameInfoWindow::Show(){
    float dt = m_Window->get_delta_time();
    if(m_is_open){
		  if(ImGui::Begin("Game Info Window", &m_is_open)){
        ImGui::Text("Delta Time: %f",dt);
        ImGui::Text("FPS: %f",1.0f/dt);

      }
      ImGui::End();
    }


  }
  
}