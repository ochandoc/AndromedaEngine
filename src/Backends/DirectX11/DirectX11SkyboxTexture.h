#pragma once

#include "Andromeda/Graphics/Texture.h"

#include"Backends/DirectX11/DirectX11.h"

namespace And
{
  class DirectX11SkyboxTexture : public SkyboxTexture
  {
  private:
    DirectX11SkyboxTexture();

  public:
    static std::shared_ptr<DirectX11SkyboxTexture> CreateShared(const std::vector<std::string>& paths);

    inline ID3D11Texture2D* GetTexture() const { return m_Handle.Get(); }
    inline ID3D11ShaderResourceView* GetView() const { return m_View.Get(); }
    inline ID3D11SamplerState* GetSampler() const { return m_Sampler.Get(); }

  private:
    ComPtr<ID3D11Texture2D> m_Handle;
    ComPtr<ID3D11ShaderResourceView> m_View;
    ComPtr<ID3D11SamplerState> m_Sampler;
  };
}