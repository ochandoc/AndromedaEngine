#include "Andromeda/Graphics/Shader.h"

#include "Andromeda/Misc/Slurp.h"

#include "Backends/OpenGL/OpenGL.h"
#include "Backends/OpenGL/opengl_uniform_buffer.h"
#include "Backends/OpenGL/OpenGLTexture2D.h"

#include "andpch.hpp"


namespace And{

// Tengo que crearme un uniform buffer de cada tipo de luz
  struct OldShaderData{
    unsigned int id;
    OldShaderInfo OldShader_info;
    std::string OldShader_path;
    std::unique_ptr<UniformBuffer> uniform_buffer;
    int buffer_size;
    std::unique_ptr<UniformBuffer> uniform_buffer_lights;

    std::unique_ptr<UniformBuffer> uniform_buffer_ambient;
    int ambient_size;
    std::unique_ptr<UniformBuffer> uniform_buffer_directional;
    int directional_size;
    std::unique_ptr<UniformBuffer> uniform_buffer_point;
    int point_size;
    std::unique_ptr<UniformBuffer> uniform_buffer_spot;
    int spot_size;
    int buffer_lights_size;
  };

/*
  OldShader::OldShader(const OldShader& other) : OldShader(){
    m_Data->id = other.m_Data->id;
  }
*/

  OldShader::OldShader(OldShader&& other) : OldShader(){
    m_Data->id = other.m_Data->id;
    m_Data->OldShader_info = other.m_Data->OldShader_info;
    other.m_Data->id = 0;
  }

  /*
  OldShader& OldShader::operator=(const OldShader& other){
    m_Data->id = other.m_Data->id; 

    return *this;
  }
  */

  OldShader& OldShader::operator=(OldShader&& other){
    m_Data->id = other.m_Data->id;
    m_Data->OldShader_info = other.m_Data->OldShader_info;
    other.m_Data->id = 0;

    return *this;
  }


  // Returns true if gets an error
  bool GetOldShaderError(unsigned int id){
    // Recogemos codigo de error en caso de que lo haya
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    // Fallo en la compilacion
    if(result == GL_FALSE){
      int lenght;
      char OldShader_error[1024];
      glGetShaderiv(id, GL_INFO_LOG_LENGTH, &lenght);
      glGetShaderInfoLog(id, lenght, &lenght, OldShader_error);
      printf("%s\n",OldShader_error);
      return true;
    }

    return false;
  }

