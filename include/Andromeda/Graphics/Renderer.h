#pragma once

#include "Andromeda/Misc/CoreMiscDefines.h"
#include "Andromeda/HAL/Types.h"
#include "ObjLoader.h"
#include "Andromeda/Graphics/RenderTarget.h"
#include "Andromeda/Graphics/FlyCamera.h"
#include "Andromeda/Graphics/LightOld.h"
#include "Andromeda/ECS/Components/MeshComponent.h"
#include "Andromeda/ECS/Scene.h"
#include "Backends/OpenGL/opengl_uniform_buffer.h"

namespace And
{
  class Window;
  class OldShader;
  class Triangle;
  class ObjLoader;
  class TransformComponent;
  struct OldShaderInfo;


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

  void draw_obj(MeshComponent* obj, OldShader* s, TransformComponent* trans);

  // Si castea sombras, precalculamos las matrices de la camara y pasamos ademas la view * projection de la luz
  //void draw_obj_shadows(MeshComponent* obj, OldShader* s, TransformComponent* trans, const Light& l);
  void draw_deep_obj(MeshComponent* obj, OldShader* s, TransformComponent* tran,float* view, float* projection);

  void draw_scene(Scene& scene, OldShader* s);

  //void draw_shadows(Light l, MeshComponent* obj, TransformComponent* tran);
  std::shared_ptr<RenderTarget> get_shadow_buffer();


protected:
  Window& m_Window;
  std::shared_ptr<RenderTarget> m_RenderTarget;

private:
  bool m_bDrawOnTexture;

  FlyCamera m_Camera;

  std::shared_ptr<OldShader> m_depth_shader;
  std::shared_ptr<OldShader> m_shadow_shader;
  std::shared_ptr<RenderTarget> m_shadows_buffer_;

  std::shared_ptr<UniformBuffer> m_buffer_matrix;
  std::shared_ptr<UniformBuffer> m_buffer_ambient_light;
  std::shared_ptr<UniformBuffer> m_buffer_directional_light;
  std::shared_ptr<UniformBuffer> m_buffer_point_light;
  std::shared_ptr<UniformBuffer> m_buffer_spot_light;

};

void DrawForward(EntityComponentSystem& entity, Renderer& renderer);

}
