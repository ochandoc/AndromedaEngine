#include "Core/Window.h"

#include <assert.h>


#include "Backends/OpenGL/OpenGLRenderer.h"
#include "Backends/OpenGL/OpenGLContext.h"

#include "GLFW/glfw3.h"

#include "imgui_impl_glfw.h"

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

std::unique_ptr<Window::ImGuiImpl> Window::make_imgui_impl()
{
  return std::unique_ptr<Window::ImGuiImpl>(new Window::ImGuiImpl(*this));
}

Window::ImGuiImpl::ImGuiImpl(Window& window) : m_Window(window)
{
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  ImGuiIO& io = ImGui::GetIO();

  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
  io.IniFilename = "./Config/ImGui.ini";

  ImGuiStyle& style = ImGui::GetStyle();
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
  {
    style.WindowRounding = 0.0f;
    style.Colors[ImGuiCol_WindowBg].w = 1.0f;
  }
  
  switch (window.m_CreationInfo.api)
  {
  case GraphicsAPI_OpenGL:
    ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)m_Window.get_native_window(), true);
    break;
  }
}

Window::ImGuiImpl::~ImGuiImpl()
{
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void Window::ImGuiImpl::new_frame()
{
  ImGui_ImplGlfw_NewFrame();
}

void Window::ImGuiImpl::end_frame()
{
  static GLFWwindow* window = (GLFWwindow*)m_Window.get_native_window();

  ImGuiIO& io = ImGui::GetIO();

  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
  {
    GLFWwindow* backup_current_context = glfwGetCurrentContext();
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    glfwMakeContextCurrent(backup_current_context);
  }

  glfwSwapBuffers((GLFWwindow*)window);
  glfwPollEvents();
}

}

