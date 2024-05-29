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

#include "Andromeda.h"

int main(int argc, char** argv){
  And::Engine e;

  std::shared_ptr<And::Window> window = And::Window::make(e, 1024, 720, "Andromeda Engine", And::EGraphicsApiType::OpenGL);


  while (window->is_open()){
    window->update();

    //...

    window->swap_buffers();
  }

  return 0;
}