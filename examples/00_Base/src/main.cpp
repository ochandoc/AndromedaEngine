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

  std::shared_ptr<And::Window> window = And::Window::make(e, 1920, 1080, "Andromeda Engine");
  std::shared_ptr<And::GraphicsContext> g_context = window->get_context();
  And::Renderer g_renderer(*window);

  And::EntityComponentSystem entity_comp;
  And::AddBasicComponents(entity_comp);

  And::MeshComponent MC;
  MC.MeshOBJ = And::Geometry::load("teapot.obj");
  
  And::TransformComponent tran;
  tran.position[0] = 0.0f;
  tran.position[1] = -4.0f;
  tran.position[2] = -10.0f;
  tran.rotation[0] = 1.0f;
  tran.rotation[1] = 1.0f;
  tran.rotation[2] = 1.0f;
  tran.scale[0] = 1.0f;
  tran.scale[1] = 1.0f;
  tran.scale[2] = 1.0f;

  And::Entity* obj_id = entity_comp.new_entity(MC, tran);

  And::DirectionalLight directional{};
  directional.SetDirection(1.0f, 0.0f, 0.0f);
  directional.SetDiffuseColor(1.0f, 1.0f, 1.0f);
  directional.SetSpecularColor(1.0f, 1.0f, 1.0f);
  directional.SetEnabled(true);
  And::Entity* light_directional_entity = entity_comp.new_entity(directional);
  
  //directional.SetCastShadows(true);
  
  while (window->is_open()){
    window->update();
    g_renderer.new_frame();

    And::DrawForward(entity_comp, g_renderer);
    

    g_renderer.end_frame();
    window->swap_buffers();
  }

    //And::DrawForward(entity_comp, g_renderer);
  return 0;
}