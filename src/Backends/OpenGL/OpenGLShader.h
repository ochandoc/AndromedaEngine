#pragma once

#include "Common/Shader.h"

namespace And{

class OpenGLShader : public Shader{

public:

  /**
   * @brief Construct a new OpenGL Shader object
   * 
   * @param S_info std::vector of ShaderInfo with all shaders you need
   */
  OpenGLShader(std::vector<ShaderInfo> S_info);
  
  virtual ~OpenGLShader();

  //virtual unsigned int CompileShader(Shader) override;

private:

  unsigned int m_shader_id;

};
}