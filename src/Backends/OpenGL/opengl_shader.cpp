#include "Common/Shader.h"

#include "Common/Slurp.h"
#include "Backends/OpenGL/opengl_uniform_buffer.h"
#include "Backends/OpenGL/OpenGL.h"

#include "andpch.hpp"


namespace And{

  struct ShaderData{
    unsigned int id;
    ShaderInfo shader_info;
    std::string shader_path;
    std::unique_ptr<UniformBuffer> uniform_buffer;
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

      unsigned int id_vertex_shader = glCreateShader(GL_VERTEX_SHADER);
      unsigned int id_fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

      const char* aux_v = vertex_shader.c_str();
      const char* aux_f = fragment_shader.c_str();

      printf("%s\n", vertex_shader.c_str());

      //printf("Vertex %s\n", aux_v);

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

      //unsigned int id_ambient_block = glGetUniformBlockIndex(id_program, "Ambient_light");
      //int size = 0;
      //glGetActiveUniformBlockiv(id_program, id_ambient_block, GL_UNIFORM_BLOCK_DATA_SIZE, &size);


      unsigned int id_block = glGetUniformBlockIndex(id_program, "UniformBlock");
      int size_block;
      glGetActiveUniformBlockiv(id_program, id_block, GL_UNIFORM_BLOCK_DATA_SIZE, &size_block);
      printf("Size in C++: %d size in gl: %d\n", sizeof(UniformBlockData), size_block);

      // Debug
      //GLubyte* blockbuffer = (GLubyte*) malloc(size_block);
      /*const GLchar* names[] = {"model", "view", "projection", "ambient", "directional.enabled", "point", "spot"};

      GLuint indices[7];
      GLint offset[7];

      int totalUniforms;
      glGetProgramiv(id_program, GL_ACTIVE_UNIFORMS, &totalUniforms);
      printf("Total uniforms in program: %d\n", totalUniforms);

      glGetUniformIndices(id_program, 7, names, indices);
      glGetActiveUniformsiv(id_program, 7, indices, GL_UNIFORM_OFFSET, offset);
      
       for(int i = 0; i < 7; i++){
        printf("Atribute: %s has index %d with offset %d\n",names[i], indices[i], offset[i]);
      }
      //free(blockbuffer);
      // End debug
      */

      //printf("id-> %d size-> %d\n", id_ambient_block, size);

      GLenum err = glGetError();

      // Llegados hasta aqui, todo ha ido bien y creamos el shader
      std::shared_ptr<Shader> shader = std::shared_ptr<Shader>(new Shader);
      shader->m_Data->id = id_program;
      shader->m_Data->shader_info.path_vertex = vertex_shader.c_str();
      shader->m_Data->shader_info.path_fragment = fragment_shader.c_str();
      shader->m_Data->shader_path = path;
      //shader->m_Data->u_buffer = std::make_unique<UniformBuffer>(id_ambient_block, size);
      //err = glGetError();
      shader->m_Data->uniform_buffer = std::make_unique<UniformBuffer>(id_block, (unsigned int)size_block);

      //for(int i = 0; i < 10000; i++){
        //printf("Ole los caracoles %d ",i);
      //}
      
      // Si quito esta linea, peta
      err = glGetError();
      shader->m_uniform_block = std::make_shared<UniformBlockData>();

      //err = glGetError();
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

    m_uniform_block->light_ambient.active = light->active;
    m_uniform_block->light_ambient.specular_strength = light->specular_strength;
    m_uniform_block->light_ambient.specular_shininess = light->specular_shininess;

    for(int i = 0; i < 3; i++){
      m_uniform_block->light_ambient.direction[i] = light->direction[i];
      m_uniform_block->light_ambient.diffuse_color[i] = light->diffuse_color[i];
      m_uniform_block->light_ambient.specular_color[i] = light->specular_color[i];
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

  void Shader::upload_data(){
    m_Data->uniform_buffer->upload_data((void*)(m_uniform_block.get()), (unsigned int)sizeof(UniformBlockData));
  }


  Shader::Shader() : m_Data(new ShaderData){}

  void Shader::use(){
    glUseProgram(m_Data->id);
  }

  void Shader::configure_shader(){
    m_Data->uniform_buffer->bind();
  }

  void Shader::un_configure_shader(){
    m_Data->uniform_buffer->unbind();
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