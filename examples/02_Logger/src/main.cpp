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
#include <optional>
#include <vector>
#include <unordered_map>
#include <format>

#include "Andromeda.h"

#include "TestLogCategory.h"

int main(int argc, char** argv){
  And::Engine e;

  std::shared_ptr<And::Window> window = And::Window::make(e, 1024, 720, "Andromeda Engine", And::EGraphicsApiType::OpenGL);
  std::shared_ptr<And::Renderer> g_renderer = And::Renderer::CreateShared(*window);

  And::LogWindow log_window("Logger window");

  AND_LOG(TestLogCategory1, And::Trace, "Some trace Message");
  AND_LOG(TestLogCategory1, And::Debug, "Some debug Message");
  AND_LOG(TestLogCategory1, And::Info, "Some info Message");
  AND_LOG(TestLogCategory1, And::Warning, "Some warning Message");
  AND_LOG(TestLogCategory1, And::Error, "Some error Message");
  AND_LOG(TestLogCategory1, And::Critical, "Some critical Message");

  AND_LOG(TestLogCategory2, And::Trace, "Some trace Message");
  AND_LOG(TestLogCategory2, And::Debug, "Some debug Message");
  AND_LOG(TestLogCategory2, And::Info, "Some info Message");
  AND_LOG(TestLogCategory2, And::Warning, "Some warning Message");
  AND_LOG(TestLogCategory2, And::Error, "Some error Message");
  AND_LOG(TestLogCategory2, And::Critical, "Some critical Message");

  AND_LOG(TestLogCategory3, And::Trace, "Some trace Message");
  AND_LOG(TestLogCategory3, And::Debug, "Some debug Message");
  AND_LOG(TestLogCategory3, And::Info, "Some info Message");
  AND_LOG(TestLogCategory3, And::Warning, "Some warning Message");
  AND_LOG(TestLogCategory3, And::Error, "Some error Message");
  AND_LOG(TestLogCategory3, And::Critical, "Some critical Message");

  while (window->is_open())
  {
    window->update();
    g_renderer->new_frame();

    //AND_LOG(TestLogCategory1, And::Debug, "Some debug Message");

    log_window.Show();

    g_renderer->end_frame();
    window->swap_buffers();
  }

  return 0;
}