#pragma once

#include <memory>

namespace And
{

enum GraphicsAPI
{
  GraphicsAPI_OpenGL,
};

class GraphicsContext
{  
public:
  GraphicsContext();
  GraphicsContext(const GraphicsContext&) = delete;
  GraphicsContext(const GraphicsContext&&) = delete;

  virtual ~GraphicsContext();

  GraphicsAPI& operator =(const GraphicsAPI&) = delete;
  GraphicsAPI& operator =(const GraphicsAPI&&) = delete;

  virtual void create_info() = 0;

};

}
