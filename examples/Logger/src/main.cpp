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

  std::shared_ptr<And::Window> window = And::Window::make(e, 1024, 720, "Andromeda Engine");
  And::Renderer renderer(*window);

  And::LogWindow log_window;

  AND_LOG(TestLogCategory, And::Trace, "Some trace Message");
  AND_LOG(TestLogCategory, And::Debug, "Some debug Message");
  AND_LOG(TestLogCategory, And::Info, "Some info Message");
  AND_LOG(TestLogCategory, And::Warning, "Some warning Message");
  AND_LOG(TestLogCategory, And::Error, "Some error Message");
  AND_LOG(TestLogCategory, And::Critical, "Some critical Message");

  while (window->is_open())
  {
    window->update();
    renderer.new_frame();

    AND_LOG(TestLogCategory, And::Debug, "Some debug Message");

    log_window.Draw();

    renderer.end_frame();
    window->swap_buffers();
  }

  return 0;
}