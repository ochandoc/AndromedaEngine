#pragma once
#include <optional>

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
    Shader(const Shader& other) = delete;
    Shader(Shader&& other);
    ~Shader();


    Shader& operator=(const Shader& other) = delete;
    Shader& operator=(Shader&& other);

    void setMat4(std::string name, const float matrix[16]);
    void setVec3(std::string name, const float vector[9]);
    
    void use();

  private:
    Shader();
    std::unique_ptr<struct ShaderData> m_Data;
    char m_shader_error[1024] = {0};

  };
}