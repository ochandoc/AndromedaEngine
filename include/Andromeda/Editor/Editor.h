#pragma once
#include "EditorWindow.h"
#include <vector>
#include <memory>

namespace And{

  /**
   * @class Editor
   * @brief Manages editor windows and their interactions.
   */
  class Editor
  {
  public:
    /**
     * @brief Constructs an Editor with the given main window.
     * @param window The main window reference.
     */
    Editor(Window& window);
    /// Deleted copy constructor.
    Editor(const Editor&) = delete;
    /// Deleted move constructor.
    Editor(Editor&&) = delete;
    /// Destructor.
    ~Editor();

    /**
     * @brief Adds a new window to the editor.
     * @param window A shared pointer to the editor window to be added.
     */
    void AddWindow(std::shared_ptr<EditorWindow> window);
    /**
     * @brief Displays all added windows.
     */
    void ShowWindows();

  private:
    std::unordered_map<std::string, std::shared_ptr<EditorWindow>> m_Windows;   /**< Map of window names to their shared pointers. */
    Window& m_MainWindow;                                                       /**< Reference to the main window. */
  };
}