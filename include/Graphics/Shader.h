#pragma once
#include <iostream>


namespace And{

   enum ShaderType{
    Shader_Vertex,
    Shader_Fragment,
    Shader_Geometry,
    Shader_Teselation,
  };

  struct ShaderInfo{
    ShaderType type;
    // It should be the path of file or todo lo que tiene el archivo cargado en memoria
    std::string file; 
  };

class Shader{
  public:

  Shader();
  Shader(const Shader&) = delete;
  Shader(const Shader&&) = delete;

  virtual ~Shader();

  Shader& operator=(const Shader&) = delete;
  Shader& operator=(const Shader&&) = delete;

  virtual void CreateShader() = 0;
  
  private:

  //virtual unsigned int  CompileShader(const std::string& source, unsigned int type) = 0;

};
}