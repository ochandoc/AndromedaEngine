#include "Andromeda/Graphics/RenderTarget.h"

#include "Backends/OpenGL/OpenGL.h"

namespace And
{
  struct RenderTargetData
  {
    uint32 Id;
    uint32 Texture;
  };
}

And::RenderTarget::RenderTarget(const RenderTargetCreationInfo& InCreationInfo) : m_Data(new RenderTargetData)
{
  glCreateFramebuffers(1, &m_Data->Id);
  glBindFramebuffer(GL_FRAMEBUFFER, m_Data->Id);

  glGenTextures(1, &m_Data->Texture);
  glBindTexture(GL_TEXTURE_2D, m_Data->Texture);

  switch (InCreationInfo.format)
  {
  case ETextureFormat::Depth:
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_STENCIL, );
    break;
  case ETextureFormat::RGBA8:
    break;
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

And::RenderTarget::~RenderTarget()
{
  glDeleteFramebuffers(1, &m_Data->Id);
}
