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

  std::shared_ptr<And::Window> window = And::Window::make(e, 1920, 1080, "Andromeda Engine");
  std::shared_ptr<And::GraphicsContext> g_context = window->get_context();
  And::Renderer g_renderer(*window);

  And::ResourceManager r_manager{*window, ts};
  r_manager.AddGenerator<And::ObjGenerator>();
  r_manager.AddGenerator<And::ShaderGenerator>();
  r_manager.AddGenerator<TextureGenerator>();
  
  And::Editor editor{*window, &r_manager};

  editor.AddWindow(ts.GetEditorWindow());
  // Show pc info
  g_context->create_info();

  //And::Future<int> fi = ts.AddTaskInThread("Resource Thread", SlowTask);
  //ts.AddTaskInThread("Test", WaitTask, fi);

  // Creamos el shader
  And::Resource<And::Shader> g_shader = r_manager.NewResource<And::Shader>("default/deafult_shader.shader");
  And::Resource<OpenGLTexture2D> texture = r_manager.NewResource<OpenGLTexture2D>("teapot_texture.jpg");
  //And::Resource<OpenGLTexture2D> texture = r_manager.NewResource<OpenGLTexture2D>("missing_texture.png");
  //g_shader->set_texture(texture);
  //And::Resource<And::Shader> g_shader = r_manager.NewResource<And::Shader>("default/default_shader_normals.shader");
  

  float clear_color[4] = {0.0f, 0.0f, 0.0f, 1.0f};
  g_renderer.set_clear_color(clear_color);

  
  And::EntityComponentSystem entity_comp;
    
  entity_comp.add_component_class<And::MeshComponent>();
  entity_comp.add_component_class<And::TransformComponent>();

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
    And::MeshComponent MC;
    MC.Mesh = r_manager.NewResource<And::ObjLoader>("sponza.obj");

    //std::shared_ptr<And::ObjLoader> obj_teapot = And::ObjLoader::load("teapot.obj");
    //And::Transform tran = {{pos_x + (i*6.0f), pos_y, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}};
    And::TransformComponent tran;
    tran.position[0] = 0.0f;
    tran.position[1] = 0.0f;
    tran.position[2] = 0.0f;
    tran.rotation[0] = 0.0f;
    tran.rotation[1] = 1.0f;
    tran.rotation[2] = 0.0f;
    tran.scale[0] = 1.0f;
    tran.scale[1] = 1.0f;
    tran.scale[2] = 1.0f;
    And::Entity* obj_id = entity_comp.new_entity(MC, tran);
  //}

  And::AmbientLight ambient;
  ambient.enabled = 1.0f;
  ambient.diffuse_color[0] = 1.0f;
  ambient.diffuse_color[1] = 1.0f;
  ambient.diffuse_color[2] = 1.0f;
  ambient.specular_color[0] = 1.0f;
  ambient.specular_color[1] = 1.0f;
  ambient.specular_color[2] = 1.0f;
  ambient.direction[0] = 1.0f;
  ambient.direction[1] = 1.0f;
  ambient.direction[2] = 0.0f;

  ambient.specular_strength = 0.5f;
  ambient.specular_shininess = 1.0f;

  And::PointLight point;
  point.enabled = 1.0f;
  point.position[0] = -5.0f;
  point.position[1] = 7.0f;
  point.position[2] = 0.0f;
  point.diffuse_color[0] = 1.0f;
  point.diffuse_color[1] = 0.55f;
  point.diffuse_color[2] = 0.0f;
  point.specular_color[0] = 1.0f;
  point.specular_color[1] = 1.0f;
  point.specular_color[2] = 1.0f;
  point.specular_strength = 0.5f;
  point.specular_shininess = 1.0f;
  point.constant_att = 1.0f;
  point.linear_att = 0.7f;
  point.quadratic_att = 1.8f;
  point.attenuation = 40.0f;

  g_renderer.set_draw_on_texture(true);
  while (window->is_open()){
    window->update();
    g_renderer.new_frame();

    editor.ShowWindows();


    for (auto [transform, obj] : entity_comp.get_components<And::TransformComponent, And::MeshComponent>())
    {
      g_renderer.draw_obj(obj, &(*g_shader), transform, &ambient, &point);
    }

    g_renderer.get_render_target()->Test();

    //ambient.direction[0] -= 0.0001f;
    //printf("Direction0: %f\n", ambient.direction[0]);

    g_renderer.end_frame();
    window->swap_buffers();
  }

  return 0;
}