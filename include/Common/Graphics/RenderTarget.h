#pragma once

#include "base.h"

namespace And
{
  enum class ETextureFormat
  {
    RGBA8,
    Depth,
  };

  struct RenderTargetCreationInfo
  {
    uint32 width, height;
    ETextureFormat format;
  };

  class RenderTarget
  {
  public:
    RenderTarget(const RenderTargetCreationInfo& InCreationInfo);
    ~RenderTarget();

    void Bind();
    void Unbind();
    void Resize(uint32 width, uint32 height);

    void Test();

  private:
    std::unique_ptr<struct RenderTargetData> m_Data;
  };
}