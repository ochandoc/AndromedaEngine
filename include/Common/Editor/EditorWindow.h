#pragma once
#include <vector>
#include <string>

#include "base.h"

namespace And{

class Editor;
class Window;

  class EditorWindow 
  {
    NON_COPYABLE_CLASS(EditorWindow)
    NON_MOVABLE_CLASS(EditorWindow)
  public:
    EditorWindow(const std::string& Title);

    virtual ~EditorWindow();

    friend class Editor;
  protected:
    bool m_is_open;
    Editor* m_Editor;
    std::string m_title;
    Window* m_Window;

  private:
    void SetEditor(Editor*);
    virtual void Show() = 0;
  };
}