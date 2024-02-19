#pragma once

#include "Andromeda/Misc/CoreMiscDefines.h"
#include "Andromeda/HAL/Types.h"
#include "Andromeda/Graphics/Texture.h"

namespace And
{
  struct RenderTargetCreationInfo
  {
    uint32 Width;
    uint32 Height;
    std::vector<ETextureFormat> Formats;
  };

  class RenderTarget
  {
    NON_COPYABLE_CLASS(RenderTarget);
    NON_MOVABLE_CLASS(RenderTarget);
  public:
    RenderTarget() = default;

    virtual ~RenderTarget() = default;

    virtual void Activate() const = 0;
    virtual void Desactivate() const = 0;
    virtual void Resize(uint32 width, uint32 height) = 0;

    inline const std::vector<std::shared_ptr<Texture>>& GetTextures() const { return m_Textures; }
    inline const RenderTargetCreationInfo& GetCreationInfo() { return m_CretionInfo; }

  protected:
    RenderTargetCreationInfo m_CretionInfo;
    std::vector<std::shared_ptr<Texture>> m_Textures;
  };

  std::shared_ptr<RenderTarget> MakeRenderTarget(const RenderTargetCreationInfo& CreationInfo);

}