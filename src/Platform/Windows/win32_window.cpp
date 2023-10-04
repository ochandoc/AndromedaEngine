#include "configuration.h"

#include "Common/Window.h"

#include "Common/Renderer.h"
#include "Common/GraphicsContext.h"

#include "GLFW/glfw3.h"

#include "imgui_impl_glfw.h"

namespace And
{

  struct WindowData
  {
    GLFWwindow* handle;
    bool is_vsync;
    WindowCreationInfo creation_info;
    std::shared_ptr<GraphicsContext> context;
    std::unique_ptr<Renderer> renderer;
  };

  static void close_window_callback(GLFWwindow* window)
  {
    Window* glfw_window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
    if (glfw_window->m_OnWindowClose)
    {
      glfw_window->m_OnWindowClose();
    }
  }

  Window::Window(const WindowCreationInfo& info) : m_Data(new WindowData)
  {
    m_Data->creation_info = info;

    glfwInit();

#ifdef AND_OPENGL
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // Version principal de OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);  // Version menor de OpenGL
#   ifdef DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE); // Activar debug
#   endif
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Perfil de OpenGL
#endif

    GLFWwindow* window = glfwCreateWindow(info.width, info.height, info.title.c_str(), NULL, NULL);
    m_Data->handle = window;

    // pointer to our window class, glfw se lo guarda y luego en el evento de close window podemos acceder al puntero cuando se llama a close_window_callback
    glfwSetWindowUserPointer(window, this);

    glfwSetWindowCloseCallback(window, close_window_callback);
  
    m_Data->context = std::move(std::shared_ptr<GraphicsContext>(new GraphicsContext(*this)));
    
    set_vsync(false);
  }

  Window::~Window()
  {
    glfwDestroyWindow(m_Data->handle);
    glfwTerminate();
  }

  bool Window::is_open() const{
    return !glfwWindowShouldClose(m_Data->handle);
  }

  void Window::set_vsync(bool vsync) {
    vsync ? glfwSwapInterval(1) : glfwSwapInterval(0);
    m_Data->is_vsync = vsync;
  }

  bool Window::is_vsync() const
  {
    return m_Data->is_vsync;
  }

  void* Window::get_native_window()
  {
    return m_Data->handle;
  }

  Renderer& Window::create_renderer()
  {
    m_Data->renderer = std::make_unique<Renderer>(*this);
    return *(m_Data->renderer);
  }

  std::shared_ptr<GraphicsContext> Window::get_context() const
  {
    return m_Data->context;
  }

  void Window::imgui_start()
  {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    io.IniFilename = "./ImGui.ini";

    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
      style.WindowRounding = 0.0f;
      style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    ImGui_ImplGlfw_InitForOpenGL(m_Data->handle, true);
  }

  void Window::imgui_end()
  {
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
  }

  void Window::new_frame()
  {
    glfwPollEvents();
    ImGui_ImplGlfw_NewFrame();
  }

  void Window::end_frame()
  {
    ImGuiIO& io = ImGui::GetIO();

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
      GLFWwindow* backup_current_context = glfwGetCurrentContext();
      ImGui::UpdatePlatformWindows();
      ImGui::RenderPlatformWindowsDefault();
      glfwMakeContextCurrent(backup_current_context);
    }

    glfwSwapBuffers(m_Data->handle);
  }

}

