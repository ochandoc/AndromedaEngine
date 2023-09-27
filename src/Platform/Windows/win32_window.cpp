#include "Core/Window.h"

#include <assert.h>


#include "Backends/OpenGL/OpenGLRenderer.h"
#include "Backends/OpenGL/OpenGLContext.h"

#include "GLFW/glfw3.h"

namespace And
{

static void close_window_callback(GLFWwindow* window)
{
  Window* glfw_window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
  if (glfw_window->m_OnWindowClose)
  {
    glfw_window->m_OnWindowClose();
  }
}

Window::Window(const WindowCreationInfo& info){
  
  m_CreationInfo = info;

  glfwInit();

  switch (info.api)
  {
  case GraphicsAPI_OpenGL:  
    // Configurar GLFW para crear un contexto OpenGL
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // Version principal de OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);  // Version menor de OpenGL
#   ifdef DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE); // Activar debug
#   endif
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Perfil de OpenGL
    break;
  default:
    assert(false && "Api not suported!");
    break;
  }


  GLFWwindow* window = glfwCreateWindow(info.width, info.height, info.title.c_str(), NULL, NULL);

  // pointer to our window class, glfw se lo guarda y luego en el evento de close window podemos acceder al puntero cuando se llama a close_window_callback
  glfwSetWindowUserPointer(window, this);

  glfwSetWindowCloseCallback(window, close_window_callback);
  
  switch (m_CreationInfo.api)
  {
  case GraphicsAPI_OpenGL:
    m_Context = std::make_shared<OpenGLContext>(window);
    break;
  }

  set_vsync(false);
  m_Handle = (void*)window;
}

Window::~Window()
{
  glfwDestroyWindow((GLFWwindow*)m_Handle);
  glfwTerminate();
}

void Window::update()
{
  glfwSwapBuffers((GLFWwindow*)m_Handle);
  glfwPollEvents();
}

bool Window::is_open() const{
  return !glfwWindowShouldClose((GLFWwindow*)m_Handle);
}

void Window::set_vsync(bool vsync) {
  vsync ? glfwSwapInterval(1) : glfwSwapInterval(0);
  m_IsVSync = vsync;
}

bool Window::is_vsync() const
{
  return m_IsVSync;
}

void* Window::get_native_window() const
{
  return m_Handle;
}

Renderer& Window::create_renderer()
{
  switch (m_CreationInfo.api)
  {
  case GraphicsAPI_OpenGL:
    m_Renderer = std::make_unique<OpenGLRenderer>(*this);
    break;
  }
  return *m_Renderer;
}

}

