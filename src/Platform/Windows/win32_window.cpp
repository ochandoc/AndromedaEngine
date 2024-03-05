#include "Andromeda/HAL/Window.h"

#include "Andromeda/Engine.h"
#include "Andromeda/Graphics/Renderer.h"
#include "Andromeda/Graphics/GraphicsContext.h"
#include "Andromeda/HAL/KeyCodes.h"

#include "GLFW/glfw3.h"

#include "imgui_impl_glfw.h"
#include "Andromeda/UI/Plot/implot.h"
#include "Andromeda/Graphics/RenderTarget.h"

namespace And
{

  struct WindowData
  {
    Window* class_instance;
    GLFWwindow* handle;
    uint32 width, height;
    bool is_open;
    bool is_vsync;
    KeyboardState* keyboard;
    std::shared_ptr<GraphicsContext> m_Context;
    float LastTime;
    float DeltaTime;
  };

  static void close_window_callback(GLFWwindow* window)
  {
    WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
    Window* w = data->class_instance;
    data->is_open = false;
    if (w->OnWindowClose.IsBounded())
      w->OnWindowClose.Broadcast();
  }

  static void resize_window_callback(GLFWwindow* window, int width, int height)
  {
    WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
    Window* w = data->class_instance;
    data->width = width;
    data->height = height;
    if (w->OnWindowResize.IsBounded())
      w->OnWindowResize.Broadcast(width, height);
  }

