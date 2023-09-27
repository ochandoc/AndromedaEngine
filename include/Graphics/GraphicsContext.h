#pragma once

#include <memory>

namespace And
{

enum GraphicsAPI
{
  GraphicsAPI_OpenGL,
};

class Renderer;

class GraphicsContext
{  
public:
  GraphicsContext();
  GraphicsContext(const GraphicsContext&) = delete;
  GraphicsContext(const GraphicsContext&&) = delete;

  virtual ~GraphicsContext();

  GraphicsAPI& operator =(const GraphicsAPI&) = delete;
  GraphicsAPI& operator =(const GraphicsAPI&&) = delete;

  virtual std::shared_ptr<Renderer> create_renderer() = 0;

  virtual void create_info() = 0;

};

}
