#include "Core/Window.h"
#include "Platform/Windows/WindowGLFW.h"

namespace And
{
  Window::Window() {}

  Window::~Window() {}

  std::shared_ptr<Window> Window::Create(const WindowCreationInfo& info)
  {
  # ifdef _WIN32
    return std::shared_ptr<Window>(new WindowGLFW(info));
  # endif
  }
}

