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

  std::shared_ptr<And::Shader> shader = And::MakeShader("shader.hlsl");

  std::vector<uint32> indices = { 0, 2, 1, 0, 3, 2 };
  std::vector<And::Vertex> vertices = {
    {-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
    {-0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f},
    {0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f}, // top right
    {0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f} //
  };

  And::RawMesh cube = And::RawMesh::CreateCube();

  std::shared_ptr<And::VertexBuffer> vb = And::VertexBuffer::CreateShare(cube.GetVertices());
  std::shared_ptr<And::IndexBuffer> ib = And::IndexBuffer::CreateShared(cube.GetIndices());

  And::FlyCamera fly_cam{ *window };
  fly_cam.SetPosition(3.0f, 7.0f, 5.0f);

  fly_cam.SetFar(1000.0f);
  fly_cam.SetNear(0.1f);
  fly_cam.SetFov(90.0f);
  fly_cam.SetDirection(0.0f, 0.0f, -1.0f);

  g_renderer->set_camera(&fly_cam);

  float fps_count = 0.0f;
  while (window->is_open()){

    window->update();

    fly_cam.ProcessInput();
    g_renderer->new_frame();
    
    g_renderer->Draw(vb.get(), ib.get(), shader.get());

    g_renderer->end_frame();
    window->swap_buffers();
  }

  return 0;
}