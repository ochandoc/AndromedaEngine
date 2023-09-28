#pragma once

#include "Graphics/Renderer.h"

namespace And
{

class OpenGLRenderer : public Renderer
{
public:
  OpenGLRenderer();

  virtual ~OpenGLRenderer();

  virtual void set_viewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height) override;
  
  virtual void set_clear_color(float* color) override;

  virtual void clear() override;

  virtual std::shared_ptr<Shader> createShader(std::vector<ShaderInfo> s_info) override;
  
  // You must create a shader before show demo
  virtual void showDemo() override;
};

}
