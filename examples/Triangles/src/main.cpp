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
#include "Common/Triangle.h"
#include "Common/Input.h"
#include "Common/ActionInput.h"
#include "Common/EntityComponentSystem.h"

#include "Common/JobSystem.h"

#include <Windows.h>

LARGE_INTEGER StartTime;
LARGE_INTEGER Freq;

void ResetTimer()
{
    QueryPerformanceCounter(&StartTime);
}

void CheckTimer(const char* Name)
{
    static LARGE_INTEGER CurrentTime;
    QueryPerformanceCounter(&CurrentTime);

    printf("%s: %fms\n", Name, (((float)(CurrentTime.QuadPart - StartTime.QuadPart) / (float)Freq.QuadPart) * 1000.0f));
}

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


void DrawTriangle(And::Renderer& r, And::Triangle* tri){
  r.draw_triangle(tri);
}



int main(int argc, char** argv){
  QueryPerformanceFrequency(&Freq);
  srand(time(NULL));
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

  // Show pc info
  g_context->create_info();


  // Creamos el shader
  And::ShaderInfo s_info;
  s_info.path_fragment = "../../data/fshader.fs";
  s_info.path_vertex = "../../data/vshader.vs";

   std::optional<And::Shader> g_shader = And::Shader::make(s_info);


  float clear_color[4] = {0.0f, 0.0f, 0.0f, 1.0f};
  g_renderer.set_clear_color(clear_color);


  And::Input input{*window};

  And::ActionInput jump{"Jump", And::KeyState::Press, { And::KeyCode::L, And::KeyCode::V }};

  double mouseX = -1.0f, mouseY = -1.0f;
  double mouseXx = -1.0f, mouseYy = -1.0f;


  And::EntityComponentSystem entity_comp;
  entity_comp.add_component_class<And::Triangle>();
  float triangle_width = 0.01f;
  float triangle_height = 0.01f;


  for(int i = 0; i < 50000; i++){

    int randomx = (rand()%2001);
    int randomy = (rand()%2001);
    float initial_posx = (((float) (randomx - 1000)) / 1000.0f);
    float initial_posy = (((float) (randomy - 1000)) / 1000.0f);
    And::Vertex ver[3] = {
      {
        // {-0.5f, -0.5f, 0.0f},
        {initial_posx, initial_posy, 0.0f},
        {0.0f, 0.0f, 0.0f},
        {1.0f, 0.0f, 0.0f, 1.0f},
        {2, 1, 0}
      },
      {

        {initial_posx + (triangle_width * 0.5), initial_posy + triangle_height, 0.0f},
        {0.0f, 0.0f, 0.0f},
        {1.0f, 0.0f, 0.0f, 1.0f},
        {2, 1, 0},
      },
      {
        {initial_posx + triangle_width, initial_posy, 0.0f},
        {0.0f, 0.0f, 0.0f},
        {1.0f, 0.0f, 0.0f, 1.0f},
        {2, 1, 0},
      }
    
    };

    And::Triangle tri{ver};


    
    entity_comp.new_entity(And::Triangle{ver});

  }



  
  And::LogWindow logWindow;

  float speed = 0.01f;
  while (window->is_open()){
    window->update();
    g_renderer.new_frame();
    
    logWindow.Draw();

    if (input.check_action(jump)){
      printf("Jummpinggggg!!!\n");
    }

    //And::Vertex *vertices = tri.get_vertex();

    /*if (input.IsKeyDown(And::KeyCode::W) || input.IsKeyPressed(And::KeyCode::W)){
      for(int i = 0; i < 3; i++){
        vertices[i].position[1] += speed;
      }
    }
    if (input.IsKeyDown(And::KeyCode::S) || input.IsKeyPressed(And::KeyCode::S)){
      for(int i = 0; i < 3; i++){
        vertices[i].position[1] -= speed;
      }
    }
    
    if (input.IsKeyDown(And::KeyCode::A) || input.IsKeyPressed(And::KeyCode::A)){
      for(int i = 0; i < 3; i++){
        vertices[i].position[0] -= speed;
      }
    }
    if (input.IsKeyDown(And::KeyCode::D) || input.IsKeyPressed(And::KeyCode::D)){
      for(int i = 0; i < 3; i++){
        vertices[i].position[0] += speed;
      }
    }

    if (input.IsKeyPressed(And::KeyCode::Space)){

      printf("Space pressed!\n");
    }

    if (input.IsKeyRelease(And::KeyCode::Space)){
      printf("Space released!\n");
    }*/

    if(g_shader.has_value()){
      g_shader->use();
    }
    
    //g_renderer.showDemo();
    //g_renderer.draw_triangle(&tri);

    std::function<void(And::Triangle* tri)> f = [&g_renderer](And::Triangle* tri){
      DrawTriangle(g_renderer, tri);
    };


    ResetTimer();
    entity_comp.execute_system(f);
    CheckTimer("System");


    //input.update_input();
    g_renderer.end_frame();
    window->swap_buffers();
  }


  
  

  
  return 0;
}