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

  std::shared_ptr<Shader> Shader::make(const std::string& path){

    unsigned int id_program = glCreateProgram();
    // Error
    if(id_program == 0){
        //return std::nullopt;
      return nullptr;
    }

   //const char* paths[4] = {s_info.path_vertex, s_info.path_fragment, s_info.path_geometry, s_info.path_teselation};

    Slurp file{path.c_str()};
    std::string shaders{file.data(), file.size()};

    int vertex_pos = shaders.find("#type Vertex");
    int fragment_pos = shaders.find("#type Fragment");

    std::string vertex_shader;
    std::string fragment_shader;

    if(vertex_pos != std::string::npos && fragment_pos != std::string::npos){
      vertex_shader = shaders.substr(vertex_pos, fragment_pos);
      fragment_shader = shaders.substr(fragment_pos, shaders.size() - 1);

      std::string vertex_title = "#type Vertex";
      std::string fragment_title = "#type Fragment";

      vertex_shader.erase(0, vertex_title.size());
      fragment_shader.erase(0, fragment_title.size());

      printf("Vertex content %s\n Fragment content %s\n", vertex_shader.c_str(), fragment_shader.c_str());

      unsigned int id_vertex_shader = glCreateShader(GL_VERTEX_SHADER);
      unsigned int id_fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

      const char* aux_v = vertex_shader.c_str();
      const char* aux_f = fragment_shader.c_str();

      glShaderSource(id_vertex_shader, 1, &aux_v, nullptr);
      glShaderSource(id_fragment_shader, 1, &aux_f, nullptr);

      // Compilamos
      glCompileShader(id_vertex_shader);
      glCompileShader(id_fragment_shader);

      if(!GetShaderError(id_vertex_shader) && !GetShaderError(id_fragment_shader)){
        // Si no hay error atachamos
        glAttachShader(id_program, id_vertex_shader);
        glAttachShader(id_program, id_fragment_shader);
      }else{
        // Error
        return nullptr;
      }


      // Cuando ya tenemos todos los shader compilados, linkamos el program
    glLinkProgram(id_program);
    glValidateProgram(id_program);

    int succes;
    glGetProgramiv(id_program, GL_VALIDATE_STATUS, &succes);
    if(succes != GL_TRUE){
      return nullptr;
    }

    // Llegados hasta aqui, todo ha ido bien y creamos el shader
    std::shared_ptr<Shader> shader = std::shared_ptr<Shader>(new Shader);
    shader->m_Data->id = id_program;
    shader->m_Data->shader_info.path_vertex = vertex_shader.c_str();
    shader->m_Data->shader_info.path_fragment = fragment_shader.c_str();

    return shader;
    }
  }

  std::shared_ptr<Shader> Shader::make(ShaderInfo s_info){

    unsigned int id_program = glCreateProgram();
    // Error
    if(id_program == 0){
        //return std::nullopt;
      return nullptr;
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
          return nullptr;
        }
      }
    }


    // Cuando ya tenemos todos los shader compilados, linkamos el program
    glLinkProgram(id_program);
    glValidateProgram(id_program);

    int succes;
    glGetProgramiv(id_program, GL_VALIDATE_STATUS, &succes);
    if(succes != GL_TRUE){
      return nullptr;
    }


    // Llegados hasta aqui, todo ha ido bien y creamos el shader
    //ShaderData data = {id_program};
    //Shader s;
    //s.m_Data->id = id_program;
    //s.m_Data->shader_info = s_info;
    
    std::shared_ptr<Shader> shader = std::shared_ptr<Shader>(new Shader);
    shader->m_Data->id = id_program;
    shader->m_Data->shader_info = s_info;

    return shader;
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