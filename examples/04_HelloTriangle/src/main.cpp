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

#include "Andromeda.h"


int main(int argc, char** argv){

  And::Engine e;

  std::shared_ptr<And::Window> window = And::Window::make(e, 1024, 720, "Andromeda Engine");
  std::shared_ptr<And::GraphicsContext> g_context = window->get_context();
  And::Renderer g_renderer(*window);

  // Show pc info
  g_context->create_info();


  // Creamos el shader
  And::ShaderInfo s_info;
  s_info.path_fragment = "fshader.fs";
  s_info.path_vertex = "vshader_tri.vs";

   std::shared_ptr<And::Shader> g_shader = And::Shader::make(s_info);


  float clear_color[4] = {0.0f, 0.0f, 0.0f, 1.0f};
  g_renderer.set_clear_color(clear_color);


  And::Input input{*window};

  And::ActionInput jump{"Jump", And::KeyState::Press, { And::KeyCode::L, And::KeyCode::V }};

  double mouseX = -1.0f, mouseY = -1.0f;
  double mouseXx = -1.0f, mouseYy = -1.0f;

  
    

  And::Vertex ver[3] = {
    {
      {-0.5f, -0.5f, 0.0f},
      {0.0f, 0.0f, 0.0f},
      {1.0f, 0.0f, 0.0f, 1.0f},
      {2, 1, 0}
    },
    {

      {0.0f, 0.5f, 0.0f},
      {0.0f, 0.0f, 0.0f},
      {1.0f, 0.0f, 0.0f, 1.0f},
      {2, 1, 0},
    },
    {
      {0.5f, -0.5f, 0.0f},
      {0.0f, 0.0f, 0.0f},
      {1.0f, 0.0f, 0.0f, 1.0f},
      {2, 1, 0},
    }
  
  };



  And::Triangle tri{ver};

  And::EntityComponentSystem entity_comp;

  entity_comp.add_component_class<And::Triangle>();
  entity_comp.new_entity(And::Triangle{ver});


  float speed = 0.01f;
  while (window->is_open()){
    window->update();
    g_renderer.new_frame();

    And::Vertex *vertices = tri.get_vertex();

    if (input.IsKeyDown(And::KeyCode::W) || input.IsKeyPressed(And::KeyCode::W)){
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

    if(g_shader){
      g_shader->use();
    }
    
    g_renderer.draw_triangle(&tri);
    
    g_renderer.end_frame();
    window->swap_buffers();
  }


  
  

  
  return 0;
}