  std::shared_ptr<OldShader> OldShader::make(const std::string& path){

    unsigned int id_program = glCreateProgram();
    // Error
    if(id_program == 0){
        //return std::nullopt;
      return nullptr;
    }

    Slurp file{path.c_str()};
    std::string OldShaders{file.data(), file.size()};

    int vertex_pos = (int)OldShaders.find("#type Vertex");
    int fragment_pos = (int)OldShaders.find("#type Fragment");

    std::string vertex_OldShader;
    std::string fragment_OldShader;

    if(vertex_pos != std::string::npos && fragment_pos != std::string::npos){
      vertex_OldShader = OldShaders.substr(vertex_pos, fragment_pos);
      fragment_OldShader = OldShaders.substr(fragment_pos, OldShaders.size() - 1);

      std::string vertex_title = "#type Vertex";
      std::string fragment_title = "#type Fragment";

      vertex_OldShader.erase(0, vertex_title.size());
      fragment_OldShader.erase(0, fragment_title.size());

      unsigned int id_vertex_OldShader = glCreateShader(GL_VERTEX_SHADER);
      unsigned int id_fragment_OldShader = glCreateShader(GL_FRAGMENT_SHADER);

      const char* aux_v = vertex_OldShader.c_str();
      const char* aux_f = fragment_OldShader.c_str();

      glShaderSource(id_vertex_OldShader, 1, &aux_v, nullptr);
      glShaderSource(id_fragment_OldShader, 1, &aux_f, nullptr);

      // Compilamos
      glCompileShader(id_vertex_OldShader);
      glCompileShader(id_fragment_OldShader);

      GetOldShaderError(id_vertex_OldShader);
      GetOldShaderError(id_fragment_OldShader);

      if(!GetOldShaderError(id_vertex_OldShader) && !GetOldShaderError(id_fragment_OldShader)){
        // Si no hay error atachamos
        glAttachShader(id_program, id_vertex_OldShader);
        glAttachShader(id_program, id_fragment_OldShader);
      }else{
        // Error
        printf("Error on loading OldShader\n");
        return nullptr;
      }

      // Cuando ya tenemos todos los OldShader compilados, linkamos el program
      glLinkProgram(id_program);
      glValidateProgram(id_program);

      int succes;
      glGetProgramiv(id_program, GL_VALIDATE_STATUS, &succes);
      if(succes != GL_TRUE){
        printf("Error on loading OldShader\n");
        return nullptr;
      }

      unsigned int id_block = glGetUniformBlockIndex(id_program, "UniformBlock");
      unsigned int id_block_lights = glGetUniformBlockIndex(id_program, "UniformLights");

      int size_block;
      int size_block_lights;

      glGetActiveUniformBlockiv(id_program, id_block, GL_UNIFORM_BLOCK_DATA_SIZE, &size_block);
      glGetActiveUniformBlockiv(id_program, id_block_lights, GL_UNIFORM_BLOCK_DATA_SIZE, &size_block_lights);

       glUniformBlockBinding(id_program, id_block, 0);
      glUniformBlockBinding(id_program, id_block_lights, 1);
      
      //printf("Invalid index-> %u my index-> %d\n", GL_INVALID_INDEX, id_block);
      //int size_struct = sizeof(UniformBlockData);
      //int size_struct_lights = sizeof(UniformLights);
      //printf("Size in C++: %d size in gl: %d\n", size_struct, size_block);
      //printf("Size in C++: %d size in gl: %d\n", size_struct_lights, size_block_lights);

      // Llegados hasta aqui, todo ha ido bien y creamos el OldShader
      std::shared_ptr<OldShader> Oldshader = std::shared_ptr<OldShader>(new OldShader);
      Oldshader->m_Data->id = id_program;
      Oldshader->m_Data->OldShader_info.path_vertex = vertex_OldShader.c_str();
      Oldshader->m_Data->OldShader_info.path_fragment = fragment_OldShader.c_str();
      Oldshader->m_Data->OldShader_path = path;

      Oldshader->m_Data->uniform_buffer = std::make_unique<UniformBuffer>(0, (unsigned int)size_block);
      Oldshader->m_Data->uniform_buffer_lights = std::make_unique<UniformBuffer>(1, (unsigned int)size_block_lights);
      
      Oldshader->m_uniform_block = std::make_shared<UniformBlockData>();
      Oldshader->m_uniform_block_lights = std::make_shared<UniformLights>();
      
      Oldshader->m_Data->buffer_size = size_block;
      Oldshader->m_Data->buffer_lights_size = size_block_lights;
      Oldshader->m_texture = nullptr;
      Oldshader->m_has_texture_ = false;
      //glFlush();
      return Oldshader;
    }
    return nullptr;
  }