  static void PressedKey(GLFWwindow* window, int keyCode, int scancode, int action, int mods){
    int key = 0;
      WindowData* window_data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));

    switch (keyCode) {
        case 32: key = (int)KeyCode::Space; break;
        case 39: key = (int)KeyCode::Apostrophe; break;
        case 44: key = (int)KeyCode::Comma; break;
        case 45: key = (int)KeyCode::Minus; break;
        case 46: key = (int)KeyCode::Period; break;
        case 47: key = (int)KeyCode::Slash; break;

        // case 48: key = (int)KeyCode::D0; break;
        // case 49: key = (int)KeyCode::D1; break;
        // case 50: key = (int)KeyCode::D2; break;
        // case 51: key = (int)KeyCode::D3; break;
        // case 52: key = (int)KeyCode::D4; break;
        // case 53: key = (int)KeyCode::D5; break;
        // case 54: key = (int)KeyCode::D6; break;
        // case 55: key = (int)KeyCode::D7; break;
        // case 56: key = (int)KeyCode::D8; break;
        // case 57: key = (int)KeyCode::D9; break;

        //case 59: key = (int)KeyCode::Semicolon; break;
        //case 61: key = (int)KeyCode::Equal; break;

        case 65: key = (int)KeyCode::A; break;
        case 66: key = (int)KeyCode::B; break;
        case 67: key = (int)KeyCode::C; break;
        case 68: key = (int)KeyCode::D; break;
        case 69: key = (int)KeyCode::E; break;
        case 70: key = (int)KeyCode::F; break;
        case 71: key = (int)KeyCode::G; break;
        case 72: key = (int)KeyCode::H; break;
        case 73: key = (int)KeyCode::I; break;
        case 74: key = (int)KeyCode::J; break;
        case 75: key = (int)KeyCode::K; break;
        case 76: key = (int)KeyCode::L; break;
        case 77: key = (int)KeyCode::M; break;
        case 78: key = (int)KeyCode::N; break;
        case 79: key = (int)KeyCode::O; break;
        case 80: key = (int)KeyCode::P; break;
        case 81: key = (int)KeyCode::Q; break;
        case 82: key = (int)KeyCode::R; break;
        case 83: key = (int)KeyCode::S; break;
        case 84: key = (int)KeyCode::T; break;
        case 85: key = (int)KeyCode::U; break;
        case 86: key = (int)KeyCode::V; break;
        case 87: key = (int)KeyCode::W; break;
        case 88: key = (int)KeyCode::X; break;
        case 89: key = (int)KeyCode::Y; break;
        case 90: key = (int)KeyCode::Z; break;

        case 91: key = (int)KeyCode::LeftBracket; break;
        case 92: key = (int)KeyCode::Backslash; break;
        case 93: key = (int)KeyCode::RightBracket; break;
        case 96: key = (int)KeyCode::GraveAccent; break;

        case 49: key = (int)KeyCode::World1; break;
        case 50: key = (int)KeyCode::World2; break;

        case 51: key = (int)KeyCode::Escape; break;
        case 52: key = (int)KeyCode::Enter; break;
        case 53: key = (int)KeyCode::Tab; break;
        case 54: key = (int)KeyCode::Backspace; break;
        case 55: key = (int)KeyCode::Insert; break;
        case 56: key = (int)KeyCode::Delete; break;
        case 57: key = (int)KeyCode::Right; break;
        case 58: key = (int)KeyCode::Left; break;
        case 59: key = (int)KeyCode::Down; break;
        case 60: key = (int)KeyCode::Up; break;
        case 61: key = (int)KeyCode::PageUp; break;
        case 62: key = (int)KeyCode::PageDown; break;
        case 63: key = (int)KeyCode::Home; break;
        case 64: key = (int)KeyCode::End; break;
        //case 65: key = (int)KeyCode::CapsLock; break;
        //case 66: key = (int)KeyCode::ScrollLock; break;
        //case 67: key = (int)KeyCode::NumLock; break;
        //case 68: key = (int)KeyCode::PrintScreen; break;
        //case 69: key = (int)KeyCode::Pause; break;


        case 112: key = (int)KeyCode::F1; break;
        case 113: key = (int)KeyCode::F2; break;
        case 114: key = (int)KeyCode::F3; break;
        case 115: key = (int)KeyCode::F4; break;
        case 116: key = (int)KeyCode::F5; break;
        case 117: key = (int)KeyCode::F6; break;
        case 118: key = (int)KeyCode::F7; break;
        case 119: key = (int)KeyCode::F8; break;
        case 120: key = (int)KeyCode::F9; break;
        case 121: key = (int)KeyCode::F10; break;
        case 122: key = (int)KeyCode::F11; break;
        case 123: key = (int)KeyCode::F12; break;
        //case 82: key = (int)KeyCode::F13; break;
        //case 83: key = (int)KeyCode::F14; break;
        //case 84: key = (int)KeyCode::F15; break;
        //case 85: key = (int)KeyCode::F16; break;
        //case 86: key = (int)KeyCode::F17; break;
        //case 87: key = (int)KeyCode::F18; break;
        //case 88: key = (int)KeyCode::F19; break;
        //case 89: key = (int)KeyCode::F20; break;
        //case 90: key = (int)KeyCode::F21; break;
        //case 91: key = (int)KeyCode::F22; break;
        //case 92: key = (int)KeyCode::F23; break;
        //case 93: key = (int)KeyCode::F24; break;
        //case 94: key = (int)KeyCode::F25; break;

        //case 95: key = (int)KeyCode::KP0; break;
        //case 96: key = (int)KeyCode::KP1; break;
        //case 97: key = (int)KeyCode::KP2; break;
        //case 98: key = (int)KeyCode::KP3; break;
        //case 99: key = (int)KeyCode::KP4; break;
        //case 100: key = (int)KeyCode::KP5; break;
        //case 101: key = (int)KeyCode::KP6; break;
        //case 102: key = (int)KeyCode::KP7; break;
        //case 103: key = (int)KeyCode::KP8; break;
        //case 104: key = (int)KeyCode::KP9; break;
        //case 105: key = (int)KeyCode::KPDecimal; break;
        //case 106: key = (int)KeyCode::KPDivide; break;
        //case 107: key = (int)KeyCode::KPMultiply; break;
        //case 108: key = (int)KeyCode::KPSubtract; break;
        //case 109: key = (int)KeyCode::KPAdd; break;
        //case 110: key = (int)KeyCode::KPEnter; break;
        //case 111: key = (int)KeyCode::KPEqual; break;

        //case 112: key = (int)KeyCode::LeftShift; break;
        //case 113: key = (int)KeyCode::LeftControl; break;
        //case 114: key = (int)KeyCode::LeftAlt; break;
        //case 115: key = (int)KeyCode::LeftSuper; break;
        //case 116: key = (int)KeyCode::RightShift; break;
        //case 117: key = (int)KeyCode::RightControl; break;
        //case 118: key = (int)KeyCode::RightAlt; break;
        //case 119: key = (int)KeyCode::RightSuper; break;
        //case 120: key = (int)KeyCode::Menu; break;
        default: break;
    }

    //printf("Key: %d\n", key);

  }

  Window::Window() : m_Data(new WindowData) 
  {
    m_Data->class_instance = this;
    m_Data->LastTime = 0.0f;
  }

  Window::~Window()
  {
    glfwDestroyWindow(m_Data->handle);
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

    window->m_Data->handle = handle;
    window->m_Data->width = w;
    window->m_Data->height = h;
    window->m_Data->is_open = true;

    glfwSetWindowUserPointer(window->m_Data->handle, window->m_Data.get());
    glfwSetWindowCloseCallback(window->m_Data->handle, close_window_callback);
    glfwSetWindowSizeCallback(window->m_Data->handle, resize_window_callback);
    glfwSetKeyCallback(window->m_Data->handle, PressedKey);

    window->m_Data->m_Context = std::move(std::shared_ptr<GraphicsContext>(new GraphicsContext(*window)));
    window->set_vsync(false);

    return window;
  }

  bool Window::is_open() const{
    return m_Data->is_open;
  }

  void Window::set_vsync(bool vsync) {
    vsync ? glfwSwapInterval(1) : glfwSwapInterval(0);
    m_Data->is_vsync = vsync;
  }

  bool Window::is_vsync() const
  {
    return m_Data->is_vsync;
  }

  void Window::set_size(uint32 width, uint32 height)
  {
    glfwSetWindowSize(m_Data->handle, (int)width, (int)height);
  }

  uint32 Window::get_width() const
  {
    return m_Data->width;
  }

  uint32 Window::get_height() const
  {
    return m_Data->height;
  }

  void* Window::get_native_window()
  {
    return m_Data->handle;
  }

  void Window::update()
  {
    
    /*for (KeyState& key : window->m_KeyBoard.keys)
    {
      key = KeyState::Default;
    }*/
    
    glfwPollEvents();
    float CurrentTime = static_cast<float>(glfwGetTime());
    m_Data->DeltaTime = CurrentTime - m_Data->LastTime;
    m_Data->LastTime = CurrentTime;
  }

  void Window::swap_buffers()
  {
    glfwSwapBuffers(m_Data->handle);
  }

  float Window::get_delta_time() const
  {
      return m_Data->DeltaTime;
  }

  std::shared_ptr<GraphicsContext> Window::get_context() const
  {
    return m_Data->m_Context;
  }

  std::function<void(WorkerThreadData& Data)> Window::get_worker_function()
  {
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
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
    GLFWwindow* window = glfwCreateWindow(100, 100, "OpenGL", nullptr, m_Data->handle);
    std::function<void(WorkerThreadData& Data)> function = [this, window](WorkerThreadData& Data)
      {
        glfwMakeContextCurrent(window);
        while (true)
        {
          Task task(NoInit);
          {
            std::unique_lock<std::mutex> lock(Data.QueueMutex);
            Data.ConditionVariable.wait(lock, [&Data]() { return !Data.TasksQueue.empty() || Data.Stop; });
            if (Data.TasksQueue.empty() && Data.Stop) { glfwDestroyWindow(window); return; }
            task = std::move(Data.TasksQueue.front());
            Data.TasksQueue.pop();
          }
          auto start = std::chrono::system_clock::now();
          task();
          auto endt = std::chrono::system_clock::now();

          Data.TaskSystemOwner->MarkTaskAsResolved(task, std::chrono::duration<float>(endt - start).count());
        }
      };
    return function;
  }

  void Window::imgui_start()
  {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();

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
    ImPlot::DestroyContext();
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
  }

}

