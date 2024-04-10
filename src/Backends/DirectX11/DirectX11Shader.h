#pragma once

#include "Andromeda/Graphics/Shader.h"
#include "Backends/DirectX11/DirectX11.h"

namespace And
{
  class DirectX11Shader : public Shader
  {
  public:
    static std::shared_ptr<DirectX11Shader> CreateShared(const std::string& Path);
  };

  class DirectX11VertexShader
  {
  public:
    static std::shared_ptr<DirectX11VertexShader> CreateShared();

    inline ID3D11InputLayout* GetInputLayout() { return m_InputLayout.Get(); }
    inline ID3D11VertexShader* GetShader() { return m_Handle.Get(); }

  private:
    ComPtr<ID3D11InputLayout> m_InputLayout;
    ComPtr<ID3D11VertexShader> m_Handle;
  };

  class DirectX11PixelShader
  {
  public:
    static std::shared_ptr<DirectX11PixelShader> CreateShared();

    inline ID3D11PixelShader* GetShader() { return m_Handle.Get(); }

  private:
    ComPtr<ID3D11PixelShader> m_Handle;
  };
}