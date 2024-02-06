#include "Andromeda/Graphics/Shader.h"

#include "Andromeda/Misc/Slurp.h"

#include "Backends/OpenGL/OpenGL.h"
#include "Backends/OpenGL/opengl_uniform_buffer.h"

#include "andpch.hpp"


namespace And{

  struct ShaderData{
    unsigned int id;
    ShaderInfo shader_info;
    std::string shader_path;
    std::unique_ptr<UniformBuffer> uniform_buffer;
    int buffer_size;
    std::unique_ptr<UniformBuffer> uniform_buffer_lights;
    int buffer_lights_size;
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

    Slurp file{path.c_str()};
    std::string shaders{file.data(), file.size()};

    int vertex_pos = (int)shaders.find("#type Vertex");
    int fragment_pos = (int)shaders.find("#type Fragment");

    std::string vertex_shader;
    std::string fragment_shader;

    if(vertex_pos != std::string::npos && fragment_pos != std::string::npos){
      vertex_shader = shaders.substr(vertex_pos, fragment_pos);
      fragment_shader = shaders.substr(fragment_pos, shaders.size() - 1);

      std::string vertex_title = "#type Vertex";
      std::string fragment_title = "#type Fragment";

      vertex_shader.erase(0, vertex_title.size());
      fragment_shader.erase(0, fragment_title.size());

      unsigned int id_vertex_shader = glCreateShader(GL_VERTEX_SHADER);
      unsigned int id_fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

      const char* aux_v = vertex_shader.c_str();
      const char* aux_f = fragment_shader.c_str();

      glShaderSource(id_vertex_shader, 1, &aux_v, nullptr);
      glShaderSource(id_fragment_shader, 1, &aux_f, nullptr);

      // Compilamos
      glCompileShader(id_vertex_shader);
      glCompileShader(id_fragment_shader);

      GetShaderError(id_vertex_shader);
      GetShaderError(id_fragment_shader);

      if(!GetShaderError(id_vertex_shader) && !GetShaderError(id_fragment_shader)){
        // Si no hay error atachamos
        glAttachShader(id_program, id_vertex_shader);
        glAttachShader(id_program, id_fragment_shader);
      }else{
        // Error
        printf("Error on loading shader\n");
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

      unsigned int id_block = glGetUniformBlockIndex(id_program, "UniformBlock");
      unsigned int id_block_lights = glGetUniformBlockIndex(id_program, "UniformLights");

      int size_block;
      int size_block_lights;

      glGetActiveUniformBlockiv(id_program, id_block, GL_UNIFORM_BLOCK_DATA_SIZE, &size_block);
      glGetActiveUniformBlockiv(id_program, id_block_lights, GL_UNIFORM_BLOCK_DATA_SIZE, &size_block_lights);
      
      //printf("Invalid index-> %u my index-> %d\n", GL_INVALID_INDEX, id_block);
      //int size_struct = sizeof(UniformBlockData);
      //int size_struct_lights = sizeof(UniformLights);
      //printf("Size in C++: %d size in gl: %d\n", size_struct, size_block);
      //printf("Size in C++: %d size in gl: %d\n", size_struct_lights, size_block_lights);

      // Llegados hasta aqui, todo ha ido bien y creamos el shader
      std::shared_ptr<Shader> shader = std::shared_ptr<Shader>(new Shader);
      shader->m_Data->id = id_program;
      shader->m_Data->shader_info.path_vertex = vertex_shader.c_str();
      shader->m_Data->shader_info.path_fragment = fragment_shader.c_str();
      shader->m_Data->shader_path = path;

      shader->m_Data->uniform_buffer = std::make_unique<UniformBuffer>(id_block, (unsigned int)size_block);
      shader->m_Data->uniform_buffer_lights = std::make_unique<UniformBuffer>(id_block_lights, (unsigned int)size_block_lights);
      shader->m_uniform_block = std::make_shared<UniformBlockData>();
      shader->m_uniform_block_lights = std::make_shared<UniformLights>();
      
      shader->m_Data->buffer_size = size_block;
      shader->m_Data->buffer_lights_size = size_block_lights;

      glFlush();
      return shader;
    }
    return nullptr;
  }


