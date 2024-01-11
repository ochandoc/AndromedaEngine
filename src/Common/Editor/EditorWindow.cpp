#include "Common/Editor/EditorWindow.h"
#include "Common/Editor/Editor.h"


namespace And{


EditorWindow::EditorWindow(){
  m_is_open = true;

}

EditorWindow::~EditorWindow(){

}


void EditorWindow::SetEditor(Editor* e){
  m_Editor = e;
}

}