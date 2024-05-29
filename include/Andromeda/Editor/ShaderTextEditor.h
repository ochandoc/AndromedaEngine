#pragma once

#include "Andromeda/Editor/EditorWindow.h"

namespace And
{
  /**
   * @class ShaderTextEditor
   * @brief A specialized editor window for editing shader text files.
   *
   * This class provides functionality to load, edit, and save shader text files.
   */
  class ShaderTextEditor : public EditorWindow
  {
  public:
    /**
     * @brief Constructs a ShaderTextEditor with the given title.
     * @param Title The title of the shader text editor window.
     */
    ShaderTextEditor(const std::string& Title);

    /// Destructor.
    ~ShaderTextEditor();

    /**
    * @brief Loads the shader text from the specified path.
    * @param Path The path to the shader text file to load.
    */
    void Load(const std::string& Path);
    /// Saves the current shader text.
    void Save();

    /**
     * @brief Shows the shader text editor window.
     *
     * This function overrides the Show method from the EditorWindow base class.
     */
    void Show() override;

  private:
    /// A unique pointer to the data structure for the ShaderTextEditor.
    std::unique_ptr<struct ShaderTextEditorData> m_Data;
  };
}
