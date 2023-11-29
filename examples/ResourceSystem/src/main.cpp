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
#include "Common/ResourceManager.h" 
#include "Common/OpenGLTexture2D.h" 

int main(int argc, char** argv){
  try
  {
    And::Engine e;

    And::JobSystem js{ e };


    std::shared_ptr<And::Window> window = And::Window::make(e, 1024, 720, "Andromeda Engine");
    And::Renderer g_renderer(*window);

    float clear_color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    g_renderer.set_clear_color(clear_color);

    And::ResourceManager rm(*window, js);

    rm.add_resource_generator<TextureGenerator>();
    std::string Path = "./jou.jpg";
    And::resource<OpenGLTexture2D> tex = rm.new_resource<OpenGLTexture2D>(Path);
    And::resource<OpenGLTexture2D> tex2 = rm.new_resource<OpenGLTexture2D>(Path);
    And::resource<OpenGLTexture2D> tex3 = rm.new_resource<OpenGLTexture2D>("./test.png");
    And::resource<OpenGLTexture2D> tex4 = rm.new_resource<OpenGLTexture2D>("./esat_team.jpg");

    And::LogWindow log_window;
    while (window->is_open())
    {
      window->update();
      g_renderer.new_frame();

      log_window.Draw();

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