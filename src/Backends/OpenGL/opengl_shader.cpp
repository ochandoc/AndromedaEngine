#include "Common/Shader.h"

#include "Common/Slurp.h"

#include "Backends/OpenGL/OpenGL.h"
#include "andpch.hpp"


namespace And{

  struct ShaderData{
    unsigned int id;
    ShaderInfo shader_info;
  };

/*
  Shader::Shader(const Shader& other) : Shader(){
    m_Data->id = other.m_Data->id;
  }
*/

  Shader::Shader(Shader&& other) : Shader(){
    m_Data->id = other.m_Data->id;
    m_Data->shader_info = other.m_Data->shader_info;
    other.m_Data->id = 0;
  }

  /*
  Shader& Shader::operator=(const Shader& other){
    m_Data->id = other.m_Data->id; 

    return *this;
  }
  */

  Shader& Shader::operator=(Shader&& other){
    m_Data->id = other.m_Data->id;
    m_Data->shader_info = other.m_Data->shader_info;
    other.m_Data->id = 0;

    return *this;
  }


  // Returns true if gets an error
  bool GetShaderError(unsigned int id){
    // Recogemos codigo de error en caso de que lo haya
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    // Fallo en la compilacion
    if(result == GL_FALSE){
      int lenght;
      char shader_error[1024];
      glGetShaderiv(id, GL_INFO_LOG_LENGTH, &lenght);
      glGetShaderInfoLog(id, lenght, &lenght, shader_error);
      printf("%s\n",shader_error);
      return true;
    }

    return false;
  }

  std::optional<Shader> Shader::make(ShaderInfo s_info){

    unsigned int id_program = glCreateProgram();
    // Error
    if(id_program == 0){
        //return std::nullopt;
      return std::nullopt;
    }

    const char* paths[4] = {s_info.path_vertex, s_info.path_fragment, s_info.path_geometry, s_info.path_teselation};

    for(int i = 0; i < 4; i++){

      unsigned int id_shader;
      // Compilamos fragment shader
      if(paths[i] != nullptr){
        switch(i){
          case 0: id_shader = glCreateShader(GL_VERTEX_SHADER); break;
          case 1: id_shader = glCreateShader(GL_FRAGMENT_SHADER); break;
          case 2: id_shader = glCreateShader(GL_GEOMETRY_SHADER); break;
          case 3: id_shader = glCreateShader(GL_TESS_CONTROL_SHADER); break;
        }
        
        // Cargamos en memoria el archivo del shader
        Slurp file{paths[i]};
        char *shader_data = file.data();
        glShaderSource(id_shader, 1, &shader_data, nullptr);

        // Compilamos
        glCompileShader(id_shader);

        if(!GetShaderError(id_shader)){
          // Si no hay error atachamos
          glAttachShader(id_program, id_shader);
        }else{
          // Error
          return std::nullopt;
        }
      }
    }


    // Cuando ya tenemos todos los shader compilados, linkamos el program
    glLinkProgram(id_program);
    glValidateProgram(id_program);

    int succes;
    glGetProgramiv(id_program, GL_VALIDATE_STATUS, &succes);
    if(succes != GL_TRUE){
      return std::nullopt;
    }


    // Llegados hasta aqui, todo ha ido bien y creamos el shader
    //ShaderData data = {id_program};
    Shader s;
    s.m_Data->id = id_program;
    s.m_Data->shader_info = s_info;
    return std::optional<Shader>(std::move(s));
  }

  void Shader::setMat4(std::string name, const float matrix[16]){ 

    /*for(int i = 0; i < 16; i++){
      printf("%f ", matrix[i]);
    }*/
    glUniformMatrix4fv(glGetUniformLocation(m_Data->id, name.c_str()), 1, GL_FALSE, &matrix[0]);
  }

  void Shader::setVec3(std::string name, const float vector[9]){

    glUniform3fv(glGetUniformLocation(m_Data->id, name.c_str()),1, &vector[0]);
  }


  Shader::Shader() : m_Data(new ShaderData){

  }

  void Shader::use(){
    glUseProgram(m_Data->id);
  }

  Shader::~Shader(){
    glDeleteProgram(m_Data->id);
  }

  

}