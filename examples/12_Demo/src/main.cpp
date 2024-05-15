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


#define POOL_SIZE 5


static And::Entity* big_balls[POOL_SIZE];
static std::random_device rd;
static std::mt19937 gen(rd());

static void LaunchBall(const float* pos, const float* dir, float force, And::EntityComponentSystem& ecs, And::PhysicsEngine& engine, And::MeshComponent& mesh, And::MaterialComponent& material_comp, bool big = false) {

    And::TransformComponent tran;
    tran.position[0] = pos[0] + (dir[0] * 5.0f);
    tran.position[1] = pos[1] + (dir[1] * 5.0f);
    tran.position[2] = pos[2] + (dir[2] * 5.0f);
    
    //tran.position[0] = 1.0f;
    //tran.position[1] = 1.0f;
    //tran.position[2] = 1.0f;
    
    tran.rotation[0] = 0.0f;
    tran.rotation[1] = 0.0f;
    tran.rotation[2] = 0.0f;
    tran.rotation[3] = 0.0f;

    tran.scale[0] = 0.5f;
    tran.scale[1] = 0.5f;
    tran.scale[2] = 0.5f;


    And::RigidBody rb = engine.CreateRigidBody();
    
    if (big) {
        tran.scale[0] = 2.0f;
        tran.scale[1] = 2.0f;
        tran.scale[2] = 2.0f;

        rb.AddSphereCollider(tran.position, 2.0f, And::ColliderType::RigidDynamic, 0.4f, 0.4f, 0.8f);
        rb.SetMass(60.0f);
        force *= 4.0f;
    }else {

        rb.AddSphereCollider(tran.position, 0.5f, And::ColliderType::RigidDynamic, 0.4f, 0.4f, 0.8f);
        rb.SetMass(20.0f);
    }
   
    rb.AffectsGravity(true);

    tran.HasRigidBody();

    //And::Entity* e = ecs.new_entity(mesh, material_comp, tran);
    And::Entity* e = ecs.new_entity(mesh, material_comp, tran, rb);

    float* transform_tmp = e->get_component<And::TransformComponent>()->GetModelMatrix();

    float new_dir[3] = {dir[0] * force, dir[1] * force, dir[2] * force };
    
    And::RigidBody* tmp = e->get_component<And::RigidBody>();
    tmp->AddForce(new_dir, And::ForceMode::IMPULSE);
}

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
  rb.AddSphereCollider(pos, 1.0f, And::ColliderType::RigidDynamic);
  rb.AffectsGravity(true);
  rb.SetMass(5.0f);

  tran.HasRigidBody();


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
    rb->SetPosition(0.0f, 5.0, 0.0f);

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

static void CreateSuelo(And::EntityComponentSystem& ecs, And::PhysicsEngine& engine, And::MeshComponent& MC_suelo,  And::MaterialComponent& suelo_mat_comp) {
   

    float pos_tmp[3] = { -5.0f, 5.0f, 0.0f };
    float scale_tmp[3] = { 1.0f, 10.0f, 10.0f };
    And::TransformComponent suelo_tran;
    suelo_tran.SetPosition(pos_tmp);
    //suelo_tran.SetRotation(0.0f, 0.0f, 3.1415f / 2.0f);
    suelo_tran.SetRotation(0.0f, 0.0f, 0.0f);
    suelo_tran.SetScale(scale_tmp);

    And::RigidBody rb = engine.CreateRigidBody();
    rb.AddBoxCollider(pos_tmp, scale_tmp, And::ColliderType::RigidStatic, 0.5f, 0.5f, 0.8f);
    rb.SetMass(50.0f);
    rb.AffectsGravity(true);
    suelo_tran.HasRigidBody();
    ecs.new_entity(MC_suelo, suelo_mat_comp, suelo_tran, rb);
    
    /*And::RigidBody rb_2 = engine.CreateRigidBody();
    rb_2.AddBoxCollider(pos_tmp, scale_tmp, And::ColliderType::RigidDynamic, 0.5f, 0.5f, 0.8f);
    rb_2.SetMass(10.0f);
    rb_2.AffectsGravity(true);


    suelo_tran.SetPosition(20.0f, 4.0f, 0.0f);
    ecs.new_entity(MC_suelo, suelo_mat_comp, suelo_tran);*/

    suelo_tran.SetRotation(0.0f, 0.0f, 0.0f);

    const float suelo_scale = 20.0f;
    const int suelo_num = 5;
    suelo_tran.SetScale(suelo_scale, 1.0f, suelo_scale);

    float scale[3] = { suelo_scale , 1.0f, suelo_scale };

    for (int j = -suelo_num; j < suelo_num; j++) {

        for (int i = -suelo_num; i < suelo_num; i++) {

            float pos[3] = { i * suelo_scale , 0.0f, j * suelo_scale };

            And::RigidBody rb = engine.CreateRigidBody();
            rb.AddBoxCollider(pos, scale, And::ColliderType::RigidStatic, 0.5f, 0.5f, 0.8f);
            //rb.SetMass(10.0f);
            //rb.AffectsGravity(true);
            suelo_tran.SetPosition(pos[0], pos[1], pos[2]);
            suelo_tran.HasRigidBody();
            ecs.new_entity(MC_suelo, suelo_mat_comp, suelo_tran, rb);
            //ecs.new_entity(MC_suelo, suelo_mat_comp, suelo_tran, rb);
        }
    }
}

