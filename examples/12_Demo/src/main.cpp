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

static void CreateJouCube(const float* pos, const float* dir, float force, And::EntityComponentSystem& ecs, And::PhysicsEngine& engine, std::shared_ptr<And::Texture> texture) {
    And::MeshComponent MC;
    MC.MeshOBJ = And::Geometry::load("cube.obj");
    //MC.MeshOBJ->SetTexture(texture);

    And::TransformComponent tran;
    tran.position[0] = pos[0] + (dir[0] * 5.0f);
    tran.position[1] = pos[1] + (dir[1] * 5.0f);
    tran.position[2] = pos[2] + (dir[2] * 5.0f);
    tran.rotation[0] = 0.0f;
    tran.rotation[1] = 0.0f;
    tran.rotation[2] = 0.0f;
    tran.scale[0] = 2.0f;
    tran.scale[1] = 2.0f;
    tran.scale[2] = 2.0f;

    And::RigidBody rb = engine.CreateRigidBody();
    rb.AddBoxCollider(tran.position, tran.scale);
    rb.AffectsGravity(true);
    rb.SetMass(5.0f);

    And::Entity* e = ecs.new_entity(MC, tran, rb);

    float new_dir[3] = {dir[0] * force, dir[1] * force, dir[2] * force };
    
    e->get_component<And::RigidBody>()->AddForce(new_dir, And::ForceMode::IMPULSE);
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


  And::ResourceManager r_manager{*window, ts};
  r_manager.AddGenerator<And::ShaderGenerator>();
  
  And::Editor editor{*window, &r_manager};

  editor.AddWindow(ts.GetEditorWindow());

  // Show pc info
  g_context->create_info();


  float clear_color[4] = {1.0f, 0.0f, 0.0f, 1.0f};
  g_renderer->set_clear_color(clear_color);

  And::EntityComponentSystem entity_comp;
  And::AddBasicComponents(entity_comp);


  std::shared_ptr<And::PhysicsEngine> physics_engine = And::PhysicsEngine::Init();

  int num_obj = 10;
  float pos_x = 0.0f;
  float pos_y = -5.0f;

  And::MeshComponent MC;


  std::shared_ptr<And::Geometry> geo = And::Geometry::load("cube.obj");
  float position_tmp[3] = {-1.0f, 20.0f, -15.0f};
  /*for (int i = 0; i < 30; i++) {

      position_tmp[0] += (i * 0.2f);
      position_tmp[1] += i;
      And::MeshComponent MC_tmp;
      MC_tmp.MeshOBJ = geo;
      //MC_tmp.MeshOBJ->SetTexture(texture_cara_de_jou);

      And::TransformComponent tran_tmp;
      tran_tmp.position[0] = position_tmp[0];
      tran_tmp.position[1] = position_tmp[1];
      tran_tmp.position[2] = position_tmp[2];
      tran_tmp.rotation[0] = 0.0f;
      tran_tmp.rotation[1] = 0.0f;
      tran_tmp.rotation[2] = 0.0f;
      tran_tmp.scale[0] = 2.0f;
      tran_tmp.scale[1] = 2.0f;
      tran_tmp.scale[2] = 2.0f;

      And::RigidBody rb_tmp = physics_engine->CreateRigidBody();
      rb_tmp.AddBoxCollider(tran_tmp.position, tran_tmp.scale);
      //rb_tmp.AddSphereCollider(tran_tmp.position, tran_tmp.scale);
      rb_tmp.AffectsGravity(true);
      rb_tmp.SetMass(5.0f);

      entity_comp.new_entity(MC_tmp, tran_tmp, rb_tmp);

  }*/

  And::Input input{ *window };
  And::ActionInput jump{ "Jump", And::KeyState::Press, { And::KeyCode::Space} };
  And::ActionInput shot{ "Shot", And::KeyState::Press, { And::KeyCode::C} };


  float fps_count = 0.0f;
  const float force = 100.0f;
  while (window->is_open()){

    
    window->update();
    g_renderer->new_frame();
    editor.ShowWindows();

    //if(input.IsMouseButtonPressed(And::MouseCode::Left)){
    //if(input.check_action(shot)){
        //CreateJouCube(fly_cam.GetPosition(), fly_cam.GetDirection(), force, entity_comp, *physics_engine, texture_cara_de_jou);
    //}

    fps_count +=0.01f; 

    if (fps_count > 0.1f) [[likely]] {
        //physics_engine->Simulate(window->get_delta_time());
    }

    physics_engine->Apply(entity_comp);
    
    //g_renderer->draw_forward(entity_comp);
    g_renderer->draw_deferred(entity_comp);

    
    g_renderer->end_frame();
    window->swap_buffers();
  }

  physics_engine->Release(entity_comp);

  return 0;
}