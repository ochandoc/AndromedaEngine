#include "OpenGLShader.h"

#include <vector>
#include "GL/glew.h"

namespace And{

  OpenGLShader::OpenGLShader(std::vector<ShaderInfo> S_info){

    unsigned int program = glCreateProgram();
    unsigned int id;
    
    for(ShaderInfo& shader : S_info){
      switch (shader.type){
      case Shader_Vertex:
        id = glCreateShader(GL_VERTEX_SHADER);
      break;
      case Shader_Fragment:
        id = glCreateShader(GL_FRAGMENT_SHADER);
      break;
      case Shader_Geometry:
        
      break;
      case Shader_Teselation:
        
      break;
      
      default:
      break;
      }


      // Compilamos todos los shaders y hacemos el attach
      const char* src = shader.file.c_str();
      glShaderSource(id, 1, &src, nullptr);
      glCompileShader(id);

      glAttachShader(program, id);
    }

    // Cuando ya tenemos todos los shader compilados, linkamos el program
    glLinkProgram(program);
    // glValidateProgram(program);
   
    // glDeleteShader(shader)

  }

  OpenGLShader::~OpenGLShader(){

  }

  void OpenGLShader::CreateShader(){

  }

  

}