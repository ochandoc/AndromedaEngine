#pragma once

namespace And{

  enum ShaderType
  {
    Shader_Vertex,
    Shader_Fragment,
    Shader_Geometry,
    Shader_Teselation,
  };

  struct ShaderInfo
  {
    // Shader type
    ShaderType type;
    // File path
    const char* file_path;
  };

  class Shader
  {
  public:

    Shader(std::vector<ShaderInfo> S_info);
    Shader(const Shader&) = delete;
    Shader(Shader&&) = delete;

    ~Shader();

    Shader& operator=(const Shader&) = delete;
    Shader& operator=(Shader&&) = delete;
    
    void use();

  private:
    std::unique_ptr<struct ShaderData> m_Data;
  };
}