#include <assert.h>

#include <stdio.h>
#include <iostream>
#include <fstream>

#include <chrono>
#include <functional>
#include <algorithm>
#include <utility>

#include <optional>

#include <memory>
#include <string>
#include <vector>
#include <queue>
#include <unordered_map>
#include <tuple>

#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>

#include "Andromeda.h"

int main(int argc, char** argv){

  try
  {
    And::Engine e;

    And::TaskSystem ts;


    std::shared_ptr<And::Window> window = And::Window::make(e, 1024, 720, "Andromeda Engine");
    And::Renderer g_renderer(*window);

    float clear_color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    g_renderer.set_clear_color(clear_color);

    And::ResourceManager rm(*window, ts);

    rm.AddGenerator<TextureGenerator>();
    And::Resource<OpenGLTexture2D> tex = rm.NewResource<OpenGLTexture2D>("./jou.jpg");
    And::Resource<OpenGLTexture2D> tex2 = rm.NewResource<OpenGLTexture2D>("vm_esat.png");
    And::Resource<OpenGLTexture2D> tex3 = rm.NewResource<OpenGLTexture2D>("./test.jpg");
    And::Resource<OpenGLTexture2D> tex4 = rm.NewResource<OpenGLTexture2D>("./esat_team.jpg");

    And::LogWindow log_window("Name");
    while (window->is_open())
    {
      window->update();
      g_renderer.new_frame();

      log_window.Show();

      //...
      tex->draw_in_imgui(0);
      tex2->draw_in_imgui(1);
      tex3->draw_in_imgui(2);
      tex4->draw_in_imgui(3);


      g_renderer.end_frame();
      window->swap_buffers();
    }
  }
  catch (const std::exception& e)
  {
    std::cout << e.what() << "\n";
  }
  return 0;
}