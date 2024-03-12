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
#include "Andromeda/Graphics/Lights/DirectionalLight.h"
#include "Andromeda/Graphics/Lights/PointLight.h"

namespace And
{
  class Window;
  class OldShader;
  class Triangle;
  class ObjLoader;
  class TransformComponent;
  struct OldShaderInfo;
  class UniformBuffer;

  struct Direction;

class Renderer
{
  NON_COPYABLE_CLASS(Renderer)
  NON_MOVABLE_CLASS(Renderer)
public:
  Renderer(Window& window);
  ~Renderer();

  /**
   * @brief Must called at begining after window->update
   * 
   */
  void new_frame();

  /**
   * @brief Must called at end of the frame before swap buffers
   * 
   */
  void end_frame();

  /**
   * @brief Set the viewport of the window
   * 
   * @param x position
   * @param y position
   * @param width size
   * @param height size
   */
  void set_viewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height);

  /**
   * @brief Set clear color value 
   * 
   * @param color 
   */
  void set_clear_color(float* color);

  /**
   * @brief Show Demo window
   * 
   */
  void showDemo();

  /**
   * @brief Show Imgui Demo window
   * 
   */
  void showImGuiDemoWindow();

  /**
   * @brief Draw Forward Rendering
   * 
   * @param entity 
   * @param renderer 
   */
  friend void DrawForward(EntityComponentSystem& entity, Renderer& renderer);

  private:
  void draw_triangle(Triangle *t);
  void draw_obj(MeshComponent* obj, Light* l, TransformComponent* tran);
  void draw_obj_shadows(MeshComponent* obj, TransformComponent* trans, SpotLight* l);
  void draw_obj_shadows(MeshComponent* obj, TransformComponent* trans, DirectionalLight* l);
  void draw_obj_shadows(MeshComponent* obj, TransformComponent* trans, PointLight* l, float* dir);
  void draw_deep_obj(MeshComponent* obj, std::shared_ptr<Shader> s, TransformComponent* tran, float* view, float* projection);

  void draw_scene(Scene& scene, Shader* s);

  void draw_shadows(SpotLight* l, MeshComponent* obj, TransformComponent* tran);
  void draw_shadows(DirectionalLight* l, MeshComponent* obj, TransformComponent* tran);
  void draw_shadows(PointLight* l, MeshComponent* obj, TransformComponent* tran, float* dir);

  std::shared_ptr<RenderTarget> get_shadow_buffer();
  std::vector<std::shared_ptr<RenderTarget>> get_shadow_buffer_pointLight();

protected:
  Window& m_Window;
  std::shared_ptr<RenderTarget> m_RenderTarget;

private:
  bool m_bDrawOnTexture;

  FlyCamera m_Camera;

  std::shared_ptr<RenderTarget> m_shadows_buffer_;
  std::vector<std::shared_ptr<RenderTarget>> m_shadows_buffer_pointLight;

  std::shared_ptr<Shader> m_shadow_shader;
  std::shared_ptr<Shader> m_depth_shader;
  
  std::shared_ptr<Shader> m_shader_ambient;
  std::shared_ptr<Shader> m_shader_directional;
  std::shared_ptr<Shader> m_shader_shadows_directional;
  std::shared_ptr<Shader> m_shader_point;
  std::shared_ptr<Shader> m_shader_shadows_point;
  std::shared_ptr<Shader> m_shader_spot;
  std::shared_ptr<Shader> m_shader_shadows_spot;

  std::shared_ptr<UniformBuffer> m_buffer_matrix; // 208
  std::shared_ptr<UniformBuffer> m_buffer_matrix_pointLight; // 208 + 16 * 5
  std::shared_ptr<UniformBuffer> m_buffer_ambient_light; // 48
  std::shared_ptr<UniformBuffer> m_buffer_directional_light; // 48
  std::shared_ptr<UniformBuffer> m_buffer_point_light; // 64
  std::shared_ptr<UniformBuffer> m_buffer_spot_light; // 96

  std::shared_ptr<Direction> m_directions;

};

void DrawForward(EntityComponentSystem& entity, Renderer& renderer);

}
