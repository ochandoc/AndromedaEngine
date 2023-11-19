#pragma once

#include "base.h"

namespace And
{
class Window;
class Shader;
class Triangle;
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
  void showImGuiDemoWindow();

  void draw_triangle(Triangle *t);

protected:
  Window& m_Window;
};

}
