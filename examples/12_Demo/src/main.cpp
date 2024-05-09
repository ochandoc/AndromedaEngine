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
#include <iostream>
#include <random>
#include "Andromeda.h"


#define POOL_SIZE 50


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

static std::random_device rd;
static std::mt19937 gen(rd());

static void SpawnBall(std::shared_ptr<And::PhysicsEngine> engine, And::EntityComponentSystem& entity_comp) {
  float pos[3] = {0.0f, 2.0f, 0.0f};

  float base[3] = {1.0f, 1.0f, 1.0f};
  float scale[3] = {0.1f, 0.1f, 0.1f};


  And::MeshComponent MC;
  MC.MeshOBJ = And::Geometry::load("demo/obj/sphere.obj");
  And::MaterialComponent mat_comp;
  std::shared_ptr<And::Material> mat = std::make_shared<And::Material>();
  mat->SetColor(0.2f, 0.2f, 1.0f, 1.0f);
  mat_comp.SetMaterial(mat);
  And::TransformComponent tran;
  tran.SetPosition(pos);
  tran.SetRotation(0.0f, 0.0f, 0.0f);
  tran.SetScale(scale);

  And::RigidBody rb = engine->CreateRigidBody();
  rb.AddSphereCollider(pos, base);
  rb.AffectsGravity(true);
  rb.SetMass(5.0f);


  And::Entity* e = entity_comp.new_entity(MC, mat_comp, tran, rb);

  int minimo = 1;
  int maximo = 1000;
  std::uniform_int_distribution<> distribucion(minimo, maximo);
  int numero_aleatorio_x = distribucion(gen);
  int numero_aleatorio_z = distribucion(gen);

  float x = numero_aleatorio_x / 100;
  float z = numero_aleatorio_z / 100;

  e->get_component<And::RigidBody>()->AddForce(x, 100.0f, z, And::ForceMode::IMPULSE);

  printf("Lanuch at X:%f Y:%f Z:%f\n", x, 100.0f, z);
}

static void ThrowBall(And::Entity* pool[], int index) {

    And::RigidBody* rb = pool[index]->get_component<And::RigidBody>();
    rb->SetPosition(0.0f, 2.0, 0.0f);

    int minimo = 1;
    int maximo = 1000;
    std::uniform_int_distribution<> distribucion(minimo, maximo);
    int numero_aleatorio_x = distribucion(gen);
    int numero_aleatorio_z = distribucion(gen);

    float x = numero_aleatorio_x / 100;
    float z = numero_aleatorio_z / 100;

    rb->AddForce(x, 50.0f, z, And::ForceMode::IMPULSE);
    printf("Lanuch at X:%f Y:%f Z:%f\n", x, 50.0f, z);

}

