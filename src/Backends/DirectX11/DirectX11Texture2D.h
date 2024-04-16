#pragma once

#include "Andromeda/Graphics/Texture.h"

#include "Backends/DirectX11/DirectX11.h"

namespace And
{
  class DirectX11Texture2D : public Texture
  {
  private:
    DirectX11Texture2D();

  public:
    static std::shared_ptr<DirectX11Texture2D> CreateShared(const std::string& path);

    virtual void UploadData(void* Data, uint64 Size) override {}
    virtual bool IsLoadedFromFile() const override;
    virtual const char* GetPath() const override;

    inline ID3D11Texture2D* GetTexture() const { return m_Handle.Get(); }
    inline ID3D11ShaderResourceView* GetView() const { return m_View.Get(); }
    inline ID3D11SamplerState* GetSampler() const { return m_Sampler.Get(); }

  private:
    ComPtr<ID3D11Texture2D> m_Handle;
    ComPtr<ID3D11ShaderResourceView> m_View;
    ComPtr<ID3D11SamplerState> m_Sampler;
    bool m_LoadedFromFile;
    std::string m_Path;
  };
}