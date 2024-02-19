#include "Backends/OpenGL/OpenGLRenderTarget.h"

#include "Backends/OpenGL/OpenGLTexture2D.h"
#include "Backends/OpenGL/OpenGL.h"

#include "imgui.h"

namespace And
{
  OpenGLRenderTarget::OpenGLRenderTarget() : m_Id(0)
  {

  }

  OpenGLRenderTarget::OpenGLRenderTarget(ENoInit) : OpenGLRenderTarget()
  {
  }

  OpenGLRenderTarget::~OpenGLRenderTarget()
  {
    m_Textures.clear();

    if (m_Id)
    {
      glDeleteFramebuffers(1, &m_Id);
    }
  }

  std::shared_ptr<OpenGLRenderTarget> OpenGLRenderTarget::Make(const RenderTargetCreationInfo& CreationInfo)
  {
    uint64 Size = CreationInfo.Width * CreationInfo.Height;
    if (Size == 0) return std::shared_ptr<OpenGLRenderTarget>();

    std::shared_ptr<OpenGLRenderTarget> Rendertarget(new OpenGLRenderTarget);

    TextureCreationInfo TexCreationInfo;
    TexCreationInfo.Width = CreationInfo.Width;
    TexCreationInfo.Height = CreationInfo.Height;
    TexCreationInfo.Mipmaps = false;
    for (ETextureFormat Format : CreationInfo.Formats)
    {
      TexCreationInfo.Format = Format;
      Rendertarget->m_Textures.push_back(MakeTexture(TexCreationInfo));
    }

    glGenFramebuffers(1, &Rendertarget->m_Id);
    glBindFramebuffer(GL_FRAMEBUFFER, Rendertarget->m_Id);

    uint32 ColorIndex = 0;
    for (std::shared_ptr<Texture>& Textures : Rendertarget->m_Textures)
    {
      OpenGLTexture2D* OpenGlTexture = dynamic_cast<OpenGLTexture2D*>(Textures.get());
      switch (OpenGlTexture->GetFormat())
      {
      case And::ETextureFormat::RGBA8:
        {
          glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + ColorIndex, GL_TEXTURE_2D, OpenGlTexture->GetId(), 0);
          ++ColorIndex;
        }
        break;
      case And::ETextureFormat::RGB8:
        {
          glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + ColorIndex, GL_TEXTURE_2D, OpenGlTexture->GetId(), 0);
          ++ColorIndex;
        }
        break;
      case And::ETextureFormat::Depth:
        {
          glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, OpenGlTexture->GetId(), 0);
        }
        break;
      }
    }
    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return Rendertarget;
  }

  void OpenGLRenderTarget::Activate() const
  {
    glBindFramebuffer(GL_FRAMEBUFFER, m_Id);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

  void OpenGLRenderTarget::Desactivate() const
  {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  void OpenGLRenderTarget::Resize(uint32 width, uint32 height)
  {
  }
}

/*void And::RenderTarget::Test()
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
}*/