void CreateBallsPool(And::Entity* e[], std::shared_ptr<And::PhysicsEngine> engine, And::EntityComponentSystem& entity_comp) {
    for (int i = 0; i < POOL_SIZE; i++) {
        float pos[3] = { i * 2.0f , 0.0f, 0.0f };

        float base[3] = { 0.5f, 0.5f, 0.5f };
        float scale[3] = { 0.1f, 0.1f, 0.1f };


        And::MeshComponent MC;
        MC.MeshOBJ = And::Geometry::load("demo/obj/sphere.obj");
        And::MaterialComponent mat_comp;
        std::shared_ptr<And::Material> mat = std::make_shared<And::Material>();
        mat->SetColor(0.2f, 0.2f, 1.0f, 1.0f);
        mat_comp.SetMaterial(mat);
        And::TransformComponent tran;
        tran.SetPosition(pos);
        tran.SetRotation(0.0f, 0.0f, 0.0f);
        tran.SetScale(scale);

        And::RigidBody rb = engine->CreateRigidBody();
        rb.AddSphereCollider(pos, base);
        rb.AffectsGravity(true);
        rb.SetMass(5.0f);


        e[i] = entity_comp.new_entity(MC, mat_comp, tran, rb);
    }

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

    std::shared_ptr<And::PhysicsEngine> physics_engine = And::PhysicsEngine::Init(8192);

    int num_obj = 10;
    float pos_x = 0.0f;
    float pos_y = -5.0f;

    And::MeshComponent MC_fountain;
    MC_fountain.MeshOBJ = And::Geometry::load("demo/obj/fountain.obj");
    And::MaterialComponent fountain_mat_comp;
    std::shared_ptr<And::Material> fountain_mat = std::make_shared<And::Material>();
    fountain_mat->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
    fountain_mat_comp.SetMaterial(fountain_mat);
    And::TransformComponent fountain_tran;
    fountain_tran.SetPosition(0.0f, 1.0f, 0.0f);
    fountain_tran.SetRotation(0.0f, 0.0f, 0.0f);
    fountain_tran.SetScale(1.0f, 1.0f, 1.0f);
    entity_comp.new_entity(MC_fountain, fountain_mat_comp, fountain_tran);
  
  
    And::MeshComponent MC_suelo;
    MC_suelo.MeshOBJ = And::Geometry::load("cube.obj");
  
    And::MaterialComponent suelo_mat_comp;
    std::shared_ptr<And::Material> suelo_mat = std::make_shared<And::Material>();
    std::shared_ptr<And::Texture> suelo_tex = And::MakeTexture("demo/textures/suelo/albedo.png");
    std::shared_ptr<And::Texture> suelo_normals = And::MakeTexture("demo/textures/suelo/normals.png");
    suelo_mat->SetColorTexture(suelo_tex);
    //suelo_mat->SetNormalTexture(suelo_normals);
    suelo_mat_comp.SetMaterial(suelo_mat);

    And::TransformComponent suelo_tran;
    suelo_tran.SetPosition(-20.0f, 0.0f, 0.0f);
    suelo_tran.SetRotation(0.0f, 0.0f, 3.1415f / 2.0f);
    suelo_tran.SetScale(10.0f, 1.0f, 10.0f);
    entity_comp.new_entity(MC_suelo, suelo_mat_comp, suelo_tran);


    suelo_tran.SetPosition(20.0f, 0.0f, 0.0f);
    entity_comp.new_entity(MC_suelo, suelo_mat_comp, suelo_tran);
  
    suelo_tran.SetRotation(0.0f, 0.0f, 0.0f);
   
    const float suelo_scale = 20.0f;
    const int suelo_num = 2;
    suelo_tran.SetScale(suelo_scale, 1.0f, suelo_scale);
   
   
    for (int j = -suelo_num; j < suelo_num; j++) {

        for (int i = -suelo_num; i < suelo_num; i++) {
            suelo_tran.SetPosition(i * suelo_scale, -1.5f, j * suelo_scale);
            entity_comp.new_entity(MC_suelo, suelo_mat_comp, suelo_tran);
        }
    }
  
  



  And::AmbientLight ambient;
  ambient.SetDiffuseColor(1.0f, 1.0f, 1.0f);
  ambient.SetAmbientStrenght(0.3f);
  entity_comp.new_entity(ambient);

  

  And::DirectionalLight directional;
  directional.SetDiffuseColor(1.0f, 1.0f, 1.0f);
  directional.SetDirection(1.0f, -0.5f, 0.0f);
  directional.SetSpecularColor(1.0f, 1.0f, 1.0f);
  directional.SetSpecularShininess(32.0f);
  directional.SetSpecularStrength(1.0f);
  directional.SetEnabled(true);
  directional.SetCastShadows(false);
  entity_comp.new_entity(directional);


  And::PointLight point;
  point.SetPosition(0.0f, 3.0f, 0.0f);
  point.SetSpecularColor(1.0f, 1.0f, 1.0f);
  point.SetSpecularShininess(2.0f);
  point.SetSpecularStrength(0.003f);
  point.SetLinearAtt(0.014f);
  point.SetQuadraticAtt(0.0007f);
  point.SetLinearAtt(1.0f);
  point.SetEnabled(true);
  point.SetCastShadows(false);
  point.SetDiffuseColor(1.0f, 0.0f, 0.0f);
  //And::Entity* entity_tmp = entity_comp.new_entity(point);
  //And::PointLight* point_tmp = entity_tmp->get_component<And::PointLight>();

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

  And::Entity* balls_pool[POOL_SIZE];
  int index_pool = 0;

  CreateBallsPool(balls_pool, physics_engine, entity_comp);


  float fps_count = 0.0f;
  const float force = 100.0f;
  int frames = 0;
  float secondsToSpawn = 3.0f;
  while (window->is_open()){
       
    
    window->update();
    g_renderer->new_frame();
    editor.ShowWindows();
    fps_count +=window->get_delta_time(); 

    //if(input.IsMouseButtonPressed(And::MouseCode::Left)){
    //if(input.check_action(shot)){
        //CreateJouCube(fly_cam.GetPosition(), fly_cam.GetDirection(), force, entity_comp, *physics_engine, texture_cara_de_jou);
    //}

    //float* pos = point_tmp->GetPosition();
    //point_tmp->SetPosition(pos[0], std::abs(cosf(fps_count) * 3.0f), pos[2]);
    //printf("Position-> Y: %f\n", std::abs(cosf(fps_count) * 3.0f));

    if (fps_count >= secondsToSpawn) {
        //SpawnBall(physics_engine, entity_comp);
        ThrowBall(balls_pool, index_pool);
        fps_count -= secondsToSpawn;
        index_pool++;
        if (index_pool >= POOL_SIZE)index_pool = 0;
    }


    if (fps_count > 0.1f) [[likely]] {
        physics_engine->Simulate(window->get_delta_time());
    }

    physics_engine->Apply(entity_comp);
    
    //g_renderer->draw_forward(entity_comp);
    g_renderer->draw_deferred(entity_comp);

    
    frames++;
    g_renderer->end_frame();
    window->swap_buffers();
  }

  physics_engine->Release(entity_comp);

  return 0;
}