#pragma once

namespace And{

  enum ShaderType
  {
    Shader_Vertex,
    Shader_Fragment,
    Shader_Geometry,
    Shader_Teselation,
  };

  class Shader
  {
  public:

    Shader();
    Shader(const Shader&) = delete;
    Shader(Shader&&) = delete;

    ~Shader();

    Shader& operator=(const Shader&) = delete;
    Shader& operator=(Shader&&) = delete;
    
    void use();

    void upload_shader(ShaderType t, const char* path);

    // returns link program error code
    int link_shaders();

    // returns error of the last shader uploaded
    char* get_upload_shader_error();

  private:
    std::unique_ptr<struct ShaderData> m_Data;
    char m_shader_error[1024] = {0};
  };
}