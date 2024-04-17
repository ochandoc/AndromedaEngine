#include "DirectX11SkyboxTexture.h"

#include "Backends/DirectX11/DirectX11Renderer.h"

#include "stb_image.h"

namespace And
{
  struct ImageData
  {
    int width;
    int height;
    uint8* data;
  };

  DirectX11SkyboxTexture::DirectX11SkyboxTexture() {}

  std::shared_ptr<DirectX11SkyboxTexture> And::DirectX11SkyboxTexture::CreateShared(const std::vector<std::string>& paths)
  {
    ID3D11Device* device = DirectX11Renderer::GetDevice();

    if (!device) return nullptr;
    if (paths.size() != 6) return nullptr;

    
    std::vector<ImageData> image_datas;
    for (const std::string& path : paths)
    {
      int width;
      int height;
      int channels;
      stbi_set_flip_vertically_on_load_thread(0);
      uint8* pixels = stbi_load(path.c_str(), &width, &height, &channels, 4);

      if (pixels)
      {
        ImageData data;
        data.width = width;
        data.height = height;
        data.data = pixels;
        image_datas.push_back(data);
      }
    }

    if (image_datas.size() != 6)
    {
      for (ImageData& img : image_datas)
      {
        stbi_image_free(img.data);
      }
      return nullptr;
    }

    ComPtr<ID3D11Texture2D> texture;
    ComPtr<ID3D11ShaderResourceView> view;
    ComPtr<ID3D11SamplerState> samplerState;

    HRESULT result = S_OK;

    D3D11_TEXTURE2D_DESC TexDesc = {
      .Width = (uint32)image_datas.front().width,
      .Height = (uint32)image_datas.front().height,
      .MipLevels = 1,
      .ArraySize = 6,
      .Format = DXGI_FORMAT_R8G8B8A8_UNORM,
      .SampleDesc = { .Count = 1, .Quality = 0 },
      .Usage = D3D11_USAGE_DEFAULT,
      .BindFlags = D3D11_BIND_SHADER_RESOURCE,
      .CPUAccessFlags = 0,
      .MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE
    };

    D3D11_SUBRESOURCE_DATA data[6] = {};
    for (int i = 0; i < 6; ++i)
    {
      data[i].pSysMem = image_datas[i].data;
      data[i].SysMemPitch = image_datas[i].width * 4u;
      data[i].SysMemSlicePitch = 0;
    }
    
    result = device->CreateTexture2D(&TexDesc, data, texture.GetAddressOf());

    for (ImageData& img : image_datas)
    {
      stbi_image_free(img.data);
    }

    if (FAILED(result)) return nullptr;

    D3D11_SHADER_RESOURCE_VIEW_DESC ViewDesc = {
      .Format = TexDesc.Format,
      .ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE,
      .TextureCube = { .MostDetailedMip = 0, .MipLevels = 1}
    };

    result = device->CreateShaderResourceView(texture.Get(), &ViewDesc, view.GetAddressOf());

    if (FAILED(result)) return nullptr;

    D3D11_SAMPLER_DESC SamplerDesc = {
      .Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR,
      .AddressU = D3D11_TEXTURE_ADDRESS_WRAP,
      .AddressV = D3D11_TEXTURE_ADDRESS_WRAP,
      .AddressW = D3D11_TEXTURE_ADDRESS_WRAP,
      .MipLODBias = 0.0f,
      .MaxAnisotropy = 1,
      .ComparisonFunc = D3D11_COMPARISON_NEVER,
      .BorderColor = { 0.0f, 0.0f, 0.0f, 0.0f },
      .MinLOD = -D3D11_FLOAT32_MAX,
      .MaxLOD = D3D11_FLOAT32_MAX,
    };

    result = device->CreateSamplerState(&SamplerDesc, samplerState.GetAddressOf());

    if (FAILED(result)) return nullptr;

    std::shared_ptr<DirectX11SkyboxTexture> SkyboxTexture(new DirectX11SkyboxTexture);

    SkyboxTexture->m_Handle = texture;
    SkyboxTexture->m_Sampler = samplerState;
    SkyboxTexture->m_View = view;

    return SkyboxTexture;
  }
}

