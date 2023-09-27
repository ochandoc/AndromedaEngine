#pragma once
#include <iostream>

namespace And{

class Shader{
  public:

  Shader();
  Shader(const Shader&) = delete;
  Shader(const Shader&&) = delete;

  virtual ~Shader();

  Shader& operator=(const Shader&) = delete;
  Shader& operator=(const Shader&&) = delete;

  virtual unsigned int  CreateShader(const std::string& vertexShader, const std::string& fragmentShader) = 0;
  virtual unsigned int  CompileShader(const std::string& source, unsigned int type) = 0;

};
}