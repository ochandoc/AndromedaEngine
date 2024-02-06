#pragma once
#include "EditorWindow.h"
#include "Andromeda/Resources/ResourceManager.h"
#include <vector>
#include <memory>

namespace And{

  class Editor
  {
  public:

    Editor(Window& window, ResourceManager* rm = nullptr);
    Editor(const Editor&) = delete;
    Editor(Editor&&) = delete;
    ~Editor();

    void AddWindow(std::shared_ptr<EditorWindow> window);
    void ShowWindows();

  private:
    std::unordered_map<std::string, std::shared_ptr<EditorWindow>> m_Windows;
    Window& m_MainWindow;
    ResourceManager* m_resourceManager;
  };
}