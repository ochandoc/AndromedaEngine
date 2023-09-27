#pragma once

#include "Graphics/Renderer.h"

namespace And
{

class OpenGLRenderer : public Renderer
{
public:
  OpenGLRenderer(Window& window);

  virtual ~OpenGLRenderer();

  virtual void new_frame() override;
  virtual void end_frame() override;

  virtual void set_viewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height) override;
  virtual void set_clear_color(float* color) override;
};

}
