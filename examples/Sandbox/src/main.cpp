#include <assert.h>

#include <functional>
#include <algorithm>
#include <utility>

#include <optional>
#include <memory>

#include <string>
#include <vector>

#include "Common/Window.h"
#include "Common/GraphicsContext.h"
#include "Common/Renderer.h"

int main(int argc, char** argv)
{
  And::WindowCreationInfo WindowInfo;
  WindowInfo.width = 1024;
  WindowInfo.height = 720;
  WindowInfo.title = "Andromeda Engine";


  std::shared_ptr<And::Window> window(new And::Window(WindowInfo));
  std::shared_ptr<And::GraphicsContext> g_context = window->get_context();
  And::Renderer& g_renderer = window->create_renderer();

  float clear_color[4] = {1.0f, 0.0f, 0.0f, 1.0f};
  //g_renderer.set_clear_color(clear_color);

  g_context->create_info();

  while (window->is_open()){
    g_renderer.new_frame();
    
  
    g_renderer.end_frame();
  }
  

  
  return 0;
}