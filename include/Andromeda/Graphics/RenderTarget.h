#pragma once

#include "Andromeda/Misc/CoreMiscDefines.h"
#include "Andromeda/HAL/Types.h"

namespace And
{
  enum class ETextureFormat
  {
    RGBA8,
    Depth,
  };

  class RenderTarget
  {
  public:
    RenderTarget(uint32 width, uint32 height);
    ~RenderTarget();

    void Bind();
    void Unbind();
    void Resize(uint32 width, uint32 height);

    void Test();

  private:
    std::unique_ptr<struct RenderTargetData> m_Data;
  };
}