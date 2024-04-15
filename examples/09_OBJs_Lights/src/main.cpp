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
#include <chrono>
#include "Andromeda.h"



int SlowTask()
{
  std::this_thread::sleep_for(std::chrono::seconds(1));
  return 10;
}

void WaitTask(int num){
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

  std::shared_ptr<And::Window> window = And::Window::make(e, 1920, 1080, "Andromeda Engine", And::EGraphicsApiType::OpenGL);
  //window->set_vsync(true);
  std::shared_ptr<And::GraphicsContext> g_context = window->get_context();
  std::shared_ptr<And::Renderer> g_renderer = And::Renderer::CreateShared(*window);

  //And::FlyCamera fly_cam{*window};
  //fly_cam.SetPosition(0.0f, 0.0f, 0.0f);
  //fly_cam.SetSize(1920.0f, 1080.0f);

  //fly_cam.SetFar(1000.0f);
  //fly_cam.SetNear(0.1f);

  //fly_cam.SetPosition(0.0f, 0.0f, 0.0f);
  //fly_cam.SetFov(90.0f);
  //fly_cam.SetDirection(0.0f, 0.0f, -1.0f);

  //g_renderer->set_camera(&fly_cam);


  //std::shared_ptr<And::Shader> s = And::MakeShader("default/geometry.shader");

  And::ResourceManager r_manager{*window, ts};
  //r_manager.AddGenerator<And::ObjGenerator>();
  r_manager.AddGenerator<And::ShaderGenerator>();
  
  And::Editor editor{*window, &r_manager};

  editor.AddWindow(ts.GetEditorWindow());

  // Show pc info
  g_context->create_info();

  std::shared_ptr<And::Texture> texture = And::MakeTexture("teapot_texture.jpg");

  float clear_color[4] = {0.0f, 0.0f, 0.0f, 1.0f};
  g_renderer->set_clear_color(clear_color);

  And::EntityComponentSystem entity_comp;
  And::AddBasicComponents(entity_comp);

  int num_obj = 10;
  float pos_x = 0.0f;
  float pos_y = -5.0f;

  And::MeshComponent MC, MC_teapot, MC_teapot2, MC_cube;
  MC.MeshOBJ = And::Geometry::load("sponza.obj");
  MC_cube.MeshOBJ = And::Geometry::load("cube.obj");
  MC_teapot.MeshOBJ = And::Geometry::load("teapot.obj");
  MC_teapot2.MeshOBJ = And::Geometry::load("teapot.obj");

  std::shared_ptr<And::Texture> texture_bricks = And::MakeTexture("bricks.jpg");
  std::shared_ptr<And::Texture> texture_jou = And::MakeTexture("jou_cumple.png");
  MC.MeshOBJ->SetTexture(texture_bricks);
  MC_cube.MeshOBJ->SetTexture(texture_jou);


  And::TransformComponent tran;
  And::TransformComponent tran_cube;
  And::TransformComponent tran_teapot;
  And::TransformComponent tran_teapot2;
  tran.position[0] = 0.0f;
  tran.position[1] = 0.0f;
  tran.position[2] = 0.0f;
  tran.rotation[0] = 1.0f;
  tran.rotation[1] = 1.0f;
  tran.rotation[2] = 1.0f;
  tran.scale[0] = 1.0f;
  tran.scale[1] = 1.0f;
  tran.scale[2] = 1.0f;
  
  tran_cube.position[0] = 50.0f;
  tran_cube.position[1] = 5.0f;
  tran_cube.position[2] = 0.0f;
  tran_cube.rotation[0] = 1.0f;
  tran_cube.rotation[1] = 1.0f;
  tran_cube.rotation[2] = 1.0f;
  tran_cube.scale[0] = 3.0f;
  tran_cube.scale[1] = 3.0f;
  tran_cube.scale[2] = 3.0f;
  
  tran_teapot.position[0] = 0.0f;
  tran_teapot.position[1] = 5.0f;
  tran_teapot.position[2] = -5.0f;
  tran_teapot.rotation[0] = 0.0f;
  tran_teapot.rotation[1] = 1.0f;
  tran_teapot.rotation[2] = 0.0f;
  tran_teapot.scale[0] = 2.0f;
  tran_teapot.scale[1] = 5.0f;
  tran_teapot.scale[2] = 2.0f;
  
  tran_teapot2.position[0] = 3.0f;
  tran_teapot2.position[1] = 5.0f;
  tran_teapot2.position[2] = -5.0f;
  tran_teapot2.rotation[0] = 0.0f;
  tran_teapot2.rotation[1] = 1.0f;
  tran_teapot2.rotation[2] = 0.0f;
  tran_teapot2.scale[0] = 2.0f;
  tran_teapot2.scale[1] = 2.0f;
  tran_teapot2.scale[2] = 2.0f;
  And::Entity* obj_id = entity_comp.new_entity(MC, tran);
  And::Entity* obj_cube_id = entity_comp.new_entity(MC_cube, tran_cube);
  //And::Entity* obj_teapot_id = entity_comp.new_entity(MC_teapot, tran_teapot);
  //And::Entity* obj_teapot_id2 = entity_comp.new_entity(MC_teapot2, tran_teapot2);
 
  float enabled = 1.0f;
  float diffuse_color[3] = {1.0f, 0.0f, 0.0f};
  float specular_color[3] = {1.0f, 1.0f, 1.0f};
  float position[3] = {0.0f, 14.0f, 11.0f};
  float direction[3] = {0.0f, 0.0f, -1.0f};

  float specular_strength = 0.003f;
  float specular_shininess = 8.0f;
  float constant_att = 1.0f;
  float linear_att = 0.014f;
  float quadratic_att = 0.0007f;
  float cutt_off = 2.5f;
  float outer_cut_off= 17.5f;

  And::SpotLight spot{};
  spot.SetPosition(position);
  spot.SetDirection(direction);
  spot.SetDiffuseColor(diffuse_color);
  spot.SetSpecularColor(specular_color);
  spot.SetSpecularStrength(specular_strength);
  spot.SetSpecularShininess(specular_shininess);
  spot.SetConstantAtt(constant_att);
  spot.SetLinearAtt(linear_att);
  spot.SetQuadraticAtt(quadratic_att);
  spot.SetCuttOff(cutt_off);
  spot.SetOuterCuttOff(outer_cut_off);
  spot.SetCastShadows(true);
  spot.SetEnabled(true);
  //entity_comp.new_entity(spot);

  position[1] += 10.0f;
  diffuse_color[0] = 0.0f;
  diffuse_color[1] = 1.0f;

  And::SpotLight spot2{};
  spot2.SetPosition(position);
  spot2.SetDirection(direction);
  spot2.SetDiffuseColor(diffuse_color);
  spot2.SetSpecularColor(specular_color);
  spot2.SetSpecularStrength(specular_strength);
  spot2.SetSpecularShininess(specular_shininess);
  spot2.SetConstantAtt(constant_att);
  spot2.SetLinearAtt(linear_att);
  spot2.SetQuadraticAtt(quadratic_att);
  spot2.SetCuttOff(cutt_off);
  spot2.SetOuterCuttOff(outer_cut_off);
  spot2.SetCastShadows(true);
  spot2.SetEnabled(true);
  //And::Entity* spot2_entity = entity_comp.new_entity(spot2);
    
  enabled = 1.0f;

  And::AmbientLight ambient{};
  ambient.SetDiffuseColor(0.1f, 0.1f, 0.1f);
  //entity_comp.new_entity(ambient);

  And::DirectionalLight directional{};
  directional.SetDirection(1.0f, 0.0f, 0.0f);
  directional.SetDiffuseColor(1.0f, 1.0f, 1.0f);
  directional.SetSpecularColor(1.0f, 1.0f, 1.0f);
  directional.SetSpecularShininess(32.0f);
  directional.SetCastShadows(true);
  directional.SetEnabled(true);
  entity_comp.new_entity(directional);

  float position2[3] = {0.0f, 34.0f, -5.0f};
  diffuse_color[2] = 1.0f;
  diffuse_color[1] = 0.0f;
  And::PointLight point{};
  point.SetPosition(position2);
  point.SetEnabled(1.0f);
  point.SetDiffuseColor(diffuse_color);
  point.SetSpecularStrength(specular_strength);
  point.SetSpecularColor(specular_color);
  point.SetSpecularShininess(specular_shininess);
  point.SetCastShadows(true);
  point.SetLinearAtt(linear_att);
  point.SetConstantAtt(constant_att);
  point.SetQuadraticAtt(quadratic_att);
  //And::Entity* point_entity = entity_comp.new_entity(point);
  
  position2[0] += 40.0f;
  diffuse_color[2] = 0.0f;
  diffuse_color[0] = 1.0f;

  And::PointLight point2{};
  point2.SetPosition(position2);
  point2.SetEnabled(1.0f);
  point2.SetDiffuseColor(diffuse_color);
  point2.SetSpecularStrength(specular_strength);
  point2.SetSpecularColor(specular_color);
  point2.SetSpecularShininess(specular_shininess);
  point2.SetCastShadows(true);
  point2.SetLinearAtt(linear_att);
  point2.SetConstantAtt(constant_att);
  point2.SetQuadraticAtt(quadratic_att);
  
  //And::Entity* point_entity2 = entity_comp.new_entity(point2);

  And::Transform* tr_tmp = obj_id->get_component<And::Transform>();

  

  float fps_count = 0.0f;
  while (window->is_open()){

    window->update();
    g_renderer->new_frame();
    editor.ShowWindows();

    //fly_cam.ProcessInput();

    //And::FlyCamera* cam;
        
        //g_renderer->get_camera();
    //cam->SetPosition(0.0f, 0.0f, 0.0f);
   
    //tr_tmp->position[0] = cosf(fps_count) * 5.0f;
    //position2[0] = cosf(fps_count) * 15.0f;
    //position[1] = cosf(fps_count) * 5.0f + 15.0f;
    
    fps_count +=0.01f;

    
    //point_entity->get_component<And::PointLight>()->SetPosition(position2);

    //spot2_entity->get_component<And::SpotLight>()->SetPosition(position);
    

    
    //g_renderer->draw_forward(entity_comp);
    g_renderer->draw_deferred(entity_comp);
    g_renderer->end_frame();
    window->swap_buffers();
  }

  return 0;
}