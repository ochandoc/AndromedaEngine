#include "configuration.h"

#include "Common/Window.h"
#include "Common/Renderer.h"
#include "Common/GraphicsContext.h"
#include "Common/KeyCodes.h"

#include "GLFW/glfw3.h"

#include "imgui_impl_glfw.h"

namespace And
{
  struct WindowData
  {
    Window* window_class;
    KeyboardState* state;
    GLFWwindow* handle;
    bool is_vsync;
    WindowCreationInfo creation_info;
    std::shared_ptr<GraphicsContext> context;
    std::unique_ptr<Renderer> renderer;
  };

  static void close_window_callback(GLFWwindow* window)
  {
    WindowData* glfw_window = reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
    if (glfw_window->window_class->m_OnWindowClose)
    {
      glfw_window->window_class->m_OnWindowClose();
    }
  }

    static void PressedKey(GLFWwindow* window, int keyCode, int scancode, int action, int mods){
    int key = 0;
      WindowData* window_data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));

    switch (keyCode) {
        case 32: key = Key::KeyCode::Space; break;
        case 39: key = Key::KeyCode::Apostrophe; break;
        case 44: key = Key::KeyCode::Comma; break;
        case 45: key = Key::KeyCode::Minus; break;
        case 46: key = Key::KeyCode::Period; break;
        case 47: key = Key::KeyCode::Slash; break;

        // case 48: key = Key::KeyCode::D0; break;
        // case 49: key = Key::KeyCode::D1; break;
        // case 50: key = Key::KeyCode::D2; break;
        // case 51: key = Key::KeyCode::D3; break;
        // case 52: key = Key::KeyCode::D4; break;
        // case 53: key = Key::KeyCode::D5; break;
        // case 54: key = Key::KeyCode::D6; break;
        // case 55: key = Key::KeyCode::D7; break;
        // case 56: key = Key::KeyCode::D8; break;
        // case 57: key = Key::KeyCode::D9; break;

        //case 59: key = Key::KeyCode::Semicolon; break;
        //case 61: key = Key::KeyCode::Equal; break;

        case 65: key = Key::KeyCode::A; break;
        case 66: key = Key::KeyCode::B; break;
        case 67: key = Key::KeyCode::C; break;
        case 68: key = Key::KeyCode::D; break;
        case 69: key = Key::KeyCode::E; break;
        case 70: key = Key::KeyCode::F; break;
        case 71: key = Key::KeyCode::G; break;
        case 72: key = Key::KeyCode::H; break;
        case 73: key = Key::KeyCode::I; break;
        case 74: key = Key::KeyCode::J; break;
        case 75: key = Key::KeyCode::K; break;
        case 76: key = Key::KeyCode::L; break;
        case 77: key = Key::KeyCode::M; break;
        case 78: key = Key::KeyCode::N; break;
        case 79: key = Key::KeyCode::O; break;
        case 80: key = Key::KeyCode::P; break;
        case 81: key = Key::KeyCode::Q; break;
        case 82: key = Key::KeyCode::R; break;
        case 83: key = Key::KeyCode::S; break;
        case 84: key = Key::KeyCode::T; break;
        case 85: key = Key::KeyCode::U; break;
        case 86: key = Key::KeyCode::V; break;
        case 87: key = Key::KeyCode::W; break;
        case 88: key = Key::KeyCode::X; break;
        case 89: key = Key::KeyCode::Y; break;
        case 90: key = Key::KeyCode::Z; break;

        case 91: key = Key::KeyCode::LeftBracket; break;
        case 92: key = Key::KeyCode::Backslash; break;
        case 93: key = Key::KeyCode::RightBracket; break;
        case 96: key = Key::KeyCode::GraveAccent; break;

        case 49: key = Key::KeyCode::World1; break;
        case 50: key = Key::KeyCode::World2; break;

        case 51: key = Key::KeyCode::Escape; break;
        case 52: key = Key::KeyCode::Enter; break;
        case 53: key = Key::KeyCode::Tab; break;
        case 54: key = Key::KeyCode::Backspace; break;
        case 55: key = Key::KeyCode::Insert; break;
        case 56: key = Key::KeyCode::Delete; break;
        case 57: key = Key::KeyCode::Right; break;
        case 58: key = Key::KeyCode::Left; break;
        case 59: key = Key::KeyCode::Down; break;
        case 60: key = Key::KeyCode::Up; break;
        case 61: key = Key::KeyCode::PageUp; break;
        case 62: key = Key::KeyCode::PageDown; break;
        case 63: key = Key::KeyCode::Home; break;
        case 64: key = Key::KeyCode::End; break;
        //case 65: key = Key::KeyCode::CapsLock; break;
        //case 66: key = Key::KeyCode::ScrollLock; break;
        //case 67: key = Key::KeyCode::NumLock; break;
        //case 68: key = Key::KeyCode::PrintScreen; break;
        //case 69: key = Key::KeyCode::Pause; break;


        case 112: key = Key::KeyCode::F1; break;
        case 113: key = Key::KeyCode::F2; break;
        case 114: key = Key::KeyCode::F3; break;
        case 115: key = Key::KeyCode::F4; break;
        case 116: key = Key::KeyCode::F5; break;
        case 117: key = Key::KeyCode::F6; break;
        case 118: key = Key::KeyCode::F7; break;
        case 119: key = Key::KeyCode::F8; break;
        case 120: key = Key::KeyCode::F9; break;
        case 121: key = Key::KeyCode::F10; break;
        case 122: key = Key::KeyCode::F11; break;
        case 123: key = Key::KeyCode::F12; break;
        //case 82: key = Key::KeyCode::F13; break;
        //case 83: key = Key::KeyCode::F14; break;
        //case 84: key = Key::KeyCode::F15; break;
        //case 85: key = Key::KeyCode::F16; break;
        //case 86: key = Key::KeyCode::F17; break;
        //case 87: key = Key::KeyCode::F18; break;
        //case 88: key = Key::KeyCode::F19; break;
        //case 89: key = Key::KeyCode::F20; break;
        //case 90: key = Key::KeyCode::F21; break;
        //case 91: key = Key::KeyCode::F22; break;
        //case 92: key = Key::KeyCode::F23; break;
        //case 93: key = Key::KeyCode::F24; break;
        //case 94: key = Key::KeyCode::F25; break;

        //case 95: key = Key::KeyCode::KP0; break;
        //case 96: key = Key::KeyCode::KP1; break;
        //case 97: key = Key::KeyCode::KP2; break;
        //case 98: key = Key::KeyCode::KP3; break;
        //case 99: key = Key::KeyCode::KP4; break;
        //case 100: key = Key::KeyCode::KP5; break;
        //case 101: key = Key::KeyCode::KP6; break;
        //case 102: key = Key::KeyCode::KP7; break;
        //case 103: key = Key::KeyCode::KP8; break;
        //case 104: key = Key::KeyCode::KP9; break;
        //case 105: key = Key::KeyCode::KPDecimal; break;
        //case 106: key = Key::KeyCode::KPDivide; break;
        //case 107: key = Key::KeyCode::KPMultiply; break;
        //case 108: key = Key::KeyCode::KPSubtract; break;
        //case 109: key = Key::KeyCode::KPAdd; break;
        //case 110: key = Key::KeyCode::KPEnter; break;
        //case 111: key = Key::KeyCode::KPEqual; break;

        //case 112: key = Key::KeyCode::LeftShift; break;
        //case 113: key = Key::KeyCode::LeftControl; break;
        //case 114: key = Key::KeyCode::LeftAlt; break;
        //case 115: key = Key::KeyCode::LeftSuper; break;
        //case 116: key = Key::KeyCode::RightShift; break;
        //case 117: key = Key::KeyCode::RightControl; break;
        //case 118: key = Key::KeyCode::RightAlt; break;
        //case 119: key = Key::KeyCode::RightSuper; break;
        //case 120: key = Key::KeyCode::Menu; break;
        default: break;
    }

    window_data->state->keysBufferBefore[key] = true; 

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


    m_Data->window_class = this;
    m_Data->state = &m_KeyBoard;
    // pointer to our window class, glfw se lo guarda y luego en el evento de close window podemos acceder al puntero cuando se llama a close_window_callback
    glfwSetWindowUserPointer(window, m_Data.get());

    glfwSetWindowCloseCallback(window, close_window_callback);
    glfwSetKeyCallback(m_Data->handle, PressedKey);

    for(int i = 0; i < m_KeyBoard.buffer_size; i++){
      m_KeyBoard.keysBufferBefore[i] = false;
      m_KeyBoard.keysBufferAfter[i] = false;
    }
  
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

    // Swap input buffers
    for(unsigned int i = 0; i < m_KeyBoard.buffer_size; i++){
      m_KeyBoard.keysBufferAfter[i] = m_KeyBoard.keysBufferBefore[i];
      m_KeyBoard.keysBufferBefore[i] = false;
    }

  }

}

