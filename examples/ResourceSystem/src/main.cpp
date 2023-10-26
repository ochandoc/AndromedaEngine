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

#include "Common/OpenGLTexture2D.h"

int main(int argc, char** argv){
  And::Engine e;

  And::JobSystem js;


  std::shared_ptr<And::Window> window = And::Window::make(e, 1024, 720, "Andromeda Engine");
  And::Renderer g_renderer(*window);

  float clear_color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
  g_renderer.set_clear_color(clear_color);

  And::ResourceManager rm(*window, js);

  And::resource<OpenGLTexture2D> tex = rm.new_texture2D("./test.jpg");
  

  while (window->is_open()) {
    window->update();
    g_renderer.new_frame();

    //...
    tex->draw_in_imgui();
    

    g_renderer.end_frame();
    window->swap_buffers();
  }

  return 0;
}