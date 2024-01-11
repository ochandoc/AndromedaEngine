#include "Common/Editor/Editor.h"


namespace And{

Editor::Editor(){

  // Logger, shader editor, job system, content browser
  
}

Editor::~Editor(){

}

void Editor::ShowWindows(){

  for(auto& e : m_Windows){
    if(e->m_is_open){
      e->Show();
    }
  }
}
}