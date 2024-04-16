#include "DirectX11Shader.h"

#include "Backends/DirectX11/DirectX11ShaderPreProcessor.h"
#include "Backends/DirectX11/DirectX11Renderer.h"

namespace And
{

#pragma region DIRECTX11_SHADER

  DirectX11Shader::DirectX11Shader() {}

  std::shared_ptr<DirectX11Shader> DirectX11Shader::CreateShared(const std::string& Path)
  {
    std::string allSources = DirectX11ShaderPreProcessor::LoadSourceFromFile(Path);

    if (allSources.size() == 0) return nullptr;

    std::unordered_map<EDirectX11ShaderType, std::string> sources = DirectX11ShaderPreProcessor::SplitSource(allSources);

    if (!sources.contains(EDirectX11ShaderType::Vertex)) return nullptr;
    if (!sources.contains(EDirectX11ShaderType::Pixel)) return nullptr;

    if (sources.contains(EDirectX11ShaderType::None))
    {
      // TODO (och): print an error
      return nullptr;
    }

    std::shared_ptr<DirectX11VertexShader> VertexShader = DirectX11VertexShader::CreateShared(sources[EDirectX11ShaderType::Vertex]);
    std::shared_ptr<DirectX11PixelShader> PixelShader = DirectX11PixelShader::CreateShared(sources[EDirectX11ShaderType::Pixel]);

    if (!VertexShader && !PixelShader) return nullptr;
 
    std::shared_ptr<DirectX11Shader> shader(new DirectX11Shader);

    shader->m_VertexShader = VertexShader;
    shader->m_PixelShader = PixelShader;

    return shader;
  }

#pragma endregion

#pragma region DIRECTX11_VERTEX_SHADER

  std::shared_ptr<DirectX11VertexShader> DirectX11VertexShader::CreateShared(const std::string& source)
  {
    ID3D11Device* device = DirectX11Renderer::GetDevice();

    if (!device) return nullptr;

    ComPtr<ID3DBlob> CompiledCode;
    ComPtr<ID3DBlob> ErrorBuff;

    HRESULT result = D3DCompile(
      (void*)source.data(),
      source.size(),
      NULL,
      NULL,
      NULL,
      "main",
      "vs_5_0",
      0,
      0,
      CompiledCode.GetAddressOf(),
      ErrorBuff.GetAddressOf()
    );

    if (FAILED(result))
    {
      std::string err = DirectX11Tools::BlobToString(ErrorBuff);
      printf("Error: %s\n", err.c_str());
      return nullptr;
    }

    constexpr D3D11_INPUT_ELEMENT_DESC LayoutDescriptor[] = {
      {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
      {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
      {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };


    ComPtr<ID3D11VertexShader> shader;
    ComPtr<ID3D11InputLayout> inputLayout;

    result = device->CreateInputLayout(
      LayoutDescriptor,
      3,
      CompiledCode->GetBufferPointer(),
      CompiledCode->GetBufferSize(),
      inputLayout.GetAddressOf()
    );

    if (FAILED(result)) return nullptr;

    result = device->CreateVertexShader(
      CompiledCode->GetBufferPointer(),
      CompiledCode->GetBufferSize(),
      NULL,
      shader.GetAddressOf()
    );

    if (FAILED(result)) return nullptr;

    std::shared_ptr<DirectX11VertexShader> s(new DirectX11VertexShader);
    s->m_Handle = shader;
    s->m_InputLayout = inputLayout;

    return s;
  }

#pragma endregion

#pragma region DIRECTX11_PIXEL_SHADER

  std::shared_ptr<DirectX11PixelShader> DirectX11PixelShader::CreateShared(const std::string& source)
  {
    ID3D11Device* device = DirectX11Renderer::GetDevice();

    ComPtr<ID3DBlob> CompiledCode;
    ComPtr<ID3DBlob> ErrorBuff;
    
    HRESULT result = D3DCompile(
      (void*)source.data(),
      source.size(),
      NULL,
      NULL,
      NULL,
      "main",
      "ps_5_0",
      0,
      0,
      CompiledCode.GetAddressOf(),
      ErrorBuff.GetAddressOf()
    );
    
    if (FAILED(result))
    {
      std::string err = DirectX11Tools::BlobToString(ErrorBuff);
      printf("Error: %s\n", err.c_str());
      return nullptr;
    }

    ComPtr<ID3D11PixelShader> shader;
    result = device->CreatePixelShader(
      CompiledCode->GetBufferPointer(),
      CompiledCode->GetBufferSize(),
      NULL,
      shader.GetAddressOf()
      );

    if (FAILED(result)) return nullptr;

    std::shared_ptr<DirectX11PixelShader> s(new DirectX11PixelShader);
    s->m_Handle = shader;
    
    return s;
  }

#pragma endregion
}