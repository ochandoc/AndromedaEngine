#pragma once
#include "EditorWindow.h"
#include <vector>
#include <memory>

namespace And{

  class Editor
  {
  public:

    Editor();
    Editor(const Editor&) = delete;
    Editor(Editor&&) = delete;
    ~Editor();

    void ShowWindows();

  private:
    std::unordered_map<std::string, std::shared_ptr<EditorWindow>> m_Windows;
  };
}