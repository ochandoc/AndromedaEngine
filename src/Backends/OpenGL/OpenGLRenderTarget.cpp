#include "Backends/OpenGL/OpenGLRenderTarget.h"

#include "Backends/OpenGL/OpenGLTexture2D.h"
#include "Backends/OpenGL/OpenGL.h"

#include "imgui.h"

namespace And
{
  OpenGLRenderTarget::OpenGLRenderTarget() 
    : m_Id(0), m_ColorBufferId(0), m_DepthBufferId(0)
  {

  }

  OpenGLRenderTarget::OpenGLRenderTarget(ENoInit) : OpenGLRenderTarget()
  {
  }

  OpenGLRenderTarget::~OpenGLRenderTarget()
  {
    m_Textures.clear();

    if (m_ColorBufferId)
    {
      glDeleteRenderbuffers(1, &m_ColorBufferId);
    }

    if (m_DepthBufferId)
    {
      glDeleteRenderbuffers(1, &m_DepthBufferId);
    }

    if (m_Id)
    {
      glDeleteFramebuffers(1, &m_Id);
    }
  }

  std::shared_ptr<OpenGLRenderTarget> OpenGLRenderTarget::Make(const RenderTargetCreationInfo& CreationInfo)
  {
    bool bColorTexture = false;
    bool bDepthTexture = false;

    uint64 Size = CreationInfo.Width * CreationInfo.Height;
    if (Size == 0) return std::shared_ptr<OpenGLRenderTarget>();

    std::shared_ptr<OpenGLRenderTarget> Rendertarget(new OpenGLRenderTarget);

    for (ETextureFormat Format : CreationInfo.Formats)
    {
      uint8 uintFormat = (uint8)Format;
      if (uintFormat >= 0 && uintFormat < 6)
        bColorTexture = true;

      if (Format == ETextureFormat::Depth)
        bDepthTexture = true;
    }

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

    std::vector<uint32> ColotAttachments;
    uint32 ColorIndex = 0;
    for (std::shared_ptr<Texture>& Textures : Rendertarget->m_Textures)
    {
      OpenGLTexture2D* OpenGlTexture = dynamic_cast<OpenGLTexture2D*>(Textures.get());
      switch (OpenGlTexture->GetFormat())
      {
      case And::ETextureFormat::RGB8:
      {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + ColorIndex, GL_TEXTURE_2D, OpenGlTexture->GetId(), 0);
        ColotAttachments.push_back(GL_COLOR_ATTACHMENT0 + ColorIndex);
        ++ColorIndex;
      } break;
      case And::ETextureFormat::RGBA8:
      {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + ColorIndex, GL_TEXTURE_2D, OpenGlTexture->GetId(), 0);
        ColotAttachments.push_back(GL_COLOR_ATTACHMENT0 + ColorIndex);
        ++ColorIndex;
      } break;
      case And::ETextureFormat::RGB16F:
      {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + ColorIndex, GL_TEXTURE_2D, OpenGlTexture->GetId(), 0);
        ColotAttachments.push_back(GL_COLOR_ATTACHMENT0 + ColorIndex);
        ++ColorIndex;
      } break;
      case And::ETextureFormat::RGBA16F:
      {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + ColorIndex, GL_TEXTURE_2D, OpenGlTexture->GetId(), 0);
        ColotAttachments.push_back(GL_COLOR_ATTACHMENT0 + ColorIndex);
        ++ColorIndex;
      } break;
      case And::ETextureFormat::RGB32F:
      {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + ColorIndex, GL_TEXTURE_2D, OpenGlTexture->GetId(), 0);
        ColotAttachments.push_back(GL_COLOR_ATTACHMENT0 + ColorIndex);
        ++ColorIndex;
      } break;
      case And::ETextureFormat::RGBA32F:
      {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + ColorIndex, GL_TEXTURE_2D, OpenGlTexture->GetId(), 0);
        ColotAttachments.push_back(GL_COLOR_ATTACHMENT0 + ColorIndex);
        ++ColorIndex;
      } break;
      case And::ETextureFormat::Depth:
      {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, OpenGlTexture->GetId(), 0);
      }
      break;
      }
    }

    glDrawBuffers((int)ColotAttachments.size(), ColotAttachments.data());
    
    if (!bColorTexture)
    {
      glGenRenderbuffers(1, &Rendertarget->m_ColorBufferId);
      glBindRenderbuffer(GL_RENDERBUFFER, Rendertarget->m_ColorBufferId);
      glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, CreationInfo.Width, CreationInfo.Height);
      glBindRenderbuffer(GL_RENDERBUFFER, 0);
      glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, Rendertarget->m_ColorBufferId);
    }

    if (!bDepthTexture)
    {
      glGenRenderbuffers(1, &Rendertarget->m_DepthBufferId);
      glBindRenderbuffer(GL_RENDERBUFFER, Rendertarget->m_DepthBufferId);
      glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, CreationInfo.Width, CreationInfo.Height);
      glBindRenderbuffer(GL_RENDERBUFFER, 0);
      glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, Rendertarget->m_DepthBufferId);
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