  std::shared_ptr<OldShader> OldShader::make_default(const std::string& path, const std::string& light_path, LightType type){

    unsigned int id_program = glCreateProgram();
    // Error
    if(id_program == 0){
        //return std::nullopt;
      return nullptr;
    }

    Slurp file{path.c_str()};
    std::string OldShaders{file.data(), file.size()};

    int vertex_pos = (int)OldShaders.find("#type Vertex");
    int fragment_pos = (int)OldShaders.find("#type Fragment");

    std::string vertex_OldShader;
    std::string fragment_OldShader;

    if(vertex_pos != std::string::npos && fragment_pos != std::string::npos){
      vertex_OldShader = OldShaders.substr(vertex_pos, fragment_pos);
      fragment_OldShader = OldShaders.substr(fragment_pos, OldShaders.size() - 1);

      std::string vertex_title = "#type Vertex";
      std::string fragment_title = "#type Fragment";

      vertex_OldShader.erase(0, vertex_title.size());
      fragment_OldShader.erase(0, fragment_title.size());

      unsigned int id_vertex_OldShader = glCreateShader(GL_VERTEX_SHADER);
      unsigned int id_fragment_OldShader = glCreateShader(GL_FRAGMENT_SHADER);

      const char* aux_v = vertex_OldShader.c_str();
      const char* aux_f = fragment_OldShader.c_str();

      //printf("%s\n %s\n", aux_v, aux_f);

      glShaderSource(id_vertex_OldShader, 1, &aux_v, nullptr);
      glShaderSource(id_fragment_OldShader, 1, &aux_f, nullptr);

      // Compilamos
      glCompileShader(id_vertex_OldShader);
      glCompileShader(id_fragment_OldShader);

      GetOldShaderError(id_vertex_OldShader);
      GetOldShaderError(id_fragment_OldShader);

      if(!GetOldShaderError(id_vertex_OldShader) && !GetOldShaderError(id_fragment_OldShader)){
        // Si no hay error atachamos
        glAttachShader(id_program, id_vertex_OldShader);
        glAttachShader(id_program, id_fragment_OldShader);
      }else{
        // Error
        printf("Error on loading OldShader\n");
        return nullptr;
      }

      // Cuando ya tenemos todos los OldShader compilados, linkamos el program
      glLinkProgram(id_program);
      glValidateProgram(id_program);

      int succes;
      glGetProgramiv(id_program, GL_VALIDATE_STATUS, &succes);
      if(succes != GL_TRUE){
        printf("Error on loading OldShader\n");
        return nullptr;
      }

      unsigned int id_block = glGetUniformBlockIndex(id_program, "UniformBlock");

      // this block must be for ambient, directional, point or spotlight
      unsigned int id_block_lights = -1;
      if(type != LightType::None){
        id_block_lights = glGetUniformBlockIndex(id_program, light_path.c_str());
      }

      int size_block;
      int size_block_lights = -1;

      glGetActiveUniformBlockiv(id_program, id_block, GL_UNIFORM_BLOCK_DATA_SIZE, &size_block);

      if(type != LightType::None){
        glGetActiveUniformBlockiv(id_program, id_block_lights, GL_UNIFORM_BLOCK_DATA_SIZE, &size_block_lights);
      }
        

      printf("Size of Spot in c++ %zu size in opengl %d\n", sizeof(SpotLight), size_block_lights);

      glUniformBlockBinding(id_program, id_block, 0);

      //id_block_lights = glGetUniformBlockIndex(id_program, light_path.c_str());
      
      //printf("Invalid index-> %u my index-> %d\n", GL_INVALID_INDEX, id_block);
      //int size_struct = sizeof(UniformBlockData);
      //int size_struct_lights = sizeof(UniformLights);
      //printf("Size in C++: %d size in gl: %d\n", size_struct, size_block);
      //printf("Size in C++: %d size in gl: %d\n", size_struct_lights, size_block_lights);

      // Llegados hasta aqui, todo ha ido bien y creamos el OldShader
      std::shared_ptr<OldShader> Oldshader = std::shared_ptr<OldShader>(new OldShader);
      Oldshader->m_Data->id = id_program;
      Oldshader->m_Data->OldShader_info.path_vertex = vertex_OldShader.c_str();
      Oldshader->m_Data->OldShader_info.path_fragment = fragment_OldShader.c_str();
      Oldshader->m_Data->OldShader_path = path;

      Oldshader->m_Data->uniform_buffer = std::make_unique<UniformBuffer>(0, (unsigned int)size_block);
      switch(type){
        case LightType::Ambient: 
        glUniformBlockBinding(id_program, id_block_lights, (unsigned int)LightBindingPoint::Ambient);
        Oldshader->m_Data->uniform_buffer_ambient = std::make_unique<UniformBuffer>((unsigned int)LightBindingPoint::Ambient, (unsigned int)size_block_lights);
        Oldshader->m_Data->ambient_size = size_block_lights;
        Oldshader->m_default_ambient = std::make_shared<AmbientLight>();
        break;
        case LightType::Directional: 
        glUniformBlockBinding(id_program, id_block_lights, (unsigned int)LightBindingPoint::Directional);
        Oldshader->m_Data->uniform_buffer_directional = std::make_unique<UniformBuffer>((unsigned int)LightBindingPoint::Directional, (unsigned int)size_block_lights);
        Oldshader->m_Data->directional_size = size_block_lights;
        Oldshader->m_default_directional = std::make_shared<DirectionalLight>();
        break;
        case LightType::Point: 
        glUniformBlockBinding(id_program, id_block_lights, (unsigned int)LightBindingPoint::Point);
        Oldshader->m_Data->uniform_buffer_point = std::make_unique<UniformBuffer>((unsigned int)LightBindingPoint::Point, (unsigned int)size_block_lights);
        Oldshader->m_Data->point_size = size_block_lights;
        Oldshader->m_default_point = std::make_shared<PointLight>();
        break;
        case LightType::Spot: 
        glUniformBlockBinding(id_program, id_block_lights, (unsigned int)LightBindingPoint::Spot);
        Oldshader->m_Data->uniform_buffer_spot = std::make_unique<UniformBuffer>((unsigned int)LightBindingPoint::Spot, (unsigned int)size_block_lights);
        Oldshader->m_Data->spot_size = size_block_lights;
        Oldshader->m_default_spot = std::make_shared<SpotLight>();
        break;
      }

      Oldshader->m_uniform_block = std::make_shared<UniformBlockData>();
      Oldshader->m_Data->buffer_size = size_block;
      Oldshader->m_texture = nullptr;
      Oldshader->m_has_texture_ = false;


      //glFlush();
      return Oldshader;
    }
    return nullptr;
  }

