#include "Andromeda/Editor/ShaderTextEditor.h"

#include "TextEditor.h"
#include "Andromeda/Misc/Threw.h"
#include "Andromeda/Resources/ResourceManager.h"
#include "Andromeda/Misc/Slurp.h"
#include "Andromeda/Graphics/Shader.h"

namespace And
{

  struct ShaderEditorInfo
  {
    ShaderEditorInfo(const std::string& type, const std::string& Source) : Type(type)
    {
      shaderEditor.SetLanguageDefinition(TextEditor::LanguageDefinition::GLSL());
      shaderEditor.SetShowWhitespaces(false);
      shaderEditor.SetTabSize(2);
      shaderEditor.SetText(Source);
    }

    std::string Type;
    TextEditor shaderEditor;
  };

  struct ShaderTextEditorData
  {
    std::vector<ShaderEditorInfo> shaderEditors;
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
    m_Data->shaderEditors.clear();
    m_Data->Path = Path;

    Slurp file(Path.c_str());

    if (file.size() > 0)
    {
      bool bEnd = false;

      std::string ShaderSource(file.data(), file.size());

      while (!bEnd)
      {
        size_t ShaderStart = ShaderSource.find("#type");
        size_t ShaderEnd = ShaderSource.find("#type", ShaderStart + 5);

        if (ShaderEnd == std::string::npos)
        {
          bEnd = true;
          ShaderEnd = ShaderSource.size();
        }

        std::string Source = ShaderSource.substr(ShaderStart, ShaderEnd);
        ShaderSource = ShaderSource.substr(ShaderEnd, ShaderSource.size());

        size_t TypeEnd = Source.find("\n", 5);
        m_Data->shaderEditors.emplace_back(Source.substr(ShaderStart + 6, TypeEnd - 5).c_str(), Source.substr(TypeEnd + 1, Source.size()));
      }
      
    }
    m_is_open = true;
  }

  void ShaderTextEditor::Save()
  {
    std::string ShaderSource;
    for (auto& shader : m_Data->shaderEditors)
    {
      ShaderSource += "#type ";
      ShaderSource += shader.Type + "\n";
      ShaderSource += shader.shaderEditor.GetText();
    }

    FILE* f;

    f = fopen(m_Data->Path.c_str(), "wb");

    if (f != NULL) 
    {
      fwrite(ShaderSource.c_str(), 1, ShaderSource.size(), f);
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
              printf("\n*** Reload shader ***\n");
              Save();
              // ReloadResource
              if(m_resourceManager){
                m_resourceManager->ReloadResource<Shader>(m_Data->Path);
              }
            }
            ImGui::EndMenu();
          }
          ImGui::EndMenuBar();
        }
        if (ImGui::BeginTabBar("Shaders"))
        {
          for (auto& shader : m_Data->shaderEditors)
          {
            if (ImGui::BeginTabItem(shader.Type.c_str()))
            {
              shader.shaderEditor.Render(shader.Type.c_str());
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
        m_Data->shaderEditors.clear();
      }
    }
  }
}