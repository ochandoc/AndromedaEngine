#include "Andromeda/Graphics/RenderTarget.h"

#include "Backends/OpenGL/OpenGL.h"

#include "imgui.h"

namespace And
{
  struct RenderTargetTextureData
  {
    ETextureFormat Format;
    uint32 Id;
  };

  struct RenderTargetData
  {
    uint32 Id;
    std::vector<RenderTargetTextureData> Textures;
  };
}


And::RenderTarget::RenderTarget(uint32 width, uint32 height, const std::vector<ETextureFormat>& TextureFormats) : m_Data(new RenderTargetData)
{
  for (ETextureFormat Format : TextureFormats)
  {
    RenderTargetTextureData& TextureData = m_Data->Textures.emplace_back();
    TextureData.Format = Format;

    switch (Format)
    {
    case And::ETextureFormat::RGBA8:
      {
        glGenTextures(1, &TextureData.Id);
        glBindTexture(GL_TEXTURE_2D, TextureData.Id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

        glBindTexture(GL_TEXTURE_2D, 0);
      }
      break;
    case And::ETextureFormat::Depth:
      {
        glGenTextures(1, &TextureData.Id);
        glBindTexture(GL_TEXTURE_2D, TextureData.Id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

        glBindTexture(GL_TEXTURE_2D, 0);
      }
      break;
    }
  }

  glGenFramebuffers(1, &m_Data->Id);
  glBindFramebuffer(GL_FRAMEBUFFER, m_Data->Id);

  for (RenderTargetTextureData& Textures : m_Data->Textures)
  {
    switch (Textures.Format)
    {
    case And::ETextureFormat::RGBA8:
    {
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Textures.Id, 0);
    }
    break;
    case And::ETextureFormat::Depth:
    {
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, Textures.Id, 0);
    }
    break;
    }
  }  
  assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

And::RenderTarget::~RenderTarget()
{
  for (RenderTargetTextureData& Texture : m_Data->Textures)
  {
    glDeleteTextures(1, &Texture.Id);
  }
  glDeleteFramebuffers(1, &m_Data->Id);
}

void And::RenderTarget::Bind()
{
  glBindFramebuffer(GL_FRAMEBUFFER, m_Data->Id);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void And::RenderTarget::Unbind()
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void And::RenderTarget::Resize(uint32 width, uint32 height)
{
}

void And::RenderTarget::Test()
{
  ImGuiStyle& style = ImGui::GetStyle();
  ImVec2 padding = style.WindowPadding;
  style.WindowPadding = ImVec2(0.0f, 0.0f);
  static ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoScrollbar;
  windowFlags |= ImGuiWindowFlags_NoCollapse;

  std::string WindowName;
  for (RenderTargetTextureData& Texture : m_Data->Textures)
  {
    WindowName = "Render Target" + std::to_string(Texture.Id);

    if (ImGui::Begin(WindowName.c_str(), nullptr, windowFlags))
    {
      ImGui::Image((void*)(intptr_t)Texture.Id, ImGui::GetWindowSize(), ImVec2(0, 1), ImVec2(1, 0));
    }
    ImGui::End();
  };

  style.WindowPadding = padding;
}


