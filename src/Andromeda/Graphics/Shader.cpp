#include "Andromeda/Graphics/Shader.h"

#include "Backends/OpenGL/OpenGLShader.h"

namespace And
{
  std::shared_ptr<Shader> And::MakeShader(const std::string& Path)
  {
    return OpenGLShader::Make(Path);
  }
}

