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

#include <algorithm>
#include <utility>
#include <string>

int main(int argc, char** argv){
  And::WindowCreationInfo WindowInfo;
  WindowInfo.width = 1024;
  WindowInfo.height = 720;
  WindowInfo.title = "Andromeda Engine";


  std::shared_ptr<And::Window> window(new And::Window(WindowInfo));
  std::shared_ptr<And::GraphicsContext> g_context = window->get_context();
  And::Renderer& g_renderer = window->create_renderer();

  // Create basic shader
  And::ShaderInfo vs_info = { And::Shader_Vertex, "../../data/vshader.vs" };
  And::ShaderInfo fs_info = { And::Shader_Fragment, "../../data/fshader.fs" };
  std::vector<And::ShaderInfo> shaders_vec;
  shaders_vec.push_back(vs_info);
  shaders_vec.push_back(fs_info);

  std::shared_ptr<And::Shader> g_shader = g_renderer.createShader(shaders_vec);

  float clear_color[4] = {0.0f, 0.0f, 0.0f, 1.0f};
  g_renderer.set_clear_color(clear_color);

  // Show pc info
  g_context->create_info();

  
  while (window->is_open()){
    g_renderer.new_frame();
    
    g_renderer.showDemo();
    
    g_renderer.end_frame();
  }
  

  
  return 0;
}