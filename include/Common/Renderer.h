#pragma once

#include <memory>

#include "Common/Window.h"

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

protected:
  Window& m_Window;
  std::unique_ptr<Window::ImGuiImpl> m_ImGuiImpl;
};

}
