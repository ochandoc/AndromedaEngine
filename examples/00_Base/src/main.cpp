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


int main(int argc, char** argv){

  And::Engine e;
  And::TaskSystem ts;

  std::shared_ptr<And::Window> window = And::Window::make(e, 1920, 1080, "Andromeda Engine", And::EGraphicsApiType::OpenGL);
  std::shared_ptr<And::GraphicsContext> g_context = window->get_context();
  std::shared_ptr<And::Renderer> g_renderer = And::Renderer::CreateShared(*window);

  And::Editor editor{*window};
  editor.AddWindow(ts.GetEditorWindow());

  And::EntityComponentSystem entity_comp;
  And::AddBasicComponents(entity_comp);

  And::MaterialComponent mat_comp;
  std::shared_ptr<And::Material> m_ = std::make_shared<And::Material>();
  std::shared_ptr<And::Texture> tex_ = And::MakeTexture("billboard_bulb.png");
  m_->SetColorTexture(tex_);
  mat_comp.SetMaterial(m_);

  And::MeshComponent MC;
  MC.MeshOBJ = And::Geometry::load("cube.obj");
  And::RawMesh raw_mesh(MC.MeshOBJ->get_vertices(), MC.MeshOBJ->get_indices());
  std::shared_ptr<And::Mesh> mesh = std::make_shared<And::Mesh>(raw_mesh);
  MC.SetMesh(mesh);
  
  
  And::TransformComponent tran;
  tran.position[0] = 0.0f;
  tran.position[1] = 0.0f;
  tran.position[2] = -3.0f;
  tran.rotation[0] = 1.0f;
  tran.rotation[1] = 1.0f;
  tran.rotation[2] = 1.0f;
  tran.scale[0] = 1.0f;
  tran.scale[1] = 1.0f;
  tran.scale[2] = 1.0f;

  And::Entity* obj_id = entity_comp.new_entity(MC, tran);

  And::DirectionalLight directional{};
  directional.SetDirection(1.0f, -1.0f, -1.0f);
  directional.SetDiffuseColor(1.0f, 1.0f, 1.0f);
  directional.SetSpecularColor(1.0f, 1.0f, 1.0f);
  directional.SetEnabled(true);
  directional.SetCastShadows(true);
  And::Entity* light_directional_entity = entity_comp.new_entity(directional);

  And::SpotLight spot{};
  spot.SetEnabled(true);
  spot.SetPosition(0.0f, 0.0f, 0.0f);
  spot.SetDirection(0.0f, 0.0f, -1.0f);
  spot.SetDiffuseColor(1.0f, 0.0f, 0.0f);
  spot.SetSpecularColor(1.0f, 1.0f, 1.0f);
  spot.SetSpecularStrength(0.003f);
  spot.SetSpecularShininess(8.0f);
  spot.SetConstantAtt(1.0);
  spot.SetLinearAtt(0.014f);
  spot.SetQuadraticAtt(0.0007f);
  spot.SetCuttOff(2.5f);
  spot.SetOuterCuttOff(17.5f);
  spot.SetCastShadows(true);
  //entity_comp.new_entity(spot);
  

  And::PointLight point{};
  point.SetEnabled(1.0f);
  point.SetPosition(14.0f, -4.0f, 4.0f);
  point.SetDiffuseColor(0.0f, 1.0f, 0.0f);
  point.SetSpecularStrength(0.003f);
  point.SetSpecularColor(1.0f, 1.0f, 1.0f);
  point.SetSpecularShininess(8.0f);
  point.SetConstantAtt(1.0f);
  point.SetLinearAtt(0.014f);
  point.SetQuadraticAtt(0.0007f);
  point.SetCastShadows(true);
  //And::Entity* point_entity = entity_comp.new_entity(point);

  And::AmbientLight ambient{};
  ambient.SetDiffuseColor(0.1f, 0.1f, 0.1f);
  entity_comp.new_entity(ambient);

  And::Input input{*window};
  And::ActionInput jump{"Jump", And::KeyState::Press, { And::KeyCode::Space, And::KeyCode::J }};

  struct ExampleStruct{
    int a;
    float b;
    char c;
  };

  ExampleStruct test_save;
  test_save.a = 3;
  test_save.b = 3.3f;
  test_save.c = 'F';

  And::SavedObject save_object{};
  bool result = save_object.save<ExampleStruct>("test", test_save);

  ExampleStruct prueba;
  save_object.load<ExampleStruct>("test", prueba); 

  double mouse_x, mouse_y;

  And::TransformComponent* tr = obj_id->get_component<And::TransformComponent>();



  And::AudioManager audio_manager{};
  And::Audio dindindon_audio{};
  dindindon_audio.load("Audio/dindindon.wav");

  dindindon_audio.SetPitch(1.5f);
  dindindon_audio.SetGain(2.0f);
  dindindon_audio.SetPosition(5.0f, 0.0f, 0.0f);
  
  
  audio_manager.play(dindindon_audio);


  
  while (window->is_open()){
    window->update();
    g_renderer->new_frame();
    editor.ShowWindows();

    tr->position[0] += 0.001f;


    if (input.IsKeyDown(And::KeyCode::W)){
      printf("W Pressed!\n");
    }

    if(input.check_action(jump)){
      printf("Jump!\n");
    }

    input.GetMousePosition(&mouse_x, &mouse_y);

    mouse_x = input.GetMouseX();
    mouse_y = input.GetMouseY();

    g_renderer->end_frame();
    window->swap_buffers();
  }

  return 0;
}