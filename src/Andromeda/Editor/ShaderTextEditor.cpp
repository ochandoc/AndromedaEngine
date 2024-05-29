#include "Andromeda/Editor/ShaderTextEditor.h"

#include "Andromeda/UI/TextEditor.h"
#include "Andromeda/Misc/Threw.h"
#include "Andromeda/Misc/Slurp.h"
#include "Andromeda/Graphics/Shader.h"

namespace And
{

  struct ShaderEditorInfo
  {
    ShaderEditorInfo(const std::string& type, const std::string& Source) : Type(type)
    {
      OldShaderEditor.SetLanguageDefinition(TextEditor::LanguageDefinition::GLSL());
      OldShaderEditor.SetShowWhitespaces(false);
      OldShaderEditor.SetTabSize(2);
      OldShaderEditor.SetText(Source);
    }

    std::string Type;
    TextEditor OldShaderEditor;
  };

  struct ShaderTextEditorData
  {
    std::vector<ShaderEditorInfo> OldShaderEditors;
    std::string Path;
  };

  ShaderTextEditor::ShaderTextEditor(const std::string& Title) : EditorWindow(Title), m_Data(new ShaderTextEditorData)
  {
  }

  ShaderTextEditor::~ShaderTextEditor()
  {
  }

  void ShaderTextEditor::Load(const std::string& Path)
  {
    m_Data->OldShaderEditors.clear();
    m_Data->Path = Path;

    Slurp file(Path.c_str());

    if (file.size() > 0)
    {
      bool bEnd = false;

      std::string OldShaderSource(file.data(), file.size());

      while (!bEnd)
      {
        size_t OldShaderStart = OldShaderSource.find("#type");
        size_t OldShaderEnd = OldShaderSource.find("#type", OldShaderStart + 5);

        if (OldShaderEnd == std::string::npos)
        {
          bEnd = true;
          OldShaderEnd = OldShaderSource.size();
        }

        std::string Source = OldShaderSource.substr(OldShaderStart, OldShaderEnd);
        OldShaderSource = OldShaderSource.substr(OldShaderEnd, OldShaderSource.size());

        size_t TypeEnd = Source.find("\n", 5);
        m_Data->OldShaderEditors.emplace_back(Source.substr(OldShaderStart + 6, TypeEnd - 5).c_str(), Source.substr(TypeEnd + 1, Source.size()));
      }
      
    }
    m_is_open = true;
  }

  void ShaderTextEditor::Save()
  {
    std::string OldShaderSource;
    for (auto& OldShader : m_Data->OldShaderEditors)
    {
      OldShaderSource += "#type ";
      OldShaderSource += OldShader.Type + "\n";
      OldShaderSource += OldShader.OldShaderEditor.GetText();
    }

    FILE* f;

    f = fopen(m_Data->Path.c_str(), "wb");

    if (f != NULL) 
    {
      fwrite(OldShaderSource.c_str(), 1, OldShaderSource.size(), f);
      fclose(f);
    }
  }

  void ShaderTextEditor::Show()
  {

    if (m_is_open)
    {
      
      if (ImGui::Begin(m_title.c_str(), &m_is_open, ImGuiWindowFlags_MenuBar))
      {

        if(ImGui::BeginMenuBar()){
          if(ImGui::BeginMenu("File")){
            if(ImGui::MenuItem("Save")){
              printf("\n*** Reload OldShader ***\n");
              Save();
            }
            ImGui::EndMenu();
          }
          ImGui::EndMenuBar();
        }
        if (ImGui::BeginTabBar("OldShaders"))
        {
          for (auto& OldShader : m_Data->OldShaderEditors)
          {
            if (ImGui::BeginTabItem(OldShader.Type.c_str()))
            {
              OldShader.OldShaderEditor.Render(OldShader.Type.c_str());
              ImGui::EndTabItem();
            }
          }

          if (ImGui::TabItemButton("+"))
          {

          }

          ImGui::EndTabBar();
        }

      }
      ImGui::End();
      if (!m_is_open)
      {
        m_Data->Path = "";
        m_Data->OldShaderEditors.clear();
      }
    }
  }
}