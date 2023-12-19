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


class InputComponent{
  public:

    InputComponent(And::Input* input){
      m_input = input;
    }
    InputComponent(){
     
    }
    InputComponent(const InputComponent&) = default;
    InputComponent(InputComponent&&) = default;
    ~InputComponent(){}

    InputComponent& operator=(const InputComponent& other){
      if(this != &other){
        m_input = other.m_input;
      }
      return *this;
    }

    void get_input(And::Vertex* vertex, float& speed){
      if (m_input->IsKeyDown(And::KeyCode::W) || m_input->IsKeyPressed(And::KeyCode::W)){
        for(int i = 0; i < 3; i++){
          vertex[i].position[1] += speed;
        }
      }
      if (m_input->IsKeyDown(And::KeyCode::S) || m_input->IsKeyPressed(And::KeyCode::S)){
        for(int i = 0; i < 3; i++){
          vertex[i].position[1] -= speed;
        }
      }
      
      if (m_input->IsKeyDown(And::KeyCode::A) || m_input->IsKeyPressed(And::KeyCode::A)){
        for(int i = 0; i < 3; i++){
          vertex[i].position[0] -= speed;
        }
      }
      if (m_input->IsKeyDown(And::KeyCode::D) || m_input->IsKeyPressed(And::KeyCode::D)){
        for(int i = 0; i < 3; i++){
          vertex[i].position[0] += speed;
        }
      }
    }
    
  private:
    And::Input* m_input;
};

void DrawTriangle(And::Renderer& r, And::Triangle* tri){
  r.draw_triangle(tri);
}


int main(int argc, char** argv){
  srand(time(NULL));
  And::Engine e;

  std::shared_ptr<And::Window> window = And::Window::make(e, 1024, 720, "Andromeda Engine");
  std::shared_ptr<And::GraphicsContext> g_context = window->get_context();
  And::Renderer g_renderer(*window);

  // Show pc info
  g_context->create_info();


  // Creamos el shader
  And::ShaderInfo s_info;
  s_info.path_fragment = "fshader.fs";
  s_info.path_vertex = "vshader.vs";

   std::optional<And::Shader> g_shader = And::Shader::make(s_info);


  float clear_color[4] = {0.0f, 0.0f, 0.0f, 1.0f};
  g_renderer.set_clear_color(clear_color);


  And::Input input{*window};


  And::EntityComponentSystem entity_comp;
  entity_comp.add_component_class<And::Triangle>();
  entity_comp.add_component_class<InputComponent>();
  float triangle_width = 0.01f;
  float triangle_height = 0.01f;



  for(int i = 0; i < 5000; i++){

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

    //And::Triangle tri{ver};


    
    entity_comp.new_entity(And::Triangle{ver});

  }

  And::Vertex vertices[3] = {
  {
    // {-0.5f, -0.5f, 0.0f},
    { -0.5f,-0.5f, 0.0f},
    {0.0f, 0.0f, 0.0f},
    {1.0f, 0.0f, 0.0f, 1.0f},
    {2, 1, 0}
  },
  {

    {0.0f,0.5f, 0.0f},
    {0.0f, 0.0f, 0.0f},
    {1.0f, 0.0f, 0.0f, 1.0f},
    {2, 1, 0},
  },
  {
    { 0.5f,-0.5f, 0.0f},
    {0.0f, 0.0f, 0.0f},
    {1.0f, 0.0f, 0.0f, 1.0f},
    {2, 1, 0},
  }
    
};

  InputComponent input_comp{&input};
  
  And::Entity ent = entity_comp.new_entity(And::Triangle{vertices});
  entity_comp.add_entity_component(ent, input_comp);
  And::Triangle* triangle = entity_comp.get_entity_component<And::Triangle>(ent);
  And::Vertex* vertex = triangle->get_vertex();
  
  And::LogWindow logWindow;

  float speed = 0.01f;
  while (window->is_open()){
    window->update();
    g_renderer.new_frame();
    
    logWindow.Draw();

    if(g_shader.has_value()){
      g_shader->use();
    }
    
    std::function<void(And::Triangle* tri)> f = [&g_renderer](And::Triangle* tri){
      DrawTriangle(g_renderer, tri);
    };


    std::function<void(InputComponent* input_comp)> input_system = [&vertex, &speed](InputComponent* input_comp){
      input_comp->get_input(vertex, speed);
    };


    entity_comp.execute_system(f);
    entity_comp.execute_system(input_system);

    g_renderer.end_frame();
    window->swap_buffers();
  }


  
  

  
  return 0;
}