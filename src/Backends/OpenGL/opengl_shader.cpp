#include "Common/Shader.h"

#include "Common/Slurp.h"

#include "GL/glew.h"



namespace And{

  struct ShaderData
  {
    unsigned int id;
  };

  char* Shader::get_upload_shader_error(){
    return m_shader_error;
  }

  void Shader::upload_shader(ShaderType type, const char* path){

    unsigned int id;

    switch (type){
    case Shader_Vertex:
      id = glCreateShader(GL_VERTEX_SHADER);  
    break;
    case Shader_Fragment:
      id = glCreateShader(GL_FRAGMENT_SHADER);  
    break;
    
    default:
      break;
    }

    // Cargamos en memoria el archivo del shader
    Slurp file{path};
    char *shader_data = file.data();
    glShaderSource(id, 1, &shader_data, nullptr);

    // Compilamos
    glCompileShader(id);

    // Recogemos codigo de error en caso de que lo haya
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if(result == GL_FALSE){
      int lenght;
      glGetShaderiv(id, GL_INFO_LOG_LENGTH, &lenght);
      glGetShaderInfoLog(id, lenght, &lenght, m_shader_error);
    }else{
      // Si no hay error atachamos
      glAttachShader(m_Data->id, id);
    }

  }

  int Shader::link_shaders(){

    // Cuando ya tenemos todos los shader compilados, linkamos el program
    glLinkProgram(m_Data->id);
    glValidateProgram(m_Data->id);

    int succes;
    glGetProgramiv(m_Data->id, GL_VALIDATE_STATUS, &succes);

    return succes;
  }




  Shader::Shader() : m_Data(new ShaderData){

    m_Data->id = glCreateProgram();
  }

  void Shader::use()
  {
    glUseProgram(m_Data->id);
  }

  Shader::~Shader(){
    glDeleteProgram(m_Data->id);
  }

  

}