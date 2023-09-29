#include "Common/Shader.h"

#include "Common/Slurp.h"

#include "GL/glew.h"



namespace And{

  struct ShaderData
  {
    unsigned int id;
  };

  Shader::Shader(std::vector<ShaderInfo> S_info) : m_Data(new ShaderData)
  {
    m_Data->id = glCreateProgram();
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

      Slurp file{shader.file_path};
      char *shader_data = file.data();
      glShaderSource(id, 1, &shader_data, nullptr);
      glCompileShader(id);

      // Get compile error
      int result;
      glGetShaderiv(id, GL_COMPILE_STATUS, &result);
      if(result == GL_FALSE){
        int lenght;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &lenght);
        char *msg = new char[lenght];

        glGetShaderInfoLog(id, lenght, &lenght, msg);

        printf("\nFailed to compile ");
        
        switch (shader.type){
          case Shader_Vertex: printf("vertex shader ");break;
          case Shader_Fragment: printf("fragment shader ");break;
        }

        printf("%s\n", msg);
      }


      glAttachShader(m_Data->id, id);
    }

    // Cuando ya tenemos todos los shader compilados, linkamos el program
    glLinkProgram(m_Data->id);
    glValidateProgram(m_Data->id);

    int succes;
    glGetProgramiv(m_Data->id, GL_VALIDATE_STATUS, &succes);
    //printf("%d", succes);


   
    // glDeleteShader(shader)

  }

  void Shader::use()
  {
    glUseProgram(m_Data->id);
  }

  Shader::~Shader(){
    glDeleteProgram(m_Data->id);
  }

  

}