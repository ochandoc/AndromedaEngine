#include "DirectX11Shader.h"

namespace And
{


  std::shared_ptr<DirectX11VertexShader> DirectX11VertexShader::CreateShared()
  {
    return nullptr;
  }
  std::shared_ptr<DirectX11PixelShader> DirectX11PixelShader::CreateShared()
  {
    return nullptr;
  }
  std::shared_ptr<DirectX11Shader> DirectX11Shader::CreateShared(const std::string& Path)
  {
      return std::shared_ptr<DirectX11Shader>();
  }
}