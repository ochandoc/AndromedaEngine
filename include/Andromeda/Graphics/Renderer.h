#pragma once

#include "Andromeda/Misc/CoreMiscDefines.h"
#include "Andromeda/HAL/Types.h"
#include "ObjLoader.h"
#include "Andromeda/Graphics/RenderTarget.h"
#include "Andromeda/Graphics/FlyCamera.h"
#include "Andromeda/Graphics/Light.h"
#include "Andromeda/ECS/Components/MeshComponent.h"
#include "Andromeda/ECS/Scene.h"

namespace And
{
  class Window;
  class Shader;
  class Triangle;
  class ObjLoader;
  class TransformComponent;
  struct ShaderInfo;


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

  void draw_obj(ObjLoader obj, Shader* s, Transform trans, AmbientLight* ambient, PointLight* point);
  void draw_obj(MeshComponent* obj, Shader* s, TransformComponent* trans);
  void draw_obj(MeshComponent* obj, Shader* s, TransformComponent* trans, AmbientLight* ambient, PointLight* point);
  void draw_obj(MeshComponent* obj, Shader* s, TransformComponent* trans, AmbientLight* ambient, PointLight* point, Texture* texture);
  void draw_deep_obj(MeshComponent* obj, Shader* s, TransformComponent* tran,float* view, float* projection);

  void draw_scene(Scene& scene, Shader* s);

  void draw_shadows(Light l, MeshComponent* obj, TransformComponent* tran);
  std::shared_ptr<RenderTarget> get_shadow_buffer();


protected:
  Window& m_Window;
  std::shared_ptr<RenderTarget> m_RenderTarget;

private:
  bool m_bDrawOnTexture;

  FlyCamera m_Camera;

  std::shared_ptr<Shader> m_depth_shader;
  std::shared_ptr<Shader> m_shadow_shader;
  std::shared_ptr<RenderTarget> m_shadows_buffer_;
};

}
