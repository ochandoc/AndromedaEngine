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

const float PI = 3.14159265f;

void CreateHabitaculo(And::EntityComponentSystem& ecs) {

    const float scale_suelo = 50.0f;
    
    And::MaterialComponent mat_com;
    std::shared_ptr<And::Material> mat = std::make_shared<And::Material>();
    std::shared_ptr<And::Texture> tex = And::MakeTexture("demo/textures/suelo_3/albedo.png");
    std::shared_ptr<And::Texture> normals = And::MakeTexture("demo/textures/suelo_3/normals.png");
    std::shared_ptr<And::Texture> ao = And::MakeTexture("demo/textures/suelo_3/ao.png");
    std::shared_ptr<And::Texture> metallic = And::MakeTexture("demo/textures/suelo_3/metallic.png");
    std::shared_ptr<And::Texture> rou = And::MakeTexture("demo/textures/suelo_3/roughness.png");
    mat->SetColorTexture(tex);
    mat->SetNormalTexture(normals);
    mat->SetAmbientOclusionTexture(ao);
    mat->SetMetallicTexture(metallic);
    mat->SetRoughnessTexture(rou);
    mat_com.SetMaterial(mat);

    And::MeshComponent MC;
    MC.MeshOBJ = And::Geometry::load("cube.obj");
    And::RawMesh raw_mesh_vela_tmp(MC.MeshOBJ->get_vertices(), MC.MeshOBJ->get_indices());
    std::shared_ptr<And::Mesh> mesh_vela_tmp = std::make_shared<And::Mesh>(raw_mesh_vela_tmp);
    MC.SetMesh(mesh_vela_tmp);

    And::TransformComponent tr;
    tr.SetPosition(0.0f, 0.0f, 0.0f);
    tr.SetRotation(0.0f, 0.0f, 0.0f);
    tr.SetScale(scale_suelo, 1.0f, scale_suelo);
    tr.HasRigidBody(false);
    ecs.new_entity(mat_com, MC, tr);


    And::MaterialComponent mat_com_pared;
    std::shared_ptr<And::Material> mat_pared = std::make_shared<And::Material>();
    std::shared_ptr<And::Texture> tex_pared = And::MakeTexture("demo/textures/suelo_6/albedo.png");
    std::shared_ptr<And::Texture> normals_pared = And::MakeTexture("demo/textures/suelo_6/normals.png");
    std::shared_ptr<And::Texture> ao_pared = And::MakeTexture("demo/textures/suelo_6/ao.png");
    std::shared_ptr<And::Texture> metallic_pared = And::MakeTexture("demo/textures/suelo_6/metallic.png");
    std::shared_ptr<And::Texture> rou_pared = And::MakeTexture("demo/textures/suelo_6/roughness.png");
    mat_pared->SetColorTexture(tex_pared);
    mat_pared->SetNormalTexture(normals_pared);
    mat_pared->SetAmbientOclusionTexture(ao_pared);
    mat_pared->SetMetallicTexture(metallic_pared);
    mat_pared->SetRoughnessTexture(rou_pared);
    mat_com_pared.SetMaterial(mat_pared);

    
    tr.SetScale(1.0f, 30.0f, scale_suelo);
    tr.SetPosition(scale_suelo * -0.5f, 15.0f, 0.0f);
    ecs.new_entity(mat_com_pared, MC, tr);
    
    tr.SetScale(1.0f, 30.0f, scale_suelo);
    tr.SetPosition(scale_suelo * 0.5f, 15.0f, 0.0f);
    ecs.new_entity(mat_com_pared, MC, tr);
    
    tr.SetScale(scale_suelo, 30.0f, 1.0f);
    tr.SetPosition(0.0f, 15.0f, scale_suelo * 0.5f);
    ecs.new_entity(mat_com_pared, MC, tr);



}

void CreateLighting(And::EntityComponentSystem& ecs) {

    And::AmbientLight ambient;
    ambient.SetDiffuseColor(1.0f, 1.0f, 1.0f);
    ambient.SetAmbientStrenght(0.01f);
    ecs.new_entity(ambient);
    
    const float intensity = 300.0f;
    And::PointLight point;
    point.SetPosition(21.00f, 26.0f, 21.0f);
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
    ecs.new_entity(point);
}

