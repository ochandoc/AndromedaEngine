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
#include "Common/ShaderGenerator.h"

#include "Common/Input.h"
#include "Common/ActionInput.h"
#include "Common/EntityComponentSystem.h"
#include "Common/Editor/Editor.h"
#include "Common/Light.h"

#include "Common/TaskSystem/TaskSystem.h"
#include "Common/Log.h"

#include "Common/Resources/ResourceManager.h"
#include "Common/ShaderTextEditor.h"

int SlowTask()
{
  std::this_thread::sleep_for(std::chrono::seconds(1));
  return 10;
}

void WaitTask(int num)
{
  printf("Num: %d\n", num);
}

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

  //And::Future<int> fi = ts.AddTaskInThread("Resource Thread", SlowTask);
  //ts.AddTaskInThread("Test", WaitTask, fi);

  // Creamos el shader
  //And::Resource<And::Shader> g_shader = r_manager.NewResource<And::Shader>("default/default_shader_normals.shader");
  And::Resource<And::Shader> g_shader = r_manager.NewResource<And::Shader>("default/default_shader_normals.shader");
  

  float clear_color[4] = {0.0f, 0.0f, 0.0f, 1.0f};
  g_renderer.set_clear_color(clear_color);

  
  And::EntityComponentSystem entity_comp;
    
  entity_comp.add_component_class<And::Resource<And::ObjLoader>>();
  entity_comp.add_component_class<And::Transform>();

  int num_obj = 10;
  float pos_x = 0.0f;
  float pos_y = -5.0f;

  /*for(int i = -5; i < (int)(num_obj / 2); i++){
    And::Resource<And::ObjLoader> obj_teapot = r_manager.NewResource<And::ObjLoader>("teapot.obj");
    And::Transform tran = {{pos_x + (i*6.0f), pos_y, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}};
    And::Entity obj_id = entity_comp.new_entity(obj_teapot, tran);
  }
  pos_y = 5.0f;
  for(int i = -5; i < (int)(num_obj / 2); i++){
    And::Resource<And::ObjLoader> obj_teapot = r_manager.NewResource<And::ObjLoader>("teapot.obj");
    And::Transform tran = {{pos_x + (i*6.0f), pos_y, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}};
    And::Entity obj_id = entity_comp.new_entity(obj_teapot, tran);
  }*/

  //for(int i = -5; i < 5; i++){
    And::Resource<And::ObjLoader> obj_teapot = r_manager.NewResource<And::ObjLoader>("teapot.obj");
    //std::shared_ptr<And::ObjLoader> obj_teapot = And::ObjLoader::load("teapot.obj");
    //And::Transform tran = {{pos_x + (i*6.0f), pos_y, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}};
    And::Transform tran = {{0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}};
    And::Entity obj_id = entity_comp.new_entity(obj_teapot, tran);
  //}

  float active;
  float direction[3];
  float diffuse_color[3];
  float specular_color[3];
  float specular_strength;
  float specular_shininess;

  And::AmbientLight ambient;
  ambient.active = 1.0f;
  ambient.diffuse_color[0] = 1.0f;
  ambient.diffuse_color[1] = 1.0f;
  ambient.diffuse_color[2] = 1.0f;
  ambient.specular_color[0] = 1.0f;
  ambient.specular_color[1] = 1.0f;
  ambient.specular_color[2] = 1.0f;
  ambient.specular_strength = 1.0f;
  ambient.specular_shininess = 1.0f;
 
  while (window->is_open()){
    window->update();
    g_renderer.new_frame();

    editor.ShowWindows();

    std::function<void(And::Transform* trans, And::Resource<And::ObjLoader>* resource)> obj_draw =  [&g_renderer, &g_shader, &ambient] (And::Transform* trans, And::Resource<And::ObjLoader>* resource){
      g_renderer.draw_obj(*(*resource), &(*g_shader), *trans, &ambient);
    };
    
    /*std::function<void(And::Transform* trans, std::shared_ptr<And::ObjLoader> resource)> obj_draw =  [&g_renderer, &g_shader, &ambient] (And::Transform* trans, And::Resource<And::ObjLoader>* resource){
      g_renderer.draw_obj(*resource, &(*g_shader), *trans, &ambient);
    };*/

    

    entity_comp.execute_system(obj_draw);

    g_renderer.end_frame();
    window->swap_buffers();
  }

  return 0;
}