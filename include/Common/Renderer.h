#pragma once

#include <memory>

#include "Common/Window.h"
#include "Common/Shader.h"

namespace And
{

class Renderer
{
public:
  Renderer(Window& window);
  Renderer(const Renderer&) = delete;
  Renderer(const Renderer&&) = delete;

  ~Renderer();

  Renderer& operator =(const Renderer&) = delete;
  Renderer& operator =(Renderer&&) = delete;

  void new_frame();
  void end_frame();

  void set_viewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height);

  void set_clear_color(float* color);

  void showDemo();

  std::shared_ptr<Shader> createShader(std::vector<ShaderInfo> s_info);

protected:
  Window& m_Window;
};

}
