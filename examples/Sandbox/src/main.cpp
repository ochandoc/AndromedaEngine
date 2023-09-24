#include "Core/Window.h"

int main(int argc, char** argv)
{
  WindowCreationInfo WindowInfo;
  WindowInfo.width = 1024;
  WindowInfo.height = 720;
  WindowInfo.title = "Andromeda Engine";


  std::shared_ptr<Window> window = Window::Create(WindowInfo);

  while (window->is_open())
  {
    window->update();
  }
  
  
  return 0;
}