#pragma once

namespace And{

  enum ShaderType
  {
    Shader_Vertex,
    Shader_Fragment,
    Shader_Geometry,
    Shader_Teselation,
  };

  struct ShaderInfo{
    const char* path_fragment = nullptr;
    const char* path_vertex = nullptr;
    const char* path_teselation = nullptr;
    const char* path_geometry = nullptr;
  };

  class Shader
  {
  public:

    static std::optional<Shader> make(ShaderInfo s_info);
    Shader(const Shader& other);
    Shader(Shader&& other);
    ~Shader();


    Shader& operator=(const Shader& other);
    Shader& operator=(Shader&& other);
    
    void use();

    void upload_shader(ShaderType t, const char* path);

    // returns link program error code
    int link_shaders();

    // returns error of the last shader uploaded
    char* get_upload_shader_error();

  private:
    Shader();
    std::unique_ptr<struct ShaderData> m_Data;
    char m_shader_error[1024] = {0};
  };
}