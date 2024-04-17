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

#include "Andromeda/Graphics/IndexBuffer.h"
#include "Andromeda/Graphics/VertexBuffer.h"
#include "Andromeda/Graphics/Shader.h"
#include "Andromeda/Graphics/RawMesh.h"
#include "Andromeda/ECS/Components/MaterialComponent.h"

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

  std::shared_ptr<And::Window> window = And::Window::make(e, 1080, 720, "Andromeda Engine", And::EGraphicsApiType::DirectX11);
  std::shared_ptr<And::GraphicsContext> g_context = window->get_context();
  std::shared_ptr<And::Renderer> g_renderer = And::Renderer::CreateShared(*window);

  And::EntityComponentSystem ecs;
  ecs.add_component_class<And::MeshComponent>();
  ecs.add_component_class<And::TransformComponent>();
  ecs.add_component_class<And::MaterialComponent>();
  ecs.add_component_class<And::DirectionalLight>();
  ecs.add_component_class<And::SpotLight>();
  ecs.add_component_class<And::PointLight>();

  std::shared_ptr<And::Shader> shader = And::MakeShader("shader.hlsl");

  std::vector<uint32> indices = { 0, 2, 1, 0, 3, 2 };
  std::vector<And::Vertex> vertices = {
    {-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
    {-0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f},
    {0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f}, // top right
    {0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f} //
  };

  And::RawMesh cube = And::RawMesh::CreateCube();

  std::shared_ptr<And::Mesh> cube_mesh = std::make_shared<And::Mesh>(cube);

  std::vector<std::string> SkyboxPaths = {
  "skybox/right.jpg",
  "skybox/left.jpg",
  "skybox/top.jpg",
  "skybox/bottom.jpg",
  "skybox/back.jpg",
  "skybox/front.jpg",
  };
  std::shared_ptr<And::SkyboxTexture> SkyboxTexture = And::MakeSkyboxTexture(SkyboxPaths);
  std::shared_ptr<And::Texture> JouTexture = And::MakeTexture("jou.jpg");

  g_renderer->enable_skybox(true);
  g_renderer->set_skybox_texture(SkyboxTexture);

  And::DirectionalLight directional{};
  directional.SetDirection(1.0f, 0.0f, 0.0f);
  directional.SetDiffuseColor(1.0f, 0.0f, 0.0f);
  directional.SetSpecularColor(1.0f, 1.0f, 1.0f);
  directional.SetCastShadows(true);
  directional.SetEnabled(true);
  ecs.new_entity(directional);

  float diffuse_color[3] = { 1.0f, 1.0f, 1.0f };
  float specular_color[3] = { 1.0f, 1.0f, 1.0f };

  float specular_strength = 0.003f;
  float specular_shininess = 8.0f;
  float constant_att = 1.0f;
  float linear_att = 0.09f;
  float quadratic_att = 0.032f;
  float cutt_off = 2.5f;
  float outer_cut_off = 17.5f;


  And::SpotLight spot{};
  spot.SetPosition(6.0f, 5.0f, -5.0f);
  spot.SetDirection(-1.0f, 0.0f, 0.0f);
  spot.SetDiffuseColor(0.0f, 1.0f, 0.0f);
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
  ecs.new_entity(spot);

  And::PointLight point{};
  point.SetPosition(6.0f, 5.0f, -5.0f);
  point.SetEnabled(1.0f);
  point.SetDiffuseColor(0.0f, 0.0f, 1.0f);
  point.SetSpecularStrength(specular_strength);
  point.SetSpecularColor(specular_color);
  point.SetSpecularShininess(specular_shininess);
  point.SetCastShadows(true);
  point.SetLinearAtt(linear_att);
  point.SetConstantAtt(constant_att);
  point.SetQuadraticAtt(quadratic_att);
  point.SetEnabled(true);
  And::Entity* point_ett = ecs.new_entity(point);
  
  And::Entity* ett;
  for (int i = 0; i < 10; ++i)
  {
    And::MeshComponent CubeMeshComponent;
    CubeMeshComponent.SetMesh(cube_mesh);

    And::MaterialComponent MaterialComponent;
    MaterialComponent.GetMaterial()->SetColorTexture(JouTexture);
    MaterialComponent.GetMaterial()->SetColor(1.0f, 0.0f, 0.0f, 1.0f);

    And::TransformComponent cube_transform;
    cube_transform.position[0] = 3.0f * (float)i;
    cube_transform.position[1] = 5.0f;
    cube_transform.position[2] = -5.0f;

    cube_transform.rotation[0] = 0.0f;
    cube_transform.rotation[1] = 0.0f;
    cube_transform.rotation[2] = 0.0f;

    cube_transform.scale[0] = 1.0f;
    cube_transform.scale[1] = 1.0f;
    cube_transform.scale[2] = 1.0f;

    And::Entity* ett1 = ecs.new_entity(CubeMeshComponent, cube_transform, MaterialComponent);
    if (i == 1)
      ett = ett1;
  }


  And::FlyCamera fly_cam{ *window };
  fly_cam.SetPosition(3.0f, 7.0f, 5.0f);

  fly_cam.SetFar(1000.0f);
  fly_cam.SetNear(0.1f);
  fly_cam.SetFov(90.0f);
  fly_cam.SetDirection(0.0f, 0.0f, -1.0f);

  g_renderer->set_camera(&fly_cam);

  float time = 0.0f;
  while (window->is_open()){

    window->update();
    time += window->get_delta_time();

    fly_cam.ProcessInput();
    g_renderer->new_frame();

    float* pos = point_ett->get_component<And::PointLight>()->GetPosition();
    point_ett->get_component<And::PointLight>()->SetPosition(cosf(time * 0.01f) * 7.0f, pos[1], pos[2]);

    ett->get_component<And::TransformComponent>()->rotation[0] += window->get_delta_time();
    ett->get_component<And::TransformComponent>()->rotation[1] -= window->get_delta_time();

    g_renderer->draw_forward(ecs);
    g_renderer->end_frame();
    window->swap_buffers();
  }

  return 0;
}