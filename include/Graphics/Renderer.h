#pragma once

#include <memory>

#include "Core/Window.h"

namespace And
{

class Renderer
{
public:
  Renderer(Window& window);
  Renderer(const Renderer&) = delete;
  Renderer(const Renderer&&) = delete;

  virtual ~Renderer();

  Renderer& operator =(const Renderer&) = delete;
  Renderer& operator =(Renderer&&) = delete;

  virtual void new_frame() = 0;
  virtual void end_frame() = 0;

  virtual void set_viewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height) = 0;

  virtual void set_clear_color(float* color) = 0;

protected:
  Window& m_Window;
  std::unique_ptr<Window::ImGuiImpl> m_ImGuiImpl;
};

}