void CreateFurnitures(And::EntityComponentSystem& ecs){
    
    // Mesa 
    {
        And::MaterialComponent mat_com;
        std::shared_ptr<And::Material> mat = std::make_shared<And::Material>();
        std::shared_ptr<And::Texture> tex = And::MakeTexture("demo/textures/bambu/albedo.png");
        std::shared_ptr<And::Texture> normals = And::MakeTexture("demo/textures/bambu/normals.png");
        std::shared_ptr<And::Texture> ao = And::MakeTexture("demo/textures/bambu/ao.png");
        std::shared_ptr<And::Texture> metallic = And::MakeTexture("demo/textures/bambu/metallic.png");
        std::shared_ptr<And::Texture> rou = And::MakeTexture("demo/textures/bambu/roughness.png");
        mat->SetColorTexture(tex);
        mat->SetNormalTexture(normals);
        mat->SetAmbientOclusionTexture(ao);
        mat->SetMetallicTexture(metallic);
        mat->SetRoughnessTexture(rou);
        mat_com.SetMaterial(mat);

        And::MeshComponent MC;
        MC.MeshOBJ = And::Geometry::load("demo/obj/office_desk.obj");
        And::RawMesh raw_mesh_mesa(MC.MeshOBJ->get_vertices(), MC.MeshOBJ->get_indices());
        std::shared_ptr<And::Mesh> mesh = std::make_shared<And::Mesh>(raw_mesh_mesa);
        MC.SetMesh(mesh);

        And::TransformComponent tr;
        tr.SetPosition(20.0f, 1.0f, 0.0f);
        tr.SetRotation(0.0f,PI / 2.0f, 0.0f);
        tr.SetScale(10.0f, 10.0f, 10.0f);
        tr.HasRigidBody(false);
        ecs.new_entity(mat_com, MC, tr);
    }

    // Guitarra 
    {
        And::MaterialComponent mat_com;
        std::shared_ptr<And::Material> mat = std::make_shared<And::Material>();
        std::shared_ptr<And::Texture> tex = And::MakeTexture("demo/textures/bambu/albedo.png");
        std::shared_ptr<And::Texture> normals = And::MakeTexture("demo/textures/bambu/normals.png");
        std::shared_ptr<And::Texture> ao = And::MakeTexture("demo/textures/bambu/ao.png");
        std::shared_ptr<And::Texture> metallic = And::MakeTexture("demo/textures/bambu/metallic.png");
        std::shared_ptr<And::Texture> rou = And::MakeTexture("demo/textures/bambu/roughness.png");
        mat->SetColorTexture(tex);
        mat->SetNormalTexture(normals);
        mat->SetAmbientOclusionTexture(ao);
        mat->SetMetallicTexture(metallic);
        mat->SetRoughnessTexture(rou);
        mat_com.SetMaterial(mat);

        And::MeshComponent MC;
        MC.MeshOBJ = And::Geometry::load("demo/obj/guitar.obj");
        And::RawMesh raw_mesh_mesa(MC.MeshOBJ->get_vertices(), MC.MeshOBJ->get_indices());
        std::shared_ptr<And::Mesh> mesh = std::make_shared<And::Mesh>(raw_mesh_mesa);
        MC.SetMesh(mesh);

        And::TransformComponent tr;
        tr.SetPosition(-21.0f, 1.0f, 21.0f);
        tr.SetRotation(PI / -8.0f, PI, 0.0f);
        tr.SetScale(10.0f, 10.0f, 10.0f);
        tr.HasRigidBody(false);
        ecs.new_entity(mat_com, MC, tr);
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

    //std::shared_ptr<And::PhysicsEngine> physics_engine = And::PhysicsEngine::Init(false, 8192);

  {


      


      And::DirectionalLight directional;
      directional.SetDiffuseColor(1.0f, 1.0f, 1.0f);
      directional.SetDirection(0.5f, -0.5f, 0.5f);
      directional.SetSpecularColor(1.0f, 1.0f, 1.0f);
      directional.SetSpecularShininess(32.0f);
      directional.SetSpecularStrength(0.003f);
      directional.SetEnabled(true);
      directional.SetCastShadows(false);
      directional.SetIntensity(1.0f);
      //entity_comp.new_entity(directional);

      

      {
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
          //spot.SetIntensity(intensity);
          //entity_comp.new_entity(spot);
      }
      

      CreateHabitaculo(entity_comp);
      CreateLighting(entity_comp);
      CreateFurnitures(entity_comp);
    
  }

  float time = 0.0f;
  while (window->is_open()){
       
    window->update();
    g_renderer->new_frame();
    editor.ShowWindows();
  
    fly_cam.ProcessInput();
    fly_cam.ShowValues();
    
    // Code Here

    





    //physics_engine->Simulate(window->get_delta_time() > 1.0f ? 1.0f / 30.0f : window->get_delta_time());
    //physics_engine->Apply(entity_comp);
    
    //g_renderer->draw_forward(entity_comp);
    //g_renderer->draw_deferred(entity_comp);
    g_renderer->draw_pbr(entity_comp);
    
    time += window->get_delta_time();
    g_renderer->end_frame();
    window->swap_buffers();
  }

  //physics_engine->Release(entity_comp);

  return 0;
}