void CreateBallsPool(And::Entity* e[], std::shared_ptr<And::PhysicsEngine> engine, And::EntityComponentSystem& entity_comp, And::MaterialComponent& mat_comp) {
    for (int i = 0; i < POOL_SIZE; i++) {
        float pos[3] = { i * 2.0f , 100.0f, 0.0f };

        float base[3] = { 0.5f, 0.5f, 0.5f };
        float scale[3] = { 0.5f, 0.5f, 0.5f };


        And::MeshComponent MC;
        MC.MeshOBJ = And::Geometry::load("sphere.obj");


        And::TransformComponent tran;
        tran.SetPosition(pos);
        tran.SetRotation(0.0f, 0.0f, 0.0f);
        tran.SetScale(scale);
        tran.HasRigidBody();

        And::RigidBody rb = engine->CreateRigidBody();
        rb.AddSphereCollider(pos, 0.5f, And::ColliderType::RigidDynamic, 0.4f, 0.4f, 0.8f);
        rb.AffectsGravity(true);
        rb.SetMass(4.0f);


        e[i] = entity_comp.new_entity(MC, mat_comp, tran, rb);
    }

}

void CreateBolos(And::EntityComponentSystem& ecs, And::PhysicsEngine& engine, And::MeshComponent& mesh, And::MaterialComponent& material_comp) {
    
    And::TransformComponent tr;
    
    tr.SetRotation(0.0f, 0.0f, 0.0f);
    tr.SetScale(15.0f, 15.0f, 15.0f);
    tr.HasRigidBody();

    float initial_position_x = 0.0f;
    float initial_position_z = 80.0f;
    float initial_position_y = 6.0f;
    float separation = 4.0f;
    float separation_x = 3.0f;


    float scale[3] = {1.2f, 5.5f, 1.2f};
    float position[3] = { 1.0f, 1.0f, 1.0f };

    tr.SetPositionOffet(0.0f, -3.0f, 0.0f);


    for (int i = -3; i <= 3; i++) {
        And::RigidBody rb = engine.CreateRigidBody();
        tr.SetPosition(initial_position_x + (i * separation_x), initial_position_y, initial_position_z);
        position[0] = initial_position_x + (i * separation_x);
        position[1] = initial_position_y;
        position[2] = initial_position_z;
        rb.AddBoxCollider(position, scale, And::ColliderType::RigidDynamic, 0.4f, 0.4f, 0.5f);
        rb.SetMass(5.0f);
        rb.AffectsGravity(true);
        tr.HasRigidBody();
        ecs.new_entity(mesh, material_comp, tr, rb);
    }
    
    for (int i = -2; i <= 2; i++) {
        And::RigidBody rb = engine.CreateRigidBody();
        tr.SetPosition(initial_position_x + (i * separation_x), initial_position_y, initial_position_z + (-separation));
        position[0] = initial_position_x + (i * separation_x);
        position[1] = initial_position_y;
        position[2] = initial_position_z + (-separation);
        rb.AddBoxCollider(position, scale, And::ColliderType::RigidDynamic, 0.4f, 0.4f, 0.5f);
        rb.SetMass(5.0f);
        rb.AffectsGravity(true);
        tr.HasRigidBody();
        ecs.new_entity(mesh, material_comp, tr, rb);
    }
    
    for (int i = -1; i <= 1; i++) {
        And::RigidBody rb = engine.CreateRigidBody();
        tr.SetPosition(initial_position_x + (i * separation_x), initial_position_y, initial_position_z + ( -separation * 2.0f));
        position[0] = initial_position_x + (i * separation_x);
        position[1] = initial_position_y;
        position[2] = initial_position_z + (-separation * 2.0f);
        rb.AddBoxCollider(position, scale, And::ColliderType::RigidDynamic, 0.4f, 0.4f, 0.5f);
        rb.SetMass(5.0f);
        rb.AffectsGravity(true);
        tr.HasRigidBody();
        ecs.new_entity(mesh, material_comp, tr, rb);
    }

    tr.SetPosition(initial_position_x, initial_position_y, initial_position_z + (-separation * 3.0f));
    position[0] = initial_position_x;
    position[1] = initial_position_y;
    position[2] = initial_position_z + (-separation * 3.0f);
    And::RigidBody rb = engine.CreateRigidBody();
    rb.AddBoxCollider(position, scale, And::ColliderType::RigidDynamic, 0.4f, 0.4f, 0.5f);
    rb.SetMass(5.0f);
    rb.AffectsGravity(true);
    tr.HasRigidBody();
    ecs.new_entity(mesh, material_comp, tr, rb);

    
    

}

