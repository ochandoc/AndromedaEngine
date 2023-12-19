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
#include "Common/ObjLoader.h"
#include "Common/ObjGenerator.h"

#include "Common/Input.h"
#include "Common/ActionInput.h"
#include "Common/EntityComponentSystem.h"

#include "Common/JobSystem.h"
#include "Common/Log.h"

#include "Common/ResourceManager.h"

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

  And::JobSystem js{e};



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

  And::ResourceManager r_manager{*window, js};
  r_manager.add_resource_generator<And::ObjGenerator>();
  

  // Show pc info
  g_context->create_info();


  // Creamos el shader
  And::ShaderInfo s_info;
  s_info.path_fragment = "fshader.fs";
  s_info.path_vertex = "vshader.vs";

  std::optional<And::Shader> g_shader = And::Shader::make(s_info);
  //std::optional<And::ObjLoader> obj_loaded = And::ObjLoader::load("../../data/boat/boat.obj", "../../data/boat/");
  //std::shared_ptr<And::ObjLoader> obj_loaded = And::ObjLoader::load("../../data/cube/cube.obj", "../../data/cube/");
  //std::optional<And::ObjLoader> obj_loaded = And::ObjLoader::load("../../data/cloud/cloud.obj", "../../data/cloud/");
  //std::shared_ptr<And::ObjLoader> obj_loaded = And::ObjLoader::load("../../data/teapot/teapot.obj", "../../data/teapot/");
  
  //if(obj_loaded){
    //g_renderer.init_obj(obj_loaded.get());
  //}


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




  entity_comp.add_component_class<And::resource<And::ObjLoader>>();
  entity_comp.add_component_class<And::Transform>();  

  int num_obj = 10;
  float pos_x = 0.0f;
  float pos_y = -5.0f;

  for(int i = -5; i < (int)(num_obj / 2); i++){
    And::resource<And::ObjLoader> obj_teapot = r_manager.new_resource<And::ObjLoader>("teapot.obj");
    And::Transform tran = {{pos_x + (i*6.0f), pos_y, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}};
    And::Entity obj_id = entity_comp.new_entity(obj_teapot, tran);
  }
  pos_y = 5.0f;
  for(int i = -5; i < (int)(num_obj / 2); i++){
    And::resource<And::ObjLoader> obj_teapot = r_manager.new_resource<And::ObjLoader>("teapot.obj");
    And::Transform tran = {{pos_x + (i*6.0f), pos_y, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}};
    And::Entity obj_id = entity_comp.new_entity(obj_teapot, tran);
  }



  float speed = 0.01f;

  //std::optional<And::ObjLoader> obj_loaded = And::ObjLoader::load("../../data/faro/faro.obj"); 
 


  while (window->is_open()){
    window->update();
    g_renderer.new_frame();
    
    if (input.check_action(jump)){
      printf("Jummpinggggg!!!\n");
    }

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

    if (input.IsKeyPressed(And::KeyCode::Space)){

      printf("Space pressed!\n");
    }

    if (input.IsKeyRelease(And::KeyCode::Space)){
      printf("Space released!\n");
    }

    /*if(g_shader.has_value()){
      g_shader->use();
    }*/
    
    //g_renderer.showDemo();
    //g_renderer.draw_triangle(&tri);
    //g_renderer.draw_obj(*obj_loaded, &g_shader.value());

    std::function<void(And::Transform* trans, And::resource<And::ObjLoader>* resource)> obj_draw =  [&g_renderer, &g_shader] (And::Transform* trans, And::resource<And::ObjLoader>* resource){

      g_renderer.draw_obj(*(*resource), &g_shader.value(), *trans);
    };

    entity_comp.execute_system(obj_draw);

    //g_renderer.draw_obj(*obj_teapot, &g_shader.value(), tran);


    //input.update_input();
    g_renderer.end_frame();
    window->swap_buffers();
  }


  
  

  
  return 0;
}