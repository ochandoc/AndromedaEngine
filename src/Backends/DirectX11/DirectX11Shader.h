#pragma once

#include "Andromeda/Graphics/Shader.h"
#include "Backends/DirectX11/DirectX11.h"

namespace And
{
  class DirectX11VertexShader;
  class DirectX11PixelShader;

  class DirectX11Shader : public Shader
  {
  private:
    DirectX11Shader();

  public:
    static std::shared_ptr<DirectX11Shader> CreateShared(const std::string& Path);

    virtual void Use() const override {}
    virtual void StopUsing() const override {}

    inline std::shared_ptr<DirectX11VertexShader> GetVertexShader() { return m_VertexShader; }
    inline std::shared_ptr<DirectX11PixelShader> GetPixelShader() { return m_PixelShader; }

  protected:
    std::shared_ptr<DirectX11VertexShader> m_VertexShader;
    std::shared_ptr<DirectX11PixelShader> m_PixelShader;
  };

  class DirectX11VertexShader
  {
  public:
    static std::shared_ptr<DirectX11VertexShader> CreateShared(const std::string& source);

    inline ID3D11InputLayout* GetInputLayout() const { return m_InputLayout.Get(); }
    inline ID3D11VertexShader* GetShader() const { return m_Handle.Get(); }

  private:
    ComPtr<ID3D11InputLayout> m_InputLayout;
    ComPtr<ID3D11VertexShader> m_Handle;
  };

  class DirectX11PixelShader
  {
  public:
    static std::shared_ptr<DirectX11PixelShader> CreateShared(const std::string& source);

    inline ID3D11PixelShader* GetShader() const { return m_Handle.Get(); }

  private:
    ComPtr<ID3D11PixelShader> m_Handle;
  };
}