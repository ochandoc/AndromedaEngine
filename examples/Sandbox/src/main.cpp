#include "Core/Window.h"
#include "Graphics/GraphicsContext.h"
#include "Graphics/Renderer.h"

int main(int argc, char** argv)
{
  WindowCreationInfo WindowInfo;
  WindowInfo.width = 1024;
  WindowInfo.height = 720;
  WindowInfo.api = GraphicsAPI_OpenGL;
  WindowInfo.title = "Andromeda Engine";


  std::shared_ptr<Window> window = Window::Create(WindowInfo);
  std::shared_ptr<GraphicsContext> g_context = window->create_context();
  std::shared_ptr<Renderer> g_renderer = g_context->create_renderer();

  float clear_color[4] = {1.0f, 0.0f, 0.0f, 1.0f};
  g_renderer->set_clear_color(clear_color);

  g_context->create_info();

  while (window->is_open()){
    g_renderer->clear();

    

    window->update();
  }
  

  
  return 0;
}