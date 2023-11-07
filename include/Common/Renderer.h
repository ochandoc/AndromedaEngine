#pragma once

#include "base.h"

namespace And
{
class Window;
class Shader;
class Triangle;
class ObjLoader;
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

  void set_clear_color(float* color);

  void showDemo();

  void draw_triangle(Triangle *t);

  void init_obj(ObjLoader* obj);
  void draw_obj(ObjLoader obj, Shader* s);

protected:
  Window& m_Window;

private:

float m_camera_pos[3];
float m_fov;
float m_aspectRatio;
float m_near;
float m_far;
};

}
