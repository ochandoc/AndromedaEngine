#pragma once

#include "Graphics/Shader.h"

namespace And{

class OpenGLShader : public Shader{

public:

  OpenGLShader();
  
  virtual ~OpenGLShader();

  virtual unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) override;
  virtual unsigned int CompileShader(const std::string& source, unsigned int type) override;


};
}