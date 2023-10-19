#include <assert.h>

#include <stdio.h>
#include <iostream>
#include <fstream>

#include <functional>
#include <algorithm>
#include <utility>

#include <optional>
#include <memory>

#include <string>
#include <vector>
#include <queue>

#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>

#include "Common/JobSystem.h"
#include "Common/Engine.h"
#include "Common/Window.h"
#include "Common/GraphicsContext.h"
#include "Common/Renderer.h"
#include "Common/Shader.h"
#include "Common/JobSystem.h"

int select_num(int i)
{
  printf("Num selected\n");
  return i;
}

int select_num1(int i)
{
  std::this_thread::sleep_for(std::chrono::seconds(2));
  printf("Num selected 1\n");
  return i;
}

int select_num2(int i)
{
  std::this_thread::sleep_for(std::chrono::seconds(5));
  printf("Num selected 2\n");
  return i;
}

void print_value(int i, int a, int b)
{
  printf("Num: %d\n", i + a + b);
}

int main(int argc, char** argv){
  And::Engine e;

  And::JobSystem js;

  And::future<int> f{ 10 };
  And::future<int> f1{ 20 };
  And::future<int> f2{ 30 };

  And::future<int> future1 = js.add_job(select_num, f);
  And::future<int> future2 = js.add_job(select_num1, f1);
  And::future<int> future3 = js.add_job(select_num2, f2);

  js.add_job(print_value, future1, future2, future3);

  std::shared_ptr<And::Window> window = And::Window::make(e, 1024, 720, "Andromeda Engine");
  std::shared_ptr<And::GraphicsContext> g_context = window->get_context();
  And::Renderer g_renderer(*window);

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
    window->update();
    g_renderer.new_frame();
    
    g_shader->use();
    g_renderer.showDemo();
    
    g_renderer.end_frame();
  }
  

  
  return 0;
}