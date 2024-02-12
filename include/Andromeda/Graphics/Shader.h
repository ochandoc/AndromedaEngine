#pragma once
#include "Andromeda/Graphics/Light.h"
#include "Andromeda/Graphics/OpenGLTexture2D.h"
#include "Andromeda/Resources/Resource.h"
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

  struct UniformBlockData{
    float model[16];
    float view[16];
    float projection[16]; // 192
    float camera_position[3];
    //float padding; // 448 bytes, aligned to 28 blocks of 16 bytes
  };

  struct UniformLights{
    AmbientLight light_ambient;
    DirectionalLight light_directional;
    PointLight light_point;
    SpotLight light_spot;
  };

  class Shader
  {
  public:

    //static std::shared_ptr<Shader> make(ShaderInfo s_info);
    static std::shared_ptr<Shader> make(const std::string& path);
    static std::shared_ptr<Shader> make_default(const std::string& path, const std::string& light_path, LightType type);
    Shader(const Shader& other) = delete;
    Shader(Shader&& other);
    ~Shader();


    Shader& operator=(const Shader& other) = delete;
    Shader& operator=(Shader&& other);

    void setMat4(std::string name, const float matrix[16]);
    void setVec3(std::string name, const float vector[9]);
    void setModelViewProj(const float model[16], const float view[16], const float projection[16]);
    
    void use();
    void configure_shader();
    void un_configure_shader();

    // Uniform buffer
    void set_light(AmbientLight* light);
    void set_light(PointLight* light);
    
    void set_default_light(AmbientLight* light);
    void set_default_light(PointLight* light);

    void set_camera_position(const float position[3]);

    void upload_data();
    void upload_default_data(LightType type);
    void reload();

    void set_texture(OpenGLTexture2D* r);
    //const OpenGLTexture2D& get_texture();

  private:
    Shader();
    std::unique_ptr<struct ShaderData> m_Data;
    char m_shader_error[1024] = {0};

    std::shared_ptr<UniformBlockData> m_uniform_block;
    std::shared_ptr<UniformLights> m_uniform_block_lights;

    std::shared_ptr<AmbientLight> m_default_ambient;
    std::shared_ptr<DirectionalLight> m_default_directional;
    std::shared_ptr<PointLight> m_default_point;
    std::shared_ptr<SpotLight> m_default_spot;
    
    OpenGLTexture2D* m_texture;
  };
}