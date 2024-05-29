#pragma once
#include <vector>
#include <string>
#include "Andromeda/Misc/CoreMiscDefines.h"
#include "Andromeda/HAL/Types.h"

namespace And{

class Editor;
class Window;
class ResourceManager;


  /**
   * @class EditorWindow
   * @brief Base class for all editor windows.
   *
   * This class provides a basic interface and common functionality for editor windows.
   */
  class EditorWindow 
  {
    NON_COPYABLE_CLASS(EditorWindow)
    NON_MOVABLE_CLASS(EditorWindow)
  public:
    /**
     * @brief Constructs an EditorWindow with the given title.
     * @param Title The title of the editor window.
     */
    EditorWindow(const std::string& Title);
    /// Virtual destructor.
    virtual ~EditorWindow();

    friend class Editor;
  protected:
    bool m_is_open;                       /**< Indicates whether the window is open. */
    Editor* m_Editor;                     /**< Pointer to the associated editor. */
    std::string m_title;                  /**< The title of the window. */
    Window* m_Window;                     /**< Pointer to the associated window. */
    ResourceManager* m_resourceManager;   /**< Pointer to the resource manager. */

  private:
    void SetEditor(Editor*);
    virtual void Show() = 0;
  };
}