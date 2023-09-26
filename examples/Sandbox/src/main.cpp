#include "Core/Window.h"
#include "Graphics/GraphicsContext.h"

int main(int argc, char** argv)
{
  WindowCreationInfo WindowInfo;
  WindowInfo.width = 1024;
  WindowInfo.height = 720;
  WindowInfo.api = GraphicsAPI_OpenGL;
  WindowInfo.title = "Andromeda Engine";


  std::shared_ptr<Window> window = Window::Create(WindowInfo);
  std::shared_ptr<GraphicsContext> g_context = window->create_context();
  g_context->create_info();

  while (window->is_open()){

    window->update();
  }
  

  
  return 0;
}