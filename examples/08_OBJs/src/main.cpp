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
#include "Common/Editor/Editor.h"

#include "Common/TaskSystem/TaskSystem.h"
#include "Common/Log.h"

#include "Common/Resources/ResourceManager.h"
#include "Common/ShaderTextEditor.h"

int main(int argc, char** argv){

  And::Engine e;

  And::TaskSystem ts;

  std::shared_ptr<And::Window> window = And::Window::make(e, 1024, 720, "Andromeda Engine");
  std::shared_ptr<And::GraphicsContext> g_context = window->get_context();
  And::Renderer g_renderer(*window);

  And::ResourceManager r_manager{*window, ts};
  r_manager.AddGenerator<And::ObjGenerator>();
  
  And::Editor editor;

  // Show pc info
  g_context->create_info();


  // Creamos el shader
  And::ShaderInfo s_info;
  s_info.path_fragment = "fshader.fs";
  s_info.path_vertex = "vshader.vs";

  std::optional<And::Shader> g_shader = And::Shader::make(s_info);
  


  float clear_color[4] = {0.0f, 0.0f, 0.0f, 1.0f};
  g_renderer.set_clear_color(clear_color);

  
  And::EntityComponentSystem entity_comp;
    
  entity_comp.add_component_class<And::Resource<And::ObjLoader>>();
  entity_comp.add_component_class<And::Transform>();  

  int num_obj = 10;
  float pos_x = 0.0f;
  float pos_y = -5.0f;

  for(int i = -5; i < (int)(num_obj / 2); i++){
    And::Resource<And::ObjLoader> obj_teapot = r_manager.NewResource<And::ObjLoader>("teapot.obj");
    And::Transform tran = {{pos_x + (i*6.0f), pos_y, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}};
    And::Entity obj_id = entity_comp.new_entity(obj_teapot, tran);
  }
  pos_y = 5.0f;
  for(int i = -5; i < (int)(num_obj / 2); i++){
    And::Resource<And::ObjLoader> obj_teapot = r_manager.NewResource<And::ObjLoader>("teapot.obj");
    And::Transform tran = {{pos_x + (i*6.0f), pos_y, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}};
    And::Entity obj_id = entity_comp.new_entity(obj_teapot, tran);
  }

 
  while (window->is_open()){
    window->update();
    g_renderer.new_frame();

    editor.ShowWindows();

    std::function<void(And::Transform* trans, And::Resource<And::ObjLoader>* Resource)> obj_draw =  [&g_renderer, &g_shader] (And::Transform* trans, And::Resource<And::ObjLoader>* Resource){

      g_renderer.draw_obj(*(*Resource), &g_shader.value(), *trans);
    };

    entity_comp.execute_system(obj_draw);

    g_renderer.end_frame();
    window->swap_buffers();
  }

  return 0;
}