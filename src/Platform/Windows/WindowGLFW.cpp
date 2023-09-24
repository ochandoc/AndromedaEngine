#include "WindowGLFW.h"

static void close_window_callback(GLFWwindow* window)
{
  WindowGLFW* glfw_window = reinterpret_cast<WindowGLFW*>(glfwGetWindowUserPointer(window));
  if (glfw_window->m_OnWindowClose)
  {
    glfw_window->m_OnWindowClose();
  }
}

WindowGLFW::WindowGLFW(const WindowCreationInfo& info)
{
  
  glfwInit();
  m_Window = glfwCreateWindow(info.width, info.height, info.title.c_str(), NULL, NULL);

  // pointer to our window class, glfw se lo guarda y luego en el evento de close window podemos acceder al puntero cuando se llama a close_window_callback
  glfwSetWindowUserPointer(m_Window, this);

  glfwSetWindowCloseCallback(m_Window, close_window_callback);
  
  set_vsync(false);
}

WindowGLFW::~WindowGLFW()
{
  glfwDestroyWindow(m_Window);
  glfwTerminate();
}

void WindowGLFW::update()
{
  glfwPollEvents();
}

bool WindowGLFW::is_open() const{
  return !glfwWindowShouldClose(m_Window);
}

void WindowGLFW::set_vsync(bool vsync) {
  vsync ? glfwSwapInterval(1) : glfwSwapInterval(0);
  m_IsVSync = vsync;
}

bool WindowGLFW::is_vsync() const
{
  return m_IsVSync;
}

void* WindowGLFW::get_native_window()
{
  return m_Window;
}