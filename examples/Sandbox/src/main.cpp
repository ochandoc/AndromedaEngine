#include "Core/Window.h"
#include "Graphics/GraphicsContext.h"
#include "Graphics/Renderer.h"
#include "Graphics/Shader.h"
#include <string>

int main(int argc, char** argv){

  
  std::string vs_test = 
    "#version 430 core\n "

    "layout(location = 0) in vec2 position;\n"
    "out vec2 blend_color;\n"

    "void main(){\n"
      "gl_Position = vec4(position.xy, 0.0, 1.0);\n"
      "blend_color = position;\n"
    "}\n";

  std::string fs_test = 
    "#version 430 core\n"

    "out vec4 FragColor;\n"
    "in vec2 blend_color;\n"

    "void main(){\n"
      "//FragColor = vec4(0.0, 1.0, 0.0, 1.0);\n"
      "FragColor = vec4(blend_color.xy, 0.0, 1.0);\n"
      
    "}\n";
  
  And::ShaderInfo vs_info = {
    And::Shader_Vertex,
    vs_test,
  };
  And::ShaderInfo fs_info = {
    And::Shader_Fragment,
    fs_test,
  };

  std::vector<And::ShaderInfo> shaders_vec;
  shaders_vec.push_back(vs_info);
  shaders_vec.push_back(fs_info);



  And::WindowCreationInfo WindowInfo;
  WindowInfo.width = 1024;
  WindowInfo.height = 720;
  WindowInfo.api = And::GraphicsAPI_OpenGL;
  WindowInfo.title = "Andromeda Engine";


  std::shared_ptr<And::Window> window = And::Window::Create(WindowInfo);
  std::shared_ptr<And::GraphicsContext> g_context = window->create_context();
  std::shared_ptr<And::Renderer> g_renderer = g_context->create_renderer();
  std::shared_ptr<And::Shader> g_shader = g_renderer->createShader(shaders_vec);

  float clear_color[4] = {0.0f, 0.0f, 0.0f, 1.0f};
  g_renderer->set_clear_color(clear_color);

  // Show pc info
  g_context->create_info();

  
  while (window->is_open()){
    g_renderer->clear();

    g_renderer->showDemo();

    window->update();
  }
  

  
  return 0;
}