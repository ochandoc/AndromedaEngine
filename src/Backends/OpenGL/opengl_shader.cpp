#include "Common/Shader.h"

#include "Common/Slurp.h"

#include "GL/glew.h"



namespace And{

  struct ShaderData
  {

  };

  Shader::Shader(std::vector<ShaderInfo> S_info) : m_Data(new ShaderData){

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


      // Compilamos todos los shaders y hacemos el attach del program al shader
      //const char* src = shader.file.c_str();
      //printf("%s",src);

      Slurp file{shader.file_path};
      char *shader_data = file.data();
      glShaderSource(id, 1, &shader_data, nullptr);
      glCompileShader(id);
      glAttachShader(program, id);
    }

    // Cuando ya tenemos todos los shader compilados, linkamos el program
    glLinkProgram(program);
    glValidateProgram(program);

    int succes;
    glGetProgramiv(program, GL_VALIDATE_STATUS, &succes);
    //printf("%d", succes);

    glUseProgram(program);
   
    // glDeleteShader(shader)

  }

  Shader::~Shader(){
      
  }

  

}