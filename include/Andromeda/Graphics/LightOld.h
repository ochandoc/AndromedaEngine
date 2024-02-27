#pragma once
#include "Andromeda/Misc/CoreMiscDefines.h"
#include "Andromeda/Resources/ResourceManager.h"
#include "Backends/OpenGL/opengl_uniform_buffer.h"

namespace And{

enum class LightType : unsigned int{ 

  None,
  Ambient,
  Directional,
  Point,
  Spot
};

enum class LightBindingPoint : unsigned int{
  Ambient = 2,
  Directional = 3,
  Point = 4,
  Spot = 5
};

struct AmbientLight{
  float direction[3] = {0.5f, 0.5f, 0.5f};
  float enabled = 0.5f;
  float diffuse_color[3] = {0.5f, 0.5f, 0.5f};
  float specular_strength = 0.5f;
  float specular_color[3] = {0.5f, 0.5f, 0.5f};
  float specular_shininess = 0.5f; // 48 bytes
};

struct DirectionalLight{
  float direction[3] = {0.5f, 0.5f, 0.5f};
  float enabled = 0.5f;
  float diffuse_color[3] = {0.5f, 0.5f, 0.5f};
  float specular_strength = 0.5f;
  float specular_color[3] = {0.5f, 0.5f, 0.5f};
  float specular_shininess = 0.5f; // 48 bytes
};

struct PointLight{
  float position[3] = {0.5f, 0.5f, 0.5f};
  float specular_strength = 0.5f;
  float diffuse_color[3] = {0.5f, 0.5f, 0.5f};
  float specular_shininess = 0.5f;
  float specular_color[3] = {0.5f, 0.5f, 0.5f};
  float constant_att = 0.5f;
  float linear_att = 0.5f;
  float quadratic_att = 0.5f;
  float enabled = 0.5f;
  float attenuation = 1.0f; // 64 bytes
};

/*
struct SpotLight {
  float position[3] = {0.5f, 0.5f, 0.5f};
  float padding3 = -1.0f;
  float direction[3] = {0.5f, 0.5f, 0.5f};
  float padding = -1.0f;
  float diffuse_color[3] = {0.5f, 0.5f, 0.5f};
  float padding2 = -1.0f;
  float specular_color[3] = {0.5f, 0.5f, 0.5f};
  float enabled = 0.5f;
  float cutt_off = 0.5f;
  float outer_cut_off = 0.5f;
  float specular_strength = 0.5f;
  float specular_shininess = 0.5f;
  float constant_att = 0.5f;
  float linear_att = 0.5f;
  float quadratic_att = 0.5f; // 80 bytes
};


struct Light{
  std::shared_ptr<AmbientLight> ambient;
  std::shared_ptr<DirectionalLight> directional;
  std::shared_ptr<PointLight> point;
  std::shared_ptr<SpotLight> spot;
  LightType type = LightType::None;
};


class OldShader;
class LightManager{

  public:
    
    LightManager();
    ~LightManager();
    LightManager(LightManager&&) = default;
    NON_COPYABLE_CLASS(LightManager)

    void add_light(std::shared_ptr<AmbientLight> l);
    void add_light(std::shared_ptr<DirectionalLight> l);
    void add_light(std::shared_ptr<PointLight> l);
    void add_light(std::shared_ptr<SpotLight> l);
        
    OldShader* bind_light(Light l);

    std::vector<Light> get_lights() { return m_Lights; }
  private:
    std::vector<Light> m_Lights;

    std::shared_ptr<OldShader> m_ambient_OldShader;
    std::shared_ptr<OldShader> m_directional_OldShader;
    std::shared_ptr<OldShader> m_point_OldShader;
    std::shared_ptr<OldShader> m_spot_OldShader;


    std::shared_ptr<UniformBuffer> m_buffer_matrix;
    std::shared_ptr<UniformBuffer> m_buffer_spot;
   
    std::shared_ptr<OldShader> m_ambient_Shader;
    std::shared_ptr<OldShader> m_directional_Shader;
    std::shared_ptr<OldShader> m_point_Shader;
    std::shared_ptr<OldShader> m_spot_Shader;


};
*/

}