And::Entity* CreateLinterna(And::EntityComponentSystem& ecs) {

    float intensity = 100.0f;
    And::PointLight point;
    point.SetPosition(0.0f, 0.0f, 0.0f);
    point.SetSpecularColor(1.0f, 1.0f, 1.0f);
    point.SetSpecularShininess(32.0f);
    point.SetSpecularStrength(0.003f);
    point.SetConstantAtt(1.0f);
    point.SetLinearAtt(0.045f);
    point.SetQuadraticAtt(0.0075f);
    point.SetEnabled(true);
    point.SetCastShadows(true);
    point.SetIntensity(intensity);
    point.SetDiffuseColor(1.0f, 1.0f, 1.0f);
    return ecs.new_entity(point);

}

void UpdateLight(And::Entity* point, And::Entity* vela, And::FlyCamera& cam) {
    const float* pos = cam.GetPosition();
    const float* dir= cam.GetDirection();
    const float* up = cam.GetUp();
    const float* right = cam.GetRight();

    float distance = 2.0f;
    float pos_x = ((pos[0] + (dir[0] * distance)) + (right[0] * -2.0f)) + (up[0] * -3.0f);
    float pos_y = ((pos[1] + (dir[1] * distance)) + (right[1] * -2.0f)) + (up[1] * -3.0f);
    float pos_z = ((pos[2] + (dir[2] * distance)) + (right[2] * -2.0f)) + (up[2] * -3.0f);


    float pos_x_light = ((pos[0] + (dir[0] * (distance + 3.0f))) + right[0]) + up[0];
    float pos_y_light = ((pos[1] + (dir[1] * (distance + 3.0f))) + right[1]) + up[1];
    float pos_z_light = ((pos[2] + (dir[2] * (distance + 3.0f))) + right[2]) + up[2];


    float yaw = atan2f(dir[0],dir[2]);
    

    point->get_component<And::PointLight>()->SetPosition(pos_x_light, pos_y_light, pos_z_light);
    And::TransformComponent* tmp = vela->get_component<And::TransformComponent>();
    tmp->SetPosition(pos_x, pos_y, pos_z);
    tmp->SetRotation(0.0f, yaw, 0.0f);
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

    std::vector<std::string> paths = { {"skybox/right.jpg"}, {"skybox/left.jpg"}, {"skybox/top.jpg"}, {"skybox/bottom.jpg"}, {"skybox/back.jpg"}, {"skybox/front.jpg"} };
    std::shared_ptr<And::SkyboxTexture> sky_box = And::MakeSkyboxTexture(paths);
    g_renderer->set_skybox_texture(sky_box);
    g_renderer->enable_skybox(true);


    And::FlyCamera fly_cam{*window};
    fly_cam.SetPosition(-5.0f, 10.0f, 10.0f);
    fly_cam.SetSize(1920.0f, 1080.0f);

    fly_cam.SetFar(1000.0f);
    fly_cam.SetNear(0.1f);


    fly_cam.SetPosition(0.0f, 5.0f, 10.0f);
    fly_cam.SetFov(90.0f);
    fly_cam.SetDirection(0.0f, 0.0f, -1.0f);

    fly_cam.SetSpeed(10.0f);

    g_renderer->set_camera(&fly_cam);


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

    std::shared_ptr<And::PhysicsEngine> physics_engine = And::PhysicsEngine::Init(false, 8192);

    int num_obj = 10;
    float pos_x = 0.0f;
    float pos_y = -5.0f;

    {

        And::MeshComponent MC_fountain;
        MC_fountain.MeshOBJ = And::Geometry::load("demo/obj/fountain.obj");
        And::MaterialComponent fountain_mat_comp;
        std::shared_ptr<And::Material> fountain_mat = std::make_shared<And::Material>();

        std::shared_ptr<And::Texture> fuente_tex = And::MakeTexture("demo/textures/fuente/albedo.png");
        std::shared_ptr<And::Texture> fuente_normals = And::MakeTexture("demo/textures/fuente/normals.png");
        std::shared_ptr<And::Texture> fuente_ao = And::MakeTexture("demo/textures/fuente/ao.png");
        std::shared_ptr<And::Texture> fuente_metallic = And::MakeTexture("demo/textures/fuente/metallic.png");
        std::shared_ptr<And::Texture> fuente_rou = And::MakeTexture("demo/textures/fuente/roughness.png");
        fountain_mat->SetColor(0.353f, 0.698f, 1.0f, 1.0f);
        fountain_mat->SetColorTexture(fuente_tex);
        fountain_mat->SetNormalTexture(fuente_normals);
        fountain_mat->SetAmbientOclusionTexture(fuente_ao);
        fountain_mat->SetMetallicTexture(fuente_metallic);
        fountain_mat->SetRoughnessTexture(fuente_rou);
        fountain_mat_comp.SetMaterial(fountain_mat);

        float pos_tmp_f[3] = {3.0f, 1.0f, 0.0f};
        And::TransformComponent fountain_tran;
        fountain_tran.SetPosition(pos_tmp_f);
        fountain_tran.SetRotation(0.0f, 0.0f, 0.0f);
        fountain_tran.SetScale(3.0f, 3.0f, 3.0f);
        fountain_tran.HasRigidBody();

        float scale_tmp_f[3] = {3.0f, 3.0f, 3.0f};
    
        And::RigidBody rb_tmp = physics_engine->CreateRigidBody();
        rb_tmp.AddBoxCollider(pos_tmp_f, scale_tmp_f, And::ColliderType::RigidStatic, 0.4f, 0.4f, 0.8f);
        rb_tmp.SetMass(50.0f);
        rb_tmp.AffectsGravity(true);

        entity_comp.new_entity(MC_fountain, fountain_mat_comp, fountain_tran, rb_tmp);
        //entity_comp.new_entity(MC_fountain, fountain_mat_comp, fountain_tran);
    }
  
  
    


    And::MeshComponent MC_bolinga;
    MC_bolinga.MeshOBJ = And::Geometry::load("sphere.obj");

    And::MaterialComponent bolinga_mat_comp;
    std::shared_ptr<And::Material> bolinga_mat = std::make_shared<And::Material>();
    std::shared_ptr<And::Texture> bolinga_tex = And::MakeTexture("demo/textures/bola/albedo.png");
    std::shared_ptr<And::Texture> bolinga_normals = And::MakeTexture("demo/textures/bola/normals.png");
    std::shared_ptr<And::Texture> bolinga_ao = And::MakeTexture("demo/textures/bola/ao.png");
    std::shared_ptr<And::Texture> bolinga_metallic = And::MakeTexture("demo/textures/bola/metallic.png");
    std::shared_ptr<And::Texture> bolinga_rou = And::MakeTexture("demo/textures/bola/roughness.png");
    bolinga_mat->SetColorTexture(bolinga_tex);
    bolinga_mat->SetNormalTexture(bolinga_normals);
    bolinga_mat->SetAmbientOclusionTexture(bolinga_ao);
    bolinga_mat->SetMetallicTexture(bolinga_metallic);
    bolinga_mat->SetRoughnessTexture(bolinga_rou);
    bolinga_mat_comp.SetMaterial(bolinga_mat);

    And::TransformComponent bolinga_tran; 
    bolinga_tran.SetPosition(5.0f, 3.0f, 0.0f);
    bolinga_tran.SetRotation(0.0f, 0.0f, 0.0f);
    bolinga_tran.SetScale(1.0f, 1.0f, 1.0f);
    bolinga_tran.HasRigidBody(false);
    And::Entity* bolinga_entity = entity_comp.new_entity(MC_bolinga, bolinga_mat_comp, bolinga_tran);
        

    And::MeshComponent MC_suelo;
    MC_suelo.MeshOBJ = And::Geometry::load("cube.obj");

    And::MaterialComponent suelo_mat_comp;
    std::shared_ptr<And::Material> suelo_mat = std::make_shared<And::Material>();
    std::shared_ptr<And::Texture> suelo_tex_b = And::MakeTexture("demo/textures/suelo_2/albedo.png");
    std::shared_ptr<And::Texture> suelo_normals_b = And::MakeTexture("demo/textures/suelo_2/normals.png");
    std::shared_ptr<And::Texture> suelo_ao_b = And::MakeTexture("demo/textures/suelo_2/ao.png");
    std::shared_ptr<And::Texture> suelo_metallic_b = And::MakeTexture("demo/textures/suelo_2/metallic.png");
    std::shared_ptr<And::Texture> suelo_rou_b = And::MakeTexture("demo/textures/suelo_2/roughness.png");
    suelo_mat->SetColorTexture(suelo_tex_b);
    suelo_mat->SetNormalTexture(suelo_normals_b);
    suelo_mat->SetAmbientOclusionTexture(suelo_ao_b);
    suelo_mat->SetMetallicTexture(suelo_metallic_b);
    suelo_mat->SetRoughnessTexture(suelo_rou_b);
    suelo_mat_comp.SetMaterial(suelo_mat);
    
    CreateSuelo(entity_comp, *physics_engine, MC_suelo, suelo_mat_comp);

  {


      And::AmbientLight ambient;
      ambient.SetDiffuseColor(1.0f, 1.0f, 1.0f);
      ambient.SetAmbientStrenght(0.025f);
      //And::Entity* ambient_entity = entity_comp.new_entity(ambient);


      And::DirectionalLight directional;
      directional.SetDiffuseColor(1.0f, 1.0f, 1.0f);
      directional.SetDirection(0.5f, -0.5f, 0.5f);
      directional.SetSpecularColor(1.0f, 1.0f, 1.0f);
      directional.SetSpecularShininess(32.0f);
      directional.SetSpecularStrength(0.003f);
      directional.SetEnabled(true);
      directional.SetCastShadows(false);
      directional.SetIntensity(1.0f);
      entity_comp.new_entity(directional);

      float intensity = 300.0f;
      And::PointLight point;
      point.SetPosition(0.0f, 14.0f, 0.0f);
      point.SetSpecularColor(1.0f, 1.0f, 1.0f);
      point.SetSpecularShininess(32.0f);
      point.SetSpecularStrength(0.003f);
      point.SetConstantAtt(1.0f);
      point.SetLinearAtt(0.045f);
      point.SetQuadraticAtt(0.0075f);
      point.SetEnabled(true);
      point.SetCastShadows(true);
      point.SetIntensity(intensity);
      point.SetDiffuseColor(1.0f, 1.0f, 1.0f);
      //And::Entity* entity_tmp = entity_comp.new_entity(point);

      And::SpotLight spot{};
      spot.SetPosition(0.0f, 16.0f, 84.0f);
      spot.SetDirection(0.0f, -1.0f, 0.0f);
      spot.SetDiffuseColor(1.0f, 1.0f, 1.0f);
      spot.SetSpecularColor(1.0f, 1.0f, 1.0f);
      spot.SetSpecularStrength(0.003f);
      spot.SetSpecularShininess(32.0f);
      spot.SetConstantAtt(1.0f);
      spot.SetLinearAtt(0.045f);
      spot.SetQuadraticAtt(0.0075f);
      spot.SetCuttOff(2.5f);
      spot.SetOuterCuttOff(50.0f);
      spot.SetCastShadows(true);
      spot.SetEnabled(true);
      spot.SetIntensity(intensity);
      //entity_comp.new_entity(spot);
  }

  std::shared_ptr<And::Geometry> geo = And::Geometry::load("cube.obj");
  float position_tmp[3] = {-1.0f, 20.0f, -15.0f};



  And::Input input{ *window };
  And::ActionInput jump{ "Jump", And::KeyState::Press, { And::KeyCode::Space} };
  And::ActionInput shot{ "Shot", And::KeyState::Press, { And::KeyCode::C} };
  And::ActionInput big_shot{ "BigShot", And::KeyState::Press, { And::KeyCode::V} };
  And::ActionInput changePoint{ "ChangePoint", And::KeyState::Press, { And::KeyCode::L} };

  And::Entity* balls_pool[POOL_SIZE];
  int index_pool = 0;


  And::MaterialComponent mat_comp;
  std::shared_ptr<And::Material> mat = std::make_shared<And::Material>();
  std::shared_ptr<And::Texture> tex = And::MakeTexture("demo/textures/bola/albedo.png");
  std::shared_ptr<And::Texture> normals = And::MakeTexture("demo/textures/bola/normals.png");
  std::shared_ptr<And::Texture> ao = And::MakeTexture("demo/textures/bola/ao.png");
  std::shared_ptr<And::Texture> metallic = And::MakeTexture("demo/textures/bola/metallic.png");
  std::shared_ptr<And::Texture> rou = And::MakeTexture("demo/textures/bola/roughness.png");


  mat->SetColor(0.2f, 0.2f, 1.0f, 1.0f);
  mat->SetColor(0.353f, 0.698f, 1.0f, 1.0f);
  mat->SetColorTexture(tex);
  mat->SetNormalTexture(normals);
  mat->SetAmbientOclusionTexture(ao);
  mat->SetMetallicTexture(metallic);
  mat->SetRoughnessTexture(rou);

  mat_comp.SetMaterial(mat);

  CreateBallsPool(balls_pool, physics_engine, entity_comp, bolinga_mat_comp);

  And::Entity* linterna = CreateLinterna(entity_comp);
  And::Entity* vela;

  {
      And::MaterialComponent mat_com;
      std::shared_ptr<And::Material> mat = std::make_shared<And::Material>();
      std::shared_ptr<And::Texture> tex = And::MakeTexture("demo/textures/human/albedo.png");
      std::shared_ptr<And::Texture> normals = And::MakeTexture("demo/textures/human/normals.png");
      std::shared_ptr<And::Texture> ao = And::MakeTexture("demo/textures/human/ao.png");
      std::shared_ptr<And::Texture> metallic = And::MakeTexture("demo/textures/human/metallic.png");
      std::shared_ptr<And::Texture> rou = And::MakeTexture("demo/textures/human/roughness.png");
      mat->SetColorTexture(tex);
      mat->SetNormalTexture(normals);
      mat->SetAmbientOclusionTexture(ao);
      mat->SetMetallicTexture(metallic);
      mat->SetRoughnessTexture(rou);
      mat_com.SetMaterial(mat);

      And::MeshComponent MC;
      MC.MeshOBJ = And::Geometry::load("demo/obj/vela_simple_01.obj");

      And::TransformComponent tr;
      tr.SetPosition(0.0f, 0.0f, 20.0f);
      tr.SetRotation(0.0f, 0.0f, 0.0f);
      tr.SetScale(10.0f, 10.0f, 10.0f);
      tr.HasRigidBody(false);

     vela = entity_comp.new_entity(mat_com, MC, tr);

  }


  {
      And::MaterialComponent mat_com_bola_bolos;
      std::shared_ptr<And::Material> mat_bola_bolos = std::make_shared<And::Material>();
      std::shared_ptr<And::Texture> tex_bola_bolos = And::MakeTexture("demo/textures/titanio/albedo.png");
      std::shared_ptr<And::Texture> normals_bola_bolos = And::MakeTexture("demo/textures/titanio/normals.png");
      std::shared_ptr<And::Texture> ao_bola_bolos = And::MakeTexture("demo/textures/titanio/ao.png");
      std::shared_ptr<And::Texture> metallic_bola_bolos = And::MakeTexture("demo/textures/titanio/metallic.png");
      std::shared_ptr<And::Texture> rou_bola_bolos = And::MakeTexture("demo/textures/titanio/roughness.png");
      mat_bola_bolos->SetColorTexture(tex_bola_bolos);
      mat_bola_bolos->SetNormalTexture(normals_bola_bolos);
      mat_bola_bolos->SetAmbientOclusionTexture(ao_bola_bolos);
      mat_bola_bolos->SetMetallicTexture(metallic_bola_bolos);
      mat_bola_bolos->SetRoughnessTexture(rou_bola_bolos);
      mat_com_bola_bolos.SetMaterial(mat_bola_bolos);
      
      And::MeshComponent MC_bola_bolos;
      MC_bola_bolos.MeshOBJ = And::Geometry::load("sphere.obj");

      And::TransformComponent bola_bolos_tr;
      bola_bolos_tr.SetPosition(8.0f, 3.0f, 0.0f);
      bola_bolos_tr.SetRotation(0.0f, 0.0f, 0.0f);
      bola_bolos_tr.SetScale(1.0f, 1.0f, 1.0f);
      bola_bolos_tr.HasRigidBody(false);

      entity_comp.new_entity(mat_com_bola_bolos, MC_bola_bolos, bola_bolos_tr);

  }

  {
      And::MaterialComponent mat_com;
      std::shared_ptr<And::Material> mat= std::make_shared<And::Material>();
      std::shared_ptr<And::Texture> tex= And::MakeTexture("demo/textures/bambu/albedo.png");
      std::shared_ptr<And::Texture> normals= And::MakeTexture("demo/textures/bambu/normals.png");
      std::shared_ptr<And::Texture> ao= And::MakeTexture("demo/textures/bambu/ao.png");
      std::shared_ptr<And::Texture> metallic= And::MakeTexture("demo/textures/bambu/metallic.png");
      std::shared_ptr<And::Texture> rou= And::MakeTexture("demo/textures/bambu/roughness.png");
      mat->SetColorTexture(tex);
      mat->SetNormalTexture(normals);
      mat->SetAmbientOclusionTexture(ao);
      mat->SetMetallicTexture(metallic);
      mat->SetRoughnessTexture(rou);
      mat_com.SetMaterial(mat);

      And::MeshComponent MC;
      MC.MeshOBJ = And::Geometry::load("demo/obj/bowling_pin.obj");

      And::TransformComponent tr;
      tr.SetPosition(10.0f, 1.0f, 0.0f);
      tr.SetRotation(0.0f, 0.0f, 0.0f);
      tr.SetScale(5.0f, 5.0f, 5.0f);
      tr.HasRigidBody(false);

      entity_comp.new_entity(mat_com, MC, tr);

      CreateBolos(entity_comp, *physics_engine, MC, mat_com);

  }

  {
      /*
      And::MaterialComponent mat_com;
      std::shared_ptr<And::Material> mat = std::make_shared<And::Material>();
      std::shared_ptr<And::Texture> tex = And::MakeTexture("demo/textures/concreto/albedo.png");
      std::shared_ptr<And::Texture> normals = And::MakeTexture("demo/textures/concreto/normals.png");
      std::shared_ptr<And::Texture> ao = And::MakeTexture("demo/textures/concreto/ao.png");
      std::shared_ptr<And::Texture> metallic = And::MakeTexture("demo/textures/concreto/metallic.png");
      std::shared_ptr<And::Texture> rou = And::MakeTexture("demo/textures/concreto/roughness.png");
      mat->SetColorTexture(tex);
      mat->SetNormalTexture(normals);
      mat->SetAmbientOclusionTexture(ao);
      mat->SetMetallicTexture(metallic);
      mat->SetRoughnessTexture(rou);
      mat_com.SetMaterial(mat);

      And::MeshComponent MC;
      MC.MeshOBJ = And::Geometry::load("demo/obj/arco.obj");

      And::TransformComponent tr;
      tr.SetPosition(0.0f, 0.0f, 20.0f);
      tr.SetRotation(0.0f, 0.0f, 0.0f);
      tr.SetScale(10.0f, 5.0f, 5.0f);
      tr.HasRigidBody(false);

      entity_comp.new_entity(mat_com, MC, tr);
      */

  }


  And::AudioManager audio_manager;
  And::Audio audio_fondo;
  And::Audio audio_fuente;
  And::Audio audio_piano;
  audio_fondo.load("demo/audio/fondo.wav");
  audio_fuente.load("demo/audio/fuente.wav");
  audio_piano.load("demo/audio/piano.wav");

  
  audio_fondo.SetGain(0.3f);
  audio_fondo.SetLooping(true);

  audio_fuente.SetPosition(0.0f, 0.0f, 0.0f);
  audio_fuente.SetDoppler(true);
  audio_fuente.SetDopplerFactor(10.0f);
  audio_fuente.SetLooping(true);
  audio_fuente.SetPitch(1.0f);
  audio_fuente.SetMaxDistance(10.0f);

  audio_manager.play(audio_fondo);
  audio_manager.play(audio_fuente);


  float fps_count = 0.0f;
  const float force = 600.0f;
  int frames = 0;
  float secondsToSpawn = 4.0f;

  float fixed_update = 0.0f;

  float time = 0.0f;

  bool is_down = false;
  bool is_big_down = false;
  bool is_pointlight_down = false;

  //And::TransformComponent* tr_tmp = bolinga_entity->get_component<And::TransformComponent>();

  while (window->is_open()){
       
    
    window->update();
    g_renderer->new_frame();
    editor.ShowWindows();
    fps_count +=window->get_delta_time(); 
    fixed_update +=window->get_delta_time(); 

    //if(input.IsMouseButtonPressed(And::MouseCode::Left)){
    if (input.check_action(shot)) {

        //static void LaunchBall(const float* pos, const float* dir, float force, And::EntityComponentSystem & ecs, And::PhysicsEngine & engine, And::MeshComponent & mesh, And::MaterialComponent & material_comp)
        if (!is_down) {
            LaunchBall(fly_cam.GetPosition(), fly_cam.GetDirection(), force, entity_comp, *physics_engine, MC_bolinga, bolinga_mat_comp);
        }
        is_down = true;
    }else {
        is_down = false;
    }
    
    if (input.check_action(big_shot)) {

        //static void LaunchBall(const float* pos, const float* dir, float force, And::EntityComponentSystem & ecs, And::PhysicsEngine & engine, And::MeshComponent & mesh, And::MaterialComponent & material_comp)
        if (!is_big_down) {
            LaunchBall(fly_cam.GetPosition(), fly_cam.GetDirection(), force, entity_comp, *physics_engine, MC_bolinga, bolinga_mat_comp, true);
        }
        is_big_down = true;
    }else {
        is_big_down = false;
    }


    if (input.check_action(jump)) {
        //And::AmbientLight* tmp = ambient_entity->get_component<And::AmbientLight>();
        //tmp->SetEnabled(!tmp->GetEnabled());
    }


    if (input.check_action(changePoint)) {

        //static void LaunchBall(const float* pos, const float* dir, float force, And::EntityComponentSystem & ecs, And::PhysicsEngine & engine, And::MeshComponent & mesh, And::MaterialComponent & material_comp)
        if (!is_pointlight_down) {
            And::PointLight* tmp = linterna->get_component<And::PointLight>();
            tmp->SetEnabled(!tmp->GetEnabled());
        }
        is_pointlight_down = true;
    }else {
        is_pointlight_down = false;
    }

   
    //float* pos = point_tmp->GetPosition();
    //point_tmp->SetPosition(pos[0], std::abs(cosf(fps_count) * 3.0f), pos[2]);
    //printf("Position-> Y: %f\n", std::abs(cosf(fps_count) * 3.0f));

    fly_cam.ProcessInput();
    fly_cam.ShowValues();
    
    UpdateLight(linterna, vela, fly_cam);




    const float* src_pos = fly_cam.GetPosition();
    const float* src_dir = fly_cam.GetDirection();

    audio_fuente.UpdateListenerPosition(src_pos);
    audio_fuente.UpdateListenerDirection(src_dir);
     
    audio_fuente.ApplyEffects();
   

    audio_manager.Update();
    if (fps_count >= secondsToSpawn) {
        //SpawnBall(physics_engine, entity_comp);
        ThrowBall(balls_pool, index_pool);
        fps_count -= secondsToSpawn;
        index_pool++;
        if (index_pool >= POOL_SIZE)index_pool = 0;
    }


    And::TransformComponent* tmp_nose = bolinga_entity->get_component<And::TransformComponent>();
    tmp_nose->SetRotation(tmp_nose->rotation[0], time, tmp_nose->rotation[2]);

    physics_engine->Simulate(window->get_delta_time() > 1.0f ? 1.0f / 30.0f : window->get_delta_time());
    physics_engine->Apply(entity_comp);
        

    
    //g_renderer->draw_forward(entity_comp);
    //g_renderer->draw_deferred(entity_comp);
    g_renderer->draw_pbr(entity_comp);
    
    frames++;
    time += window->get_delta_time();
    g_renderer->end_frame();
    window->swap_buffers();
  }

  physics_engine->Release(entity_comp);

  return 0;
}