  void Shader::setMat4(std::string name, const float matrix[16]){ 
    glUniformMatrix4fv(glGetUniformLocation(m_Data->id, name.c_str()), 1, GL_FALSE, &matrix[0]);
  }

  void Shader::setVec3(std::string name, const float vector[9]){

    glUniform3fv(glGetUniformLocation(m_Data->id, name.c_str()),1, &vector[0]);
  }

  void Shader::set_light(AmbientLight* light){
    //m_Data->uniform_buffer->upload_data((void*)(light), sizeof(AmbientLight));

    m_uniform_block_lights->light_ambient.enabled = light->enabled;
    m_uniform_block_lights->light_ambient.specular_strength = light->specular_strength;
    m_uniform_block_lights->light_ambient.specular_shininess = light->specular_shininess;

    for(int i = 0; i < 3; i++){
      m_uniform_block_lights->light_ambient.direction[i] = light->direction[i];
      m_uniform_block_lights->light_ambient.diffuse_color[i] = light->diffuse_color[i];
      m_uniform_block_lights->light_ambient.specular_color[i] = light->specular_color[i];
    }    
  }

  void Shader::set_light(PointLight* light){
    m_uniform_block_lights->light_point.enabled = light->enabled;
    m_uniform_block_lights->light_point.specular_strength = light->specular_strength;
    m_uniform_block_lights->light_point.specular_shininess = light->specular_shininess;
    m_uniform_block_lights->light_point.constant_att = light->constant_att;
    m_uniform_block_lights->light_point.linear_att = light->linear_att;
    m_uniform_block_lights->light_point.quadratic_att = light->quadratic_att;
    for(int i = 0; i < 3; i++){
      m_uniform_block_lights->light_point.position[i] = light->position[i];
      m_uniform_block_lights->light_point.diffuse_color[i] = light->diffuse_color[i];
      m_uniform_block_lights->light_point.specular_color[i] = light->specular_color[i];
    }
  }

  void Shader::setModelViewProj(const float model[16], const float view[16], const float projection[16]){

    for(int i = 0; i < 16; i++){
      m_uniform_block->model[i] = model[i];
      m_uniform_block->view[i] = view[i];
      m_uniform_block->projection[i] = projection[i];
    }
    
    //m_Data->uniform_buffer->upload_data((void*)(&tmp), (unsigned int)sizeof(ModelViewProj));
  }

  void Shader::set_camera_position(const float position[3]){
    for(int i = 0; i < 3; i++){
      m_uniform_block->camera_position[i] = position[i];
    }
  }

  void Shader::upload_data(){
 
    m_Data->uniform_buffer->upload_data((void*)(m_uniform_block.get()), (unsigned int)m_Data->buffer_size);
    m_Data->uniform_buffer->bind();
    glFlush();

    m_Data->uniform_buffer_lights->upload_data((void*)(m_uniform_block_lights.get()), (unsigned int)m_Data->buffer_lights_size);
    m_Data->uniform_buffer_lights->bind();
    glFlush();
  }

  void Shader::configure_shader(){
    //m_Data->uniform_buffer->bind();
  }

  void Shader::un_configure_shader(){
    m_Data->uniform_buffer->unbind();
    m_Data->uniform_buffer_lights->unbind();
  }

  Shader::Shader() : m_Data(new ShaderData){}

  void Shader::use(){
    glUseProgram(m_Data->id);
  }


  Shader::~Shader(){
    glDeleteProgram(m_Data->id);
  }

  void Shader::reload(){
    //glLinkProgram(m_Data->id);
    glDeleteShader(m_Data->id);
    std::shared_ptr<Shader> s = make(m_Data->shader_path);
    m_Data->id = s->m_Data->id;
  }

  

}