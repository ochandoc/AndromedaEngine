#pragma once

#include "Andromeda/Misc/CoreMiscDefines.h"

namespace And
{
  class Shader
  {
    NON_COPYABLE_CLASS(Shader)
    NON_MOVABLE_CLASS(Shader)
  public:
    Shader() = default;

    virtual ~Shader() = default;

    virtual void Use() const = 0;
    virtual void StopUsing() const = 0;

    inline const std::string& GetPath() const { return m_Path; }
  private:
    std::string m_Path;
  };

  std::shared_ptr<Shader> MakeShader(const std::string& Path);
}

#include "Andromeda/Graphics/Light.h"
#include "Andromeda/Graphics/Texture.h"
#include "Andromeda/Resources/Resource.h"
#include <optional>

namespace And{

  enum OldShaderType
  {
    OldShader_Vertex,
    OldShader_Fragment,
    OldShader_Geometry,
    OldShader_Teselation,
  };

  struct OldShaderInfo{
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

  class OldShader
  {
  public:

    //static std::shared_ptr<OldShader> make(OldShaderInfo s_info);
    static std::shared_ptr<OldShader> make(const std::string& path);
    static std::shared_ptr<OldShader> make_default(const std::string& path, const std::string& light_path, LightType type);
    OldShader(const OldShader& other) = delete;
    OldShader(OldShader&& other);
    ~OldShader();


    OldShader& operator=(const OldShader& other) = delete;
    OldShader& operator=(OldShader&& other);

    void setMat4(std::string name, const float matrix[16]);
    void setVec3(std::string name, const float vector[9]);
    void setModelViewProj(const float model[16], const float view[16], const float projection[16]);
    
    void use();
    void configure_OldShader();
    void un_configure_OldShader();

    // Uniform buffer
    void set_light(AmbientLight* light);
    void set_light(PointLight* light);
    
    void set_default_light(AmbientLight* light);
    void set_default_light(PointLight* light);
    void set_default_light(DirectionalLight* light);
    void set_default_light(SpotLight* light);

    void set_camera_position(const float position[3]);

    void upload_data();
    void upload_default_data(LightType type);
    void reload();

    void set_texture(Texture* r);
    //const OpenGLTexture2D& get_texture();

  private:
    OldShader();
    std::unique_ptr<struct OldShaderData> m_Data;
    char m_OldShader_error[1024] = {0};

    std::shared_ptr<UniformBlockData> m_uniform_block;
    std::shared_ptr<UniformLights> m_uniform_block_lights;

    std::shared_ptr<AmbientLight> m_default_ambient;
    std::shared_ptr<DirectionalLight> m_default_directional;
    std::shared_ptr<PointLight> m_default_point;
    std::shared_ptr<SpotLight> m_default_spot;
    
    Texture* m_texture;
    bool m_has_texture_;
  };
}