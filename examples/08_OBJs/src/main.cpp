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
#include "Andromeda/ECS/Scene.h"


int main(int argc, char** argv){

  And::Engine e;

  And::TaskSystem ts;

  And::WorkerCreationInfo workerCreationInfo;
  workerCreationInfo.Name = "Test";
  workerCreationInfo.Function = And::GetGenericWorkerFunction();
  workerCreationInfo.UserData = nullptr;

  ts.AddWorker(workerCreationInfo);

  std::shared_ptr<And::Window> window = And::Window::make(e, 1920, 1080, "Andromeda Engine");
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
  /*And::ShaderInfo s_info;
  s_info.path_fragment = "fshader.fs";
  s_info.path_vertex = "vshader.vs";*/

  //And::Resource<And::Shader> g_shader = r_manager.NewResource<And::Shader>("content/teapot_shader.ashader");

  //auto shader = And::Shader::make("default/depth.shader");

  auto shader = And::Shader::make("content/teapot_shader.ashader");

  float clear_color[4] = {0.0f, 0.0f, 0.0f, 1.0f};
  g_renderer.set_clear_color(clear_color);

  
  And::EntityComponentSystem entity_comp;
    
  entity_comp.add_component_class<And::MeshComponent>();
  entity_comp.add_component_class<And::TransformComponent>();

  int num_obj = 10;
  float pos_x = 0.0f;
  float pos_y = -5.0f;

  And::Scene scene;

  for (int i = -5; i < (int)(num_obj / 2); i++) {
    And::MeshComponent MC;
    MC.Mesh = r_manager.NewResource<And::ObjLoader>("teapot.obj");
    And::Entity* ett = scene.NewSceneEntity();
    ett->add_component(MC);
    And::TransformComponent* tran = ett->get_component<And::TransformComponent>();
    tran->position[0] = pos_x + (i * 6.0f);
    tran->position[1] = pos_y;
    tran->position[2] = 0.0f;
    tran->rotation[0] = 0.0f;
    tran->rotation[1] = 1.0f;
    tran->rotation[2] = 0.0f;
    tran->scale[0] = 1.0f;
    tran->scale[1] = 1.0f;
    tran->scale[2] = 1.0f;
  }
  pos_y = 5.0f;
  for (int i = -5; i < (int)(num_obj / 2); i++) {
    And::MeshComponent MC;
    MC.Mesh = r_manager.NewResource<And::ObjLoader>("teapot.obj");
    And::Entity* ett = scene.NewSceneEntity();
    ett->add_component(MC);
    And::TransformComponent* tran = ett->get_component<And::TransformComponent>();
    tran->position[0] = pos_x + (i * 6.0f);
    tran->position[1] = pos_y;
    tran->position[2] = 0.0f;
    tran->rotation[0] = 0.0f;
    tran->rotation[1] = 1.0f;
    tran->rotation[2] = 0.0f;
    tran->scale[0] = 1.0f;
    tran->scale[1] = 1.0f;
    tran->scale[2] = 1.0f;
  }

  g_renderer.set_draw_on_texture(true);


  while (window->is_open()){
    window->update();
    g_renderer.new_frame();

    editor.ShowWindows();

    g_renderer.draw_scene(scene, shader.get());

    g_renderer.get_render_target()->Test();

    g_renderer.end_frame();
    window->swap_buffers();
  }

  return 0;
}
