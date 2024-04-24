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

  And::FlyCamera fly_cam{*window};
  fly_cam.SetPosition(0.0f, 0.0f, 0.0f);
  fly_cam.SetSize(1920.0f, 1080.0f);

  fly_cam.SetFar(1000.0f);
  fly_cam.SetNear(0.1f);


  //fly_cam.SetPosition(0.0f, 0.0f, 0.0f);
  fly_cam.SetFov(90.0f);
  fly_cam.SetDirection(0.0f, 0.0f, -1.0f);

  g_renderer->set_camera(&fly_cam);


  //std::shared_ptr<And::Shader> s = And::MakeShader("default/geometry.shader");

  And::ResourceManager r_manager{*window, ts};
  //r_manager.AddGenerator<And::ObjGenerator>();
  r_manager.AddGenerator<And::ShaderGenerator>();
  
  And::Editor editor{*window, &r_manager};

  editor.AddWindow(ts.GetEditorWindow());

  // Show pc info
  g_context->create_info();

  std::shared_ptr<And::Texture> texture = And::MakeTexture("teapot_texture.jpg");

  float clear_color[4] = {1.0f, 0.0f, 0.0f, 1.0f};
  g_renderer->set_clear_color(clear_color);

  And::EntityComponentSystem entity_comp;
  And::AddBasicComponents(entity_comp);


  std::shared_ptr<And::PhysicsEngine> physics_engine = And::PhysicsEngine::Init();

  int num_obj = 10;
  float pos_x = 0.0f;
  float pos_y = -5.0f;

  And::MeshComponent MC, MC_teapot, MC_teapot2, MC_cube, MC_cube2, MC_cube_up;
  MC.MeshOBJ = And::Geometry::load("sponza.obj");
  MC_cube.MeshOBJ = And::Geometry::load("cube.obj");
  MC_cube_up.MeshOBJ = And::Geometry::load("cube.obj");
  MC_cube2.MeshOBJ = And::Geometry::load("cube.obj");
  MC_teapot.MeshOBJ = And::Geometry::load("teapot.obj");
  MC_teapot2.MeshOBJ = And::Geometry::load("teapot.obj");
  
  //std::shared_ptr<And::Physics> e_tmp(And::Physics(physics_engine));
  //std::shared_ptr<And::Physics> e_tmp = std::make_shared<And::Physics>(physics_tmp);
  

  std::shared_ptr<And::Texture> texture_bricks = And::MakeTexture("bricks.jpg");
  std::shared_ptr<And::Texture> texture_jou = And::MakeTexture("sphere_basecolor.png");
  std::shared_ptr<And::Texture> texture_cara_de_jou = And::MakeTexture("jou_cumple.png");
  std::shared_ptr<And::Texture> wall_tex = And::MakeTexture("brick-wall-bl/brick-wall_albedo.png");
  std::shared_ptr<And::Texture> wall_tex_normal = And::MakeTexture("brick-wall-bl/brick-wall_normal-ogl.png");
  std::shared_ptr<And::Texture> wall_tex_specular = And::MakeTexture("brick-wall-bl/brick-wall_ao.png");
  
  And::MaterialComponent material_comp_jou;
  std::shared_ptr<And::Material> material_jou = std::make_shared<And::Material>();
  material_jou->SetColorTexture(wall_tex);
  material_jou->SetNormalTexture(wall_tex_normal);
  material_jou->SetSpecularTexture(wall_tex_specular);
  material_jou->SetColor(0.1f, 1.0f, 0.1f, 0.5f);
  material_comp_jou.SetMaterial(material_jou);
  
  And::MaterialComponent material_comp_sponza;
  std::shared_ptr<And::Material> material_sponza = std::make_shared<And::Material>();
  material_sponza->SetColorTexture(texture_bricks);
  material_comp_sponza.SetMaterial(material_sponza);

  //MC.MeshOBJ->SetTexture(texture_bricks);
  //MC_cube.MeshOBJ->SetTexture(texture_jou);




  //MC_cube2.MeshOBJ->SetTexture(texture_cara_de_jou);
  //MC_cube_up.MeshOBJ->SetTexture(texture_cara_de_jou);


  And::TransformComponent tran;
  And::TransformComponent tran_cube_up;
  And::TransformComponent tran_cube;
  And::TransformComponent tran_cube2;
  And::TransformComponent tran_teapot;
  And::TransformComponent tran_teapot2;
  tran.position[0] = 0.0f;
  tran.position[1] = -1.0f;
  tran.position[2] = 0.0f;
  tran.rotation[0] = 0.0f;
  tran.rotation[1] = 0.0f;
  tran.rotation[2] = 0.0f;
  tran.scale[0] = 1.0f;
  tran.scale[1] = 1.0f;
  tran.scale[2] = 1.0f;
  
  tran_cube.position[0] = 0.0f;
  tran_cube.position[1] = 15.0f;
  tran_cube.position[2] = -15.0f;
  tran_cube.rotation[0] = 0.0f;
  tran_cube.rotation[1] = 0.0f;
  tran_cube.rotation[2] = 0.0f;
  tran_cube.scale[0] = 5.0f;
  tran_cube.scale[1] = 5.0f;
  tran_cube.scale[2] = 5.0f;
  
  tran_cube_up.position[0] = 1.0f;
  tran_cube_up.position[1] = 20.0f;
  tran_cube_up.position[2] = -15.0f;
  tran_cube_up.rotation[0] = 0.0f;
  tran_cube_up.rotation[1] = 0.0f;
  tran_cube_up.rotation[2] = 0.0f;
  tran_cube_up.scale[0] = 2.0f;
  tran_cube_up.scale[1] = 2.0f;
  tran_cube_up.scale[2] = 2.0f;
  
  tran_teapot.position[0] = 0.0f;
  tran_teapot.position[1] = 5.0f;
  tran_teapot.position[2] = -5.0f;
  tran_teapot.rotation[0] = 0.0f;
  tran_teapot.rotation[1] = 0.0f;
  tran_teapot.rotation[2] = 0.0f;
  tran_teapot.scale[0] = 2.0f;
  tran_teapot.scale[1] = 5.0f;
  tran_teapot.scale[2] = 2.0f;
  
  tran_teapot2.position[0] = 3.0f;
  tran_teapot2.position[1] = 5.0f;
  tran_teapot2.position[2] = -5.0f;
  tran_teapot2.rotation[0] = 0.0f;
  tran_teapot2.rotation[1] = 0.0f;
  tran_teapot2.rotation[2] = 0.0f;
  tran_teapot2.scale[0] = 2.0f;
  tran_teapot2.scale[1] = 2.0f;
  tran_teapot2.scale[2] = 2.0f;

  // Sponza
  And::Entity* obj_id = entity_comp.new_entity(MC, tran, material_comp_sponza);

  And::RigidBody rb = physics_engine->CreateRigidBody();
  And::RigidBody rb_up = physics_engine->CreateRigidBody();
  rb.AddBoxCollider(tran_cube.position, tran_cube.scale);
  rb.AffectsGravity(true);
  rb.SetMass(5.0f);
  rb_up.AddBoxCollider(tran_cube_up.position, tran_cube_up.scale);
  rb_up.AffectsGravity(true);
  rb.SetMass(5.0f);

  // Cube
  And::Entity* obj_cube_id = entity_comp.new_entity(MC_cube, tran_cube, rb, material_comp_jou);
  //And::Entity* obj_cube_up_id = entity_comp.new_entity(MC_cube_up, tran_cube_up, rb_up, material_comp_jou);

  tran_cube2.position[0] = 3.0f;
  tran_cube2.position[1] = 0.0f;
  tran_cube2.position[2] = 0.0f;
  tran_cube2.rotation[0] = 0.0f;
  tran_cube2.rotation[1] = 0.0f;
  tran_cube2.rotation[2] = 0.0f;
  tran_cube2.scale[0] = 1.0f;
  tran_cube2.scale[1] = 1.0f;
  tran_cube2.scale[2] = 1.0f;
  //And::Entity* obj_cube_id2 = entity_comp.new_entity(MC_cube2, tran_cube2, material_comp_jou);

  //And::TransformComponent* tr_cube = obj_cube_id->get_component<And::TransformComponent>();
  //And::TransformComponent* tr_cube2 = obj_cube_id2->get_component<And::TransformComponent>();
  //tr_cube2->SetParent(tr_cube);

 
  //And::Entity* obj_teapot_id = entity_comp.new_entity(MC_teapot, tran_teapot);
  //And::Entity* obj_teapot_id2 = entity_comp.new_entity(MC_teapot2, tran_teapot2);
 
  float enabled = 1.0f;
  float diffuse_color[3] = {1.0f, 0.0f, 0.0f};
  float specular_color[3] = {1.0f, 1.0f, 1.0f};
  float position[3] = {0.0f, 14.0f, 0.0f};
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
  entity_comp.new_entity(spot);

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
  entity_comp.new_entity(ambient);

  And::DirectionalLight directional{};
  directional.SetDirection(1.0f, 0.0f, 0.0f);
  directional.SetDiffuseColor(1.0f, 1.0f, 1.0f);
  directional.SetSpecularColor(1.0f, 1.0f, 1.0f);
  directional.SetSpecularShininess(32.0f);
  directional.SetCastShadows(true);
  directional.SetEnabled(true);
  entity_comp.new_entity(directional);

  float position2[3] = {0.0f, 8.0f, 0.0f};
  diffuse_color[2] = 1.0f;
  diffuse_color[1] = 0.0f;
  And::PointLight point{};
  point.SetPosition(position2);
  point.SetEnabled(1.0f);
  point.SetDiffuseColor(1.0f, 1.0f, 1.0f);
  point.SetSpecularStrength(specular_strength);
  point.SetSpecularColor(specular_color);
  point.SetSpecularShininess(specular_shininess);
  point.SetCastShadows(true);
  point.SetEnabled(true);
  point.SetLinearAtt(linear_att);
  point.SetConstantAtt(constant_att);
  point.SetQuadraticAtt(quadratic_att);
  And::Entity* point_entity = entity_comp.new_entity(point);
  
  //position2[0] += 40.0f;
  //diffuse_color[2] = 0.0f;
  //diffuse_color[0] = 1.0f;

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
  //CreateJouCube(position_tmp, entity_comp, *physics_engine, texture_cara_de_jou);
  And::Input input{ *window };
  And::ActionInput jump{ "Jump", And::KeyState::Press, { And::KeyCode::Space} };
  And::ActionInput shot{ "Shot", And::KeyState::Press, { And::KeyCode::C} };


  And::MaterialComponent* m_comp = obj_cube_id->get_component<And::MaterialComponent>();

  float fps_count = 0.0f;
  const float force = 100.0f;
  while (window->is_open()){

    window->update();
    g_renderer->new_frame();
    editor.ShowWindows();

    //if(input.IsMouseButtonPressed(And::MouseCode::Left)){
    if(input.check_action(shot)){
        CreateJouCube(fly_cam.GetPosition(), fly_cam.GetDirection(), force, entity_comp, *physics_engine, texture_cara_de_jou);
    }

    if (input.check_action(jump)) {
        physics_engine->SetGravity(0.0f, 10.0f, 0.0f);
    }

    //m_comp->GetMaterial()->SetColor(sinf(fps_count * 0.7f), cosf(fps_count), cosf(fps_count * 0.5f), 1.0f);

    //tr_cube->SetRotation(0.0f, fps_count, 0.0f);

    fly_cam.ProcessInput();
    fly_cam.ShowValues();

    //And::FlyCamera* cam;
        
    //fly_cam.ShowValues();
        //g_renderer->get_camera();
    //cam->SetPosition(0.0f, 0.0f, 0.0f);
   
    //tr_tmp->position[0] = cosf(fps_count) * 5.0f;
    //position2[0] = cosf(fps_count) * 15.0f;
    //position[1] = cosf(fps_count) * 5.0f + 15.0f;
    
    fps_count +=0.01f; 

    
    //point_entity->get_component<And::PointLight>()->SetPosition(position2);

    //spot2_entity->get_component<And::SpotLight>()->SetPosition(position);
    

    if (fps_count > 0.1f) [[likely]] {
        physics_engine->Simulate(window->get_delta_time());
    }
    //physics_engine->Simulate(1.0f/60.0f);
    physics_engine->Apply(entity_comp);

    
    

    //g_renderer->draw_forward(entity_comp);
    g_renderer->draw_deferred(entity_comp);
    g_renderer->end_frame();
    window->swap_buffers();
  }

  physics_engine->Release(entity_comp);

  return 0;
}