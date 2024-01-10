#include "Common/ShaderTextEditor.h"


#include "TextEditor.h"
#include "Common/Threw.h"

struct ShaderInfo
{
  ShaderInfo()
  {
    shaderEditor.SetLanguageDefinition(TextEditor::LanguageDefinition::GLSL());
    shaderEditor.SetShowWhitespaces(false);
    shaderEditor.SetTabSize(2);
  }

  std::string Type;
  TextEditor shaderEditor;
};

struct ShaderTextEditorData
{
  std::vector<ShaderInfo> shaderEditors;
  bool bOpen;
  std::string Path;
};

ShaderTextEditor::ShaderTextEditor(const char* Path) : m_Data(new ShaderTextEditorData)
{
  m_Data->bOpen = true;
  m_Data->Path = Path;
  {
    ShaderInfo info;
    info.Type = "Vertex";
    info.shaderEditor.SetText("Hola");
    m_Data->shaderEditors.push_back(info);
  }
  {
    ShaderInfo info;
    info.Type = "Fragment";
    info.shaderEditor.SetText("Adios");
    m_Data->shaderEditors.push_back(info);
  }
}

ShaderTextEditor::~ShaderTextEditor()
{
}

void ShaderTextEditor::do_something()
{
  static ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar;

  static bool bSaveFile = false;

  if (m_Data->bOpen)
  {
    if (ImGui::Begin(m_Data->Path.c_str(), &m_Data->bOpen, windowFlags))
    {
      if (ImGui::BeginMenuBar())
      {
        if (ImGui::BeginMenu("File"))
        {
          if (ImGui::MenuItem("Save"))
            bSaveFile = true;

          ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
      }

      ImGui::BeginTabBar("Shaders");

      for (ShaderInfo& shader : m_Data->shaderEditors)
      {
        if (ImGui::BeginTabItem(shader.Type.c_str()))
        {
          shader.shaderEditor.Render(shader.Type.c_str());
          ImGui::EndTabItem();
        }
      }

      ImGui::EndTabBar();

      if (bSaveFile)
      {
        std::string ShaderSource;
        for (ShaderInfo& shader : m_Data->shaderEditors)
        {
          ShaderSource += "#type ";
          ShaderSource += shader.Type + "\n";
          ShaderSource += shader.shaderEditor.GetText();
        }
        FILE* f;
        f = fopen(m_Data->Path.c_str(), "wb");

        if (f == NULL) {
          printf("\n*** Error writing file ***\n");
          //return false;
        }
        else {
          fwrite(ShaderSource.c_str(), 1, ShaderSource.size(), f);
          fclose(f);
        }
        bSaveFile = false;
      }
    }
    ImGui::End();
  }
}
