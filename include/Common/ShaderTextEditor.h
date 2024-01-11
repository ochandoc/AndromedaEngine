#pragma once

#include "Common/Editor/EditorWindow.h"

namespace And
{
  class ShaderTextEditor : public EditorWindow
  {
  public:
    ShaderTextEditor(const std::string& Title);

    ~ShaderTextEditor();

    void Load(const std::string& Path);
    void Save();

    void Show() override;

  private:
    std::unique_ptr<struct ShaderTextEditorData> m_Data;
  };
}
