#include "Backends/DirectX11/DirectX11Texture2D.h"

#include "Backends/DirectX11/DirectX11Renderer.h"
 
#include "stb_image.h"

namespace And
{
  DirectX11Texture2D::DirectX11Texture2D() {}

  std::shared_ptr<DirectX11Texture2D> DirectX11Texture2D::CreateShared(const std::string& path)
  {
    ID3D11Device* device = DirectX11Renderer::GetDevice();

    if (!device) return nullptr;

    int width;
    int height;
    int channels;
    stbi_set_flip_vertically_on_load_thread(1);
    uint8* pixels = stbi_load(path.c_str(), &width, &height, &channels, 4);

    if (pixels)
    {
      D3D11_TEXTURE2D_DESC TexDesc = {
        .Width = (uint32)width,
        .Height = (uint32)height,
        .MipLevels = 1,
        .ArraySize = 1,
        .Format = DXGI_FORMAT_R8G8B8A8_UNORM,
        .SampleDesc = { .Count = 1, .Quality = 0 },
        .Usage = D3D11_USAGE_DEFAULT,
        .BindFlags = D3D11_BIND_SHADER_RESOURCE,
        .CPUAccessFlags = 0,
        .MiscFlags = 0
      };
      
      D3D11_SUBRESOURCE_DATA Data = {
        .pSysMem = pixels,
        .SysMemPitch = (uint32)width * 4u,
        .SysMemSlicePitch = 0,
      };

      ComPtr<ID3D11Texture2D> Tex;
      ComPtr<ID3D11ShaderResourceView> View;
      ComPtr<ID3D11SamplerState> Sampler;

      HRESULT result = S_OK;

      result = device->CreateTexture2D(&TexDesc, &Data, Tex.GetAddressOf());

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

      result = device->CreateSamplerState(&SamplerDesc, Sampler.GetAddressOf());

      D3D11_SHADER_RESOURCE_VIEW_DESC ViewDesc = {
        .Format = TexDesc.Format,
        .ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D,
        .Texture2D = { .MostDetailedMip = 0, .MipLevels = 1 }
      };

      result = device->CreateShaderResourceView(Tex.Get(), &ViewDesc, View.GetAddressOf());

      stbi_image_free(pixels);

      std::shared_ptr<DirectX11Texture2D> texture(new DirectX11Texture2D);
      texture->m_Handle = Tex;
      texture->m_View = View;
      texture->m_Sampler = Sampler;
      texture->m_CreationInfo.Width = width;
      texture->m_CreationInfo.Height = height;
      texture->m_CreationInfo.Mipmaps = false;
      texture->m_CreationInfo.Format = ETextureFormat::RGBA8;
      texture->m_LoadedFromFile = true;
      texture->m_Path = path;

      return texture;
    }

    return std::shared_ptr<DirectX11Texture2D>();
  }
  bool DirectX11Texture2D::IsLoadedFromFile() const
  {
    return m_LoadedFromFile;
  }
  const char* DirectX11Texture2D::GetPath() const
  {
    if (!m_LoadedFromFile) return nullptr;
    return m_Path.c_str();
  }
}