  void OldShader::setMat4(std::string name, const float matrix[16]){ 
    glUniformMatrix4fv(glGetUniformLocation(m_Data->id, name.c_str()), 1, GL_FALSE, &matrix[0]);
  }

  void OldShader::setVec3(std::string name, const float vector[9]){

    glUniform3fv(glGetUniformLocation(m_Data->id, name.c_str()),1, &vector[0]);
  }

  void OldShader::set_default_light(AmbientLight* light){
    //m_Data->uniform_buffer->upload_data((void*)(light), sizeof(AmbientLight));

    m_default_ambient->enabled = light->enabled;
    m_default_ambient->specular_strength = light->specular_strength;
    m_default_ambient->specular_shininess = light->specular_shininess;

    for(int i = 0; i < 3; i++){
      m_default_ambient->direction[i] = light->direction[i];
      m_default_ambient->diffuse_color[i] = light->diffuse_color[i];
      m_default_ambient->specular_color[i] = light->specular_color[i];
    }    
  }
  
  void OldShader::set_default_light(DirectionalLight* light){
    //m_Data->uniform_buffer->upload_data((void*)(light), sizeof(AmbientLight));

    m_default_directional->enabled = light->enabled;
    m_default_directional->specular_strength = light->specular_strength;
    m_default_directional->specular_shininess = light->specular_shininess;

    for(int i = 0; i < 3; i++){
      m_default_directional->direction[i] = light->direction[i];
      m_default_directional->diffuse_color[i] = light->diffuse_color[i];
      m_default_directional->specular_color[i] = light->specular_color[i];
    }    
  }

  void OldShader::set_default_light(PointLight* light){
    m_default_point->enabled = light->enabled;
    m_default_point->specular_strength = light->specular_strength;
    m_default_point->specular_shininess = light->specular_shininess;
    m_default_point->constant_att = light->constant_att;
    m_default_point->linear_att = light->linear_att;
    m_default_point->quadratic_att = light->quadratic_att;
    m_default_point->attenuation = light->attenuation;
    for(int i = 0; i < 3; i++){
      m_default_point->position[i] = light->position[i];
      m_default_point->diffuse_color[i] = light->diffuse_color[i];
      m_default_point->specular_color[i] = light->specular_color[i];
    }
  }

  void OldShader::set_default_light(SpotLight* light){
    m_default_spot->enabled = light->enabled;
    m_default_spot->specular_strength = light->specular_strength;
    m_default_spot->specular_shininess = light->specular_shininess;
    m_default_spot->constant_att = light->constant_att;
    m_default_spot->linear_att = light->linear_att;
    m_default_spot->quadratic_att = light->quadratic_att;
    m_default_spot->cutt_off = light->cutt_off;
    m_default_spot->outer_cut_off = light->outer_cut_off;
    for(int i = 0; i < 3; i++){
      m_default_spot->position[i] = light->position[i];
      m_default_spot->diffuse_color[i] = light->diffuse_color[i];
      m_default_spot->specular_color[i] = light->specular_color[i];
      m_default_spot->direction[i] = light->direction[i];
    }
  }
  
