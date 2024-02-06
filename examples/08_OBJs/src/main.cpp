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

  And::TaskSystem ts;

  And::WorkerCreationInfo workerCreationInfo;
  workerCreationInfo.Name = "Test";
  workerCreationInfo.Function = And::GetGenericWorkerFunction();
  workerCreationInfo.UserData = nullptr;

  ts.AddWorker(workerCreationInfo);

  std::shared_ptr<And::Window> window = And::Window::make(e, 1024, 720, "Andromeda Engine");
  std::shared_ptr<And::GraphicsContext> g_context = window->get_context();
  And::Renderer g_renderer(*window);

  And::ResourceManager r_manager{*window, ts};
  r_manager.AddGenerator<And::ObjGenerator>();
  r_manager.AddGenerator<And::ShaderGenerator>();
  
  And::Editor editor{*window, &r_manager};

  editor.AddWindow(ts.GetEditorWindow());
  // Show pc info
  g_context->create_info();

  // Creamos el shader
  And::ShaderInfo s_info;
  s_info.path_fragment = "fshader.fs";
  s_info.path_vertex = "vshader.vs";

  //And::Resource<And::Shader> g_shader = r_manager.NewResource<And::Shader>("content/teapot_shader.ashader");

  auto shader = And::Shader::make("default/depth.shader");

  //auto shader = And::Shader::make("content/teapot_shader.ashader");

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

  g_renderer.set_draw_on_texture(true);


  while (window->is_open()){
    window->update();
    g_renderer.new_frame();

    editor.ShowWindows();

    for (auto [transform, obj] : entity_comp.get_components<And::Transform, And::Resource<And::ObjLoader>>())
    {
      g_renderer.draw_obj(*(*obj), shader.get(), *transform);
    }

    g_renderer.get_render_target()->Test();

    g_renderer.end_frame();
    window->swap_buffers();
  }

  return 0;
}
