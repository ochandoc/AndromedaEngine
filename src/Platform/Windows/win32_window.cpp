#include "Common/Window.h"

#include "Common/Engine.h"
#include "Common/Renderer.h"
#include "Common/GraphicsContext.h"

#include "GLFW/glfw3.h"

#include "imgui_impl_glfw.h"

namespace And
{

  /*struct WindowData
  {
    GLFWwindow* handle;
    bool is_vsync;
    WindowCreationInfo creation_info;
    std::shared_ptr<GraphicsContext> context;
    std::unique_ptr<Renderer> renderer;
  };*/

  static void close_window_callback(GLFWwindow* window)
  {
    PLATFORM_WINDOW_DATA* data = CAST_PTR(PLATFORM_WINDOW_DATA, glfwGetWindowUserPointer(window));
    Window* w = CAST_PTR(Window, data->user_data);
    data->is_open = false;
    if (w->m_OnWindowClose)
    {
      w->m_OnWindowClose();
    }

  }

  Window::Window() {}

  Window::~Window()
  {
    glfwDestroyWindow(m_Data.handle);
  }

  std::shared_ptr<Window> Window::make(Engine& e, uint32 w, uint32 h, const char* title)
  {
    if (!e.is_initialized()) return std::shared_ptr<Window>();

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
#ifdef AND_OPENGL
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // Version principal de OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);  // Version menor de OpenGL
#   ifdef DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE); // Activar debug
#   endif
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Perfil de OpenGL
#endif

    GLFWwindow* handle = glfwCreateWindow(w, h, title, NULL, NULL);

    if (!handle) return std::shared_ptr<Window>();

    std::shared_ptr<Window> window(new Window);

    window->m_Data.handle = handle;
    window->m_Data.width = w;
    window->m_Data.height = h;
    window->m_Data.user_data = window.get();
    window->m_Data.is_open = true;

    glfwSetWindowUserPointer(window->m_Data.handle, &window->m_Data);
    glfwSetWindowCloseCallback(window->m_Data.handle, close_window_callback);

    window->m_Context = std::move(std::shared_ptr<GraphicsContext>(new GraphicsContext(*window)));
    window->set_vsync(false);

    return window;
  }

  bool Window::is_open() const{
    return m_Data.is_open;
  }

  void Window::set_vsync(bool vsync) {
    vsync ? glfwSwapInterval(1) : glfwSwapInterval(0);
    m_Data.is_vsync = vsync;
  }

  bool Window::is_vsync() const
  {
    return m_Data.is_vsync;
  }

  void* Window::get_native_window()
  {
    return m_Data.handle;
  }

  void Window::update()
  {
    glfwPollEvents();
  }

  std::shared_ptr<GraphicsContext> Window::get_context() const
  {
    return m_Context;
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

    ImGui_ImplGlfw_InitForOpenGL(m_Data.handle, true);
  }

  void Window::imgui_end()
  {
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
  }

  void Window::new_frame()
  {
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

    glfwSwapBuffers(m_Data.handle);
  }

}

