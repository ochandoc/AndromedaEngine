#include "WindowGLFW.h"

#include <assert.h>

#include "Backends/OpenGL/OpenGLRenderer.h"
#include "Backends/OpenGL/OpenGLContext.h"

namespace And
{

static void close_window_callback(GLFWwindow* window)
{
  WindowGLFW* glfw_window = reinterpret_cast<WindowGLFW*>(glfwGetWindowUserPointer(window));
  if (glfw_window->m_OnWindowClose)
  {
    glfw_window->m_OnWindowClose();
  }
}

WindowGLFW::WindowGLFW(const WindowCreationInfo& info){
  
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


  m_Window = glfwCreateWindow(info.width, info.height, info.title.c_str(), NULL, NULL);

  // pointer to our window class, glfw se lo guarda y luego en el evento de close window podemos acceder al puntero cuando se llama a close_window_callback
  glfwSetWindowUserPointer(m_Window, this);

  glfwSetWindowCloseCallback(m_Window, close_window_callback);
  
  switch (m_CreationInfo.api)
  {
  case GraphicsAPI_OpenGL:
    m_Context = std::make_shared<OpenGLContext>(m_Window);
    break;
  }

  set_vsync(false);
}

WindowGLFW::~WindowGLFW()
{
  glfwDestroyWindow(m_Window);
  glfwTerminate();
}

void WindowGLFW::update()
{
  glfwSwapBuffers(m_Window);
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

Renderer& WindowGLFW::create_renderer()
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

