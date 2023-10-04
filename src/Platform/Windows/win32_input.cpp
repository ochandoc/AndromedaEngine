#include "configuration.h"

#include "Common/KeyboardInput.h"
#include "Common/Window.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "GLFW/glfw3.h"


namespace And
{

  struct InputData
  {
    //std::unordered_map<> 
    HHOOK hook;

    // estado del key buffer al iniciar el frame (todo en false) que cambia cuando recibe input
    bool keysBufferBefore[123];

    // estado del key buffer cuando acaba el frame (en el siguiente frame es el estado que habia en el frame anterior)
    bool keysBufferAfter[123];
  };

  static InputData s_Data = {};

  void PressKey(KBDLLHOOKSTRUCT* key){

  }

  void PressedKey(unsigned int keyCode){
    int key = 0;

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

        // bloque ok
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

    s_Data.keysBufferBefore[key] = true;

  }

  static LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam){
    if (nCode >= 0) {
        // Manejar el evento de teclado
        if (wParam == WM_KEYDOWN) {
          KBDLLHOOKSTRUCT* pKeyData = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);

          // Imprimir el código de la tecla presionada
          //printf("Tecla pulsada-> %d\n", pKeyData->vkCode);

          PressedKey(pKeyData->vkCode);

        }
    }

    // Pasar el control al siguiente hook en la cadena
    return CallNextHookEx(NULL, nCode, wParam, lParam);
  }

  Input::Input(Window& w) : m_window(w){

  }

  void Input::init_input(){

    // Instalar el hook de teclado
    s_Data.hook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);

    for(int i = 0; i < kTotalKeys; i++){
      s_Data.keysBufferBefore[i] = false;
      s_Data.keysBufferAfter[i] = false;
    }

  }

  void Input::update_input(){
    
    // Swap input buffers
    for(unsigned int i = 0; i < kTotalKeys; i++){
      s_Data.keysBufferAfter[i] = s_Data.keysBufferBefore[i];
      s_Data.keysBufferBefore[i] = false;
    }
  
  }

  bool Input::IsKeyPressed(Key::KeyCode key){

    // Si en el frame de antes estaba presionada y en este tambien
    if(s_Data.keysBufferAfter[key]){
      return s_Data.keysBufferBefore[key];
    }

    return false;
  }

  bool Input::IsKeyDown(Key::KeyCode key){

    // Si antes no estaba presionada y ahora si
    if(s_Data.keysBufferAfter[key] == false){
      return s_Data.keysBufferBefore[key];
    }

    // Si en el frame de antes estaba siendo presionada, ya no puede ser true
    return false;
  }

  bool Input::IsKeyRelease(Key::KeyCode key){

    // Si en el frame de antes estaba presionada y en este frame ya no
    if(s_Data.keysBufferAfter[key] && !s_Data.keysBufferBefore[key]){
      return true;
    }

    return false;
  }

  bool Input::IsMouseButtonPressed(Key::MouseCode key){

    bool isPressed = false;

    switch (key){
      case Key::MouseLeft: isPressed = GetAsyncKeyState(VK_LBUTTON) & 0x8000; break;
      case Key::MouseRight: isPressed = GetAsyncKeyState(VK_RBUTTON) & 0x8000; break;
      default: isPressed = false; break;
    }

    return isPressed;
  }

  void Input::GetMousePosition(double *x, double *y){

    //glfwGetCursorPos((GLFWwindow*)s_Data.window->get_native_window(), x, y);

  }
  
  double Input::GetMouseX(){
    double x,y;

    //glfwGetCursorPos(window, &x, &y);
    return 0.0f;
  }

  double Input::GetMouseY(){
    double x,y;

    //glfwGetCursorPos(window, &x, &y);
    return 0.0f;
  }


  Input::~Input(){
    // Desinstalar el hook antes de salir
    UnhookWindowsHookEx(s_Data.hook);
  }
};