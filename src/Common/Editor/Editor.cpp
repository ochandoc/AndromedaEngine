#include "Common/Editor/Editor.h"
#include "Common/Log.h"
#include "Common/ImGuiSink.h"


namespace And{

Editor::Editor(){

  // Logger, shader editor, job system, content browser
  m_Windows.push_back(std::shared_ptr<LogWindow>(new LogWindow{"Log Window"}));
  
}

Editor::~Editor(){

}

void Editor::ShowWindows(){

  for(auto& e : m_Windows){
    if(e->m_is_open){
      e->Show();
    }else{
      std::string title = "Open window: " + e->m_title;  
      if(ImGui::Button(title.c_str())){
        e->m_is_open = true;
      }

    }
  }
}
}