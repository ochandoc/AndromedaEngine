#pragma once

#include "Andromeda/Misc/CoreMiscDefines.h"
#include "Andromeda/HAL/Types.h"
#include "ObjLoader.h"
#include "Andromeda/Graphics/RenderTarget.h"
#include "Andromeda/Graphics/FlyCamera.h"
#include "Andromeda/Graphics/LightOld.h"
#include "Andromeda/ECS/Components/MeshComponent.h"
#include "Andromeda/ECS/Scene.h"
#include "Andromeda/Graphics/Lights/SpotLight.h"

namespace And
{
  class Window;
  class OldShader;
  class Triangle;
  class ObjLoader;
  class TransformComponent;
  struct OldShaderInfo;
  class UniformBuffer;


class Renderer
{
  NON_COPYABLE_CLASS(Renderer)
  NON_MOVABLE_CLASS(Renderer)
public:
  Renderer(Window& window);
  ~Renderer();

  void new_frame();
  void end_frame();

  void set_viewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height);

  void set_draw_on_texture(bool value);

  void set_clear_color(float* color);

  std::shared_ptr<RenderTarget> get_render_target() const;

  void showDemo();
  void showImGuiDemoWindow();

  void draw_triangle(Triangle *t);

  void draw_obj(MeshComponent* obj, Light* l, TransformComponent* tran);

  // Si castea sombras, precalculamos las matrices de la camara y pasamos ademas la view * projection de la luz
  void draw_obj_shadows(MeshComponent* obj, TransformComponent* trans, SpotLight* l);
  void draw_obj_shadows(MeshComponent* obj, TransformComponent* trans, DirectionalLight* l);
  void draw_deep_obj(MeshComponent* obj, std::shared_ptr<Shader> s, TransformComponent* tran,float* view, float* projection);

  void draw_scene(Scene& scene, Shader* s);

  void draw_shadows(SpotLight* l, MeshComponent* obj, TransformComponent* tran);
  void draw_shadows(DirectionalLight* l, MeshComponent* obj, TransformComponent* tran);

  std::shared_ptr<RenderTarget> get_shadow_buffer();

  friend void DrawForward(EntityComponentSystem& entity, Renderer& renderer);


protected:
  Window& m_Window;
  std::shared_ptr<RenderTarget> m_RenderTarget;

private:
  bool m_bDrawOnTexture;

  FlyCamera m_Camera;

  //std::shared_ptr<OldShader> m_shadow_shader;
  std::shared_ptr<RenderTarget> m_shadows_buffer_;

  

  std::shared_ptr<Shader> m_shadow_shader;
  std::shared_ptr<Shader> m_depth_shader;
  
  std::shared_ptr<Shader> m_shader_ambient;
  std::shared_ptr<Shader> m_shader_directional;
  std::shared_ptr<Shader> m_shader_shadows_directional;
  
  std::shared_ptr<Shader> m_shader_point;

  std::shared_ptr<Shader> m_shader_spot;
  std::shared_ptr<Shader> m_shader_shadows_spot;

  std::shared_ptr<UniformBuffer> m_buffer_matrix; // 208 o 256
  std::shared_ptr<UniformBuffer> m_buffer_ambient_light; // 48
  std::shared_ptr<UniformBuffer> m_buffer_directional_light; // 48
  std::shared_ptr<UniformBuffer> m_buffer_point_light; // 64
  std::shared_ptr<UniformBuffer> m_buffer_spot_light; // 96

};

void DrawForward(EntityComponentSystem& entity, Renderer& renderer);

}
