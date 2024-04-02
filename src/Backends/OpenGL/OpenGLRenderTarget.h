#pragma once

#include "Andromeda/Graphics/RenderTarget.h"

namespace And
{
  class OpenGLRenderTarget : public RenderTarget
  {
  private:
    OpenGLRenderTarget();

  public:
    OpenGLRenderTarget(ENoInit);

    virtual ~OpenGLRenderTarget();

    static std::shared_ptr<OpenGLRenderTarget> Make(const RenderTargetCreationInfo& CreationInfo);

    virtual void Activate() const override;
    virtual void Desactivate() const override;
    virtual void Resize(uint32 width, uint32 height) override;

    inline uint32 GetId() const { return m_Id; }
  private:
    uint32 m_Id;
    uint32 m_ColorBufferId;
    uint32 m_DepthBufferId;
  };
}