  void OldShader::set_light(AmbientLight* light){
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

  void OldShader::set_light(PointLight* light){
    m_uniform_block_lights->light_point.enabled = light->enabled;
    m_uniform_block_lights->light_point.specular_strength = light->specular_strength;
    m_uniform_block_lights->light_point.specular_shininess = light->specular_shininess;
    m_uniform_block_lights->light_point.constant_att = light->constant_att;
    m_uniform_block_lights->light_point.linear_att = light->linear_att;
    m_uniform_block_lights->light_point.quadratic_att = light->quadratic_att;
    m_uniform_block_lights->light_point.attenuation = light->attenuation;
    for(int i = 0; i < 3; i++){
      m_uniform_block_lights->light_point.position[i] = light->position[i];
      m_uniform_block_lights->light_point.diffuse_color[i] = light->diffuse_color[i];
      m_uniform_block_lights->light_point.specular_color[i] = light->specular_color[i];
    }
  }

  void OldShader::setModelViewProj(const float model[16], const float view[16], const float projection[16]){

    for(int i = 0; i < 16; i++){
      m_uniform_block->model[i] = model[i];
      m_uniform_block->view[i] = view[i];
      m_uniform_block->projection[i] = projection[i];
    }
    
    //m_Data->uniform_buffer->upload_data((void*)(&tmp), (unsigned int)sizeof(ModelViewProj));
  }

  void OldShader::set_camera_position(const float position[3]){
    for(int i = 0; i < 3; i++){
      m_uniform_block->camera_position[i] = position[i];
    }
  }

  void OldShader::upload_data(){
    
    // this ubo is for MVP matrices
    m_Data->uniform_buffer->upload_data((void*)(m_uniform_block.get()), (unsigned int)m_Data->buffer_size);
    m_Data->uniform_buffer->bind();
    //glFlush();

    
    //m_Data->uniform_buffer_lights->upload_data((void*)(m_uniform_block_lights.get()), (unsigned int)m_Data->buffer_lights_size);
    //m_Data->uniform_buffer_lights->bind();
    //glFlush();

   
    if(m_has_texture_){
      OpenGLTexture2D* text = static_cast<OpenGLTexture2D*>(m_texture);
      if (text != nullptr) {
          text->Activate(0);
      }
    }
    
   
  }

  void OldShader::upload_default_data(LightType type){
 
    //m_Data->uniform_buffer->upload_data((void*)(m_uniform_block.get()), (unsigned int)m_Data->buffer_size);
    //m_Data->uniform_buffer->bind();
    //glFlush();


    switch(type){
      case LightType::Ambient:
        m_Data->uniform_buffer_ambient->upload_data((void*)(m_default_ambient.get()), (unsigned int)m_Data->ambient_size);
        m_Data->uniform_buffer_ambient->bind();
        break;
      case LightType::Directional:
        m_Data->uniform_buffer_directional->upload_data((void*)(m_default_directional.get()), (unsigned int)m_Data->directional_size);
        m_Data->uniform_buffer_directional->bind();
        break;
      case LightType::Point:
        m_Data->uniform_buffer_point->upload_data((void*)(m_default_point.get()), (unsigned int)m_Data->point_size);
        m_Data->uniform_buffer_point->bind();
        break;
      case LightType::Spot:
        m_Data->uniform_buffer_spot->upload_data((void*)(m_default_spot.get()), (unsigned int)m_Data->spot_size);
        m_Data->uniform_buffer_spot->bind();
        break;
    }


    //m_Data->uniform_buffer_lights->bind();
    //glFlush();
    

    if(m_has_texture_){
      OpenGLTexture2D* text = static_cast<OpenGLTexture2D*>(m_texture);
      if (text != nullptr) {
          text->Activate(0);
      }
    }

  }

  void OldShader::configure_OldShader(){
    //m_Data->uniform_buffer->bind();
  }

  void OldShader::un_configure_OldShader(){
    m_Data->uniform_buffer->unbind();
    //m_Data->uniform_buffer_lights->unbind();
  }

  void OldShader::set_texture(Texture* texture){
    if(texture){
      m_texture = texture;
      m_has_texture_ = true;
    }else{
      m_has_texture_ = false;
    }
  }

  //void OldShader::get_texture(){}
  OldShader::OldShader() : m_Data(new OldShaderData){}

  void OldShader::use(){
    glUseProgram(m_Data->id);
  }


  OldShader::~OldShader(){
    glDeleteProgram(m_Data->id);
  }

  void OldShader::reload(){
    //glLinkProgram(m_Data->id);
    glDeleteShader(m_Data->id);
    std::shared_ptr<OldShader> s = make(m_Data->OldShader_path);
    m_Data->id = s->m_Data->id;
  }

  

}