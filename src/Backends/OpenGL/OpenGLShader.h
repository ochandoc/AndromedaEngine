#pragma once

#include "Graphics/Shader.h"

namespace And{

class OpenGLShader : public Shader{

public:

  // Recibe un vector de ShaderInfo con todos los shaders que se van a utilizar
  OpenGLShader(std::vector<ShaderInfo> S_info);
  
  virtual ~OpenGLShader();

  virtual void CreateShader() override;

  //virtual unsigned int CompileShader(Shader) override;

private:

  unsigned int m_shader_id;

};
}