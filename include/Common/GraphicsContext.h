#pragma once

#include <memory>

namespace And
{

enum GraphicsAPI
{
  GraphicsAPI_OpenGL,
};

class Window;

class GraphicsContext
{  
private:
  GraphicsContext(Window& window);

public:
  GraphicsContext(const GraphicsContext&) = delete;
  GraphicsContext(const GraphicsContext&&) = delete;

  ~GraphicsContext();

  GraphicsAPI& operator =(const GraphicsAPI&) = delete;
  GraphicsAPI& operator =(const GraphicsAPI&&) = delete;

  void create_info();

  friend class Window;
private:
  std::unique_ptr<struct ContextData> m_Data;
};

}
