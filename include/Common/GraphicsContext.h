#pragma once

namespace And
{
class Window;

class GraphicsContext
{  
private:
  GraphicsContext(Window& window);

public:
  GraphicsContext(const GraphicsContext&) = delete;
  GraphicsContext(const GraphicsContext&&) = delete;

  ~GraphicsContext();

  GraphicsContext& operator =(const GraphicsContext&) = delete;
  GraphicsContext& operator =(const GraphicsContext&&) = delete;

  void create_info();

  friend class Window;
private:
  std::unique_ptr<struct ContextData> m_Data;
};

}
