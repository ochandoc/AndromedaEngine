#include "Andromeda/Graphics/Shader.h"

#include "Andromeda/HAL/Window.h"
#include "Backends/OpenGL/OpenGLShader.h"
#include "Backends/DirectX11/DirectX11Shader.h"

namespace And
{
  std::shared_ptr<Shader> And::MakeShader(const std::string& Path)
  {
    switch (GetGraphicApiType())
    {
    case EGraphicsApiType::OpenGL:
      return OpenGLShader::Make(Path);
      break;
    case EGraphicsApiType::DirectX11:
      return DirectX11Shader::CreateShared();
      break;
    }
  }
}

