#pragma once

namespace And
{

class Renderer
{
public:
  Renderer();
  Renderer(const Renderer&) = delete;
  Renderer(const Renderer&&) = delete;

  virtual ~Renderer();

  Renderer& operator =(const Renderer&) = delete;
  Renderer& operator =(Renderer&&) = delete;

  virtual void set_viewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height) = 0;

  virtual void set_clear_color(float* color) = 0;

  virtual void clear() = 0;
};

}
