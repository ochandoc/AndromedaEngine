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

  private:
    std::unique_ptr<struct RenderTargetData> m_Data;
  };
}