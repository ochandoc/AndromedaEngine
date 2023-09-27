#pragma once

#include "Graphics/Shader.h"


namespace And{

class OpenGLShader : public Shader{

public:

  OpenGLShader(std::vector<ShaderInfo> S_info);
  
  virtual ~OpenGLShader();

  virtual void CreateShader() override;
  //virtual unsigned int CompileShader(Shader) override;

private:

  unsigned int m_shader_id;

};
}