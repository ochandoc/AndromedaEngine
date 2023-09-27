#include "Core/Window.h"
#include "Graphics/GraphicsContext.h"
#include "Graphics/Renderer.h"
#include "Graphics/Shader.h"
#include <string>

int main(int argc, char** argv){

  std::string fs_test = 
  "#version 430\n"

  "layout(location = 0) out vec4 color;\n"
  
  "void main{\n"
    "color = vec4(0.0, 1.0, 0.0, 1.0);\n"
  "}";
  
  std::string vs_test = 
  "#version 430\n "

  "layout(location = 0) in vec4 position;\n"
  "void main{\n"
    "gl_Position = position;\n"
  "}";

  And::WindowCreationInfo WindowInfo;
  WindowInfo.width = 1024;
  WindowInfo.height = 720;
  WindowInfo.api = And::GraphicsAPI_OpenGL;
  WindowInfo.title = "Andromeda Engine";


  std::shared_ptr<And::Window> window = And::Window::Create(WindowInfo);
  std::shared_ptr<And::GraphicsContext> g_context = window->create_context();
  std::shared_ptr<And::Renderer> g_renderer = g_context->create_renderer();

  And::ShaderInfo s_info = {
    And::Shader_Vertex,
    vs_test,
  };

  std::vector<And::ShaderInfo> shaders_vec;
  shaders_vec.push_back(s_info);
  std::shared_ptr<And::Shader> g_shader = g_renderer->createShader(shaders_vec);

  float clear_color[4] = {0.0f, 0.0f, 0.0f, 1.0f};
  g_renderer->set_clear_color(clear_color);

  g_context->create_info();


  float triangle[6] = {
    -0.5f, -0.5f,
    0.0f, 0.5f,
    0.5f, -0.5f,
  };

  g_renderer->showDemo(triangle);
  
  while (window->is_open()){
    g_renderer->clear();

    g_renderer->printDemo();

    window->update();
  }
  

  
  return 0;
}