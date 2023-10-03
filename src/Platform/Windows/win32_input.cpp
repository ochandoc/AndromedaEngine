#include "configuration.h"

#include "Common/KeyboardInput.h"
#include "Common/Window.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "GLFW/glfw3.h"


namespace And
{
  struct InputData{

    Window* window = nullptr;
    //std::unordered_map<> 
  };

  static InputData s_Data;

  bool Input::IsKeyPressed(Key::KeyCode key){

    bool isPressed = false;

    switch (key){
      case Key::Space: isPressed = GetAsyncKeyState(VK_SPACE) & 0x8000; break;
      case Key::Apostrophe: isPressed = GetAsyncKeyState(VK_OEM_7) & 0x8000; break;
      case Key::Comma: isPressed = GetAsyncKeyState(VK_OEM_COMMA) & 0x8000; break;
      case Key::Minus: isPressed = GetAsyncKeyState(VK_OEM_MINUS) & 0x8000; break;
      case Key::Period: isPressed = GetAsyncKeyState(VK_OEM_PERIOD) & 0x8000; break;
      case Key::Slash: isPressed = GetAsyncKeyState(VK_OEM_2) & 0x8000; break;
      case Key::D0: isPressed = GetAsyncKeyState('0') & 0x8000; break;
      case Key::D1: isPressed = GetAsyncKeyState('1') & 0x8000; break;
      case Key::D2: isPressed = GetAsyncKeyState('2') & 0x8000; break;
      case Key::D3: isPressed = GetAsyncKeyState('3') & 0x8000; break;
      case Key::D4: isPressed = GetAsyncKeyState('4') & 0x8000; break;
      case Key::D5: isPressed = GetAsyncKeyState('5') & 0x8000; break;
      case Key::D6: isPressed = GetAsyncKeyState('6') & 0x8000; break;
      case Key::D7: isPressed = GetAsyncKeyState('7') & 0x8000; break;
      case Key::D8: isPressed = GetAsyncKeyState('8') & 0x8000; break;
      case Key::D9: isPressed = GetAsyncKeyState('9') & 0x8000; break;
      case Key::Semicolon: isPressed = GetAsyncKeyState(VK_OEM_1) & 0x8000; break;
      case Key::Equal: isPressed = GetAsyncKeyState(VK_OEM_PLUS) & 0x8000; break;
      case Key::A: isPressed = GetAsyncKeyState('A') & 0x8000; break;
      case Key::B: isPressed = GetAsyncKeyState('B') & 0x8000; break;
      case Key::C: isPressed = GetAsyncKeyState('C') & 0x8000; break;
      case Key::D: isPressed = GetAsyncKeyState('D') & 0x8000; break;
      case Key::E: isPressed = GetAsyncKeyState('E') & 0x8000; break;
      case Key::F: isPressed = GetAsyncKeyState('F') & 0x8000; break;
      case Key::G: isPressed = GetAsyncKeyState('G') & 0x8000; break;
      case Key::H: isPressed = GetAsyncKeyState('H') & 0x8000; break;
      case Key::I: isPressed = GetAsyncKeyState('I') & 0x8000; break;
      case Key::J: isPressed = GetAsyncKeyState('J') & 0x8000; break;
      case Key::K: isPressed = GetAsyncKeyState('K') & 0x8000; break;
      case Key::L: isPressed = GetAsyncKeyState('L') & 0x8000; break;
      case Key::M: isPressed = GetAsyncKeyState('M') & 0x8000; break;
      case Key::N: isPressed = GetAsyncKeyState('N') & 0x8000; break;
      case Key::O: isPressed = GetAsyncKeyState('O') & 0x8000; break;
      case Key::P: isPressed = GetAsyncKeyState('P') & 0x8000; break;
      case Key::Q: isPressed = GetAsyncKeyState('Q') & 0x8000; break;
      case Key::R: isPressed = GetAsyncKeyState('R') & 0x8000; break;
      case Key::S: isPressed = GetAsyncKeyState('S') & 0x8000; break;
      case Key::T: isPressed = GetAsyncKeyState('T') & 0x8000; break;
      case Key::U: isPressed = GetAsyncKeyState('U') & 0x8000; break;
      case Key::V: isPressed = GetAsyncKeyState('V') & 0x8000; break;
      case Key::W: isPressed = GetAsyncKeyState('W') & 0x8000; break;
      case Key::X: isPressed = GetAsyncKeyState('X') & 0x8000; break;
      case Key::Y: isPressed = GetAsyncKeyState('Y') & 0x8000; break;
      case Key::Z: isPressed = GetAsyncKeyState('Z') & 0x8000; break;
      case Key::LeftBracket: isPressed = GetAsyncKeyState(VK_OEM_4) & 0x8000; break;
      case Key::Backslash: isPressed = GetAsyncKeyState(VK_OEM_5) & 0x8000; break;
      case Key::RightBracket: isPressed = GetAsyncKeyState(VK_OEM_6) & 0x8000; break;
      case Key::GraveAccent: isPressed = GetAsyncKeyState(VK_OEM_3) & 0x8000; break;
      case Key::World1: isPressed = GetAsyncKeyState(VK_OEM_102) & 0x8000; break;
      case Key::World2: isPressed = GetAsyncKeyState(VK_OEM_8) & 0x8000; break;
      case Key::Escape: isPressed = GetAsyncKeyState(VK_ESCAPE) & 0x8000; break;
      case Key::Enter: isPressed = GetAsyncKeyState(VK_RETURN) & 0x8000; break;
      case Key::Tab: isPressed = GetAsyncKeyState(VK_TAB) & 0x8000; break;
      case Key::Backspace: isPressed = GetAsyncKeyState(VK_BACK) & 0x8000; break;
      case Key::Insert: isPressed = GetAsyncKeyState(VK_INSERT) & 0x8000; break;
      case Key::Delete: isPressed = GetAsyncKeyState(VK_DELETE) & 0x8000; break;
      case Key::Right: isPressed = GetAsyncKeyState(VK_RIGHT) & 0x8000; break;
      case Key::Left: isPressed = GetAsyncKeyState(VK_LEFT) & 0x8000; break;
      case Key::Down: isPressed = GetAsyncKeyState(VK_DOWN) & 0x8000; break;
      case Key::Up: isPressed = GetAsyncKeyState(VK_UP) & 0x8000; break;
      case Key::PageUp: isPressed = GetAsyncKeyState(VK_PRIOR) & 0x8000; break;
      case Key::PageDown: isPressed = GetAsyncKeyState(VK_NEXT) & 0x8000; break;
      case Key::Home: isPressed = GetAsyncKeyState(VK_HOME) & 0x8000; break;
      case Key::End: isPressed = GetAsyncKeyState(VK_END) & 0x8000; break;
      case Key::CapsLock: isPressed = GetAsyncKeyState(VK_CAPITAL) & 0x8000; break;
      case Key::ScrollLock: isPressed = GetAsyncKeyState(VK_SCROLL) & 0x8000; break;
      case Key::NumLock: isPressed = GetAsyncKeyState(VK_NUMLOCK) & 0x8000; break;
      case Key::PrintScreen: isPressed = GetAsyncKeyState(VK_SNAPSHOT) & 0x8000; break;
      case Key::Pause: isPressed = GetAsyncKeyState(VK_PAUSE) & 0x8000; break;
      case Key::F1: isPressed = GetAsyncKeyState(VK_F1) & 0x8000; break;
      case Key::F2: isPressed = GetAsyncKeyState(VK_F2) & 0x8000; break;
      case Key::F3: isPressed = GetAsyncKeyState(VK_F3) & 0x8000; break;
      case Key::F4: isPressed = GetAsyncKeyState(VK_F4) & 0x8000; break;
      case Key::F5: isPressed = GetAsyncKeyState(VK_F5) & 0x8000; break;
      case Key::F6: isPressed = GetAsyncKeyState(VK_F6) & 0x8000; break;
      case Key::F7: isPressed = GetAsyncKeyState(VK_F7) & 0x8000; break;
      case Key::F8: isPressed = GetAsyncKeyState(VK_F8) & 0x8000; break;
      case Key::F9: isPressed = GetAsyncKeyState(VK_F9) & 0x8000; break;
      case Key::F10: isPressed = GetAsyncKeyState(VK_F10) & 0x8000; break;
      case Key::F11: isPressed = GetAsyncKeyState(VK_F11) & 0x8000; break;
      case Key::F12: isPressed = GetAsyncKeyState(VK_F12) & 0x8000; break;
      case Key::F13: isPressed = GetAsyncKeyState(VK_F13) & 0x8000; break;
      case Key::F14: isPressed = GetAsyncKeyState(VK_F14) & 0x8000; break;
      case Key::F15: isPressed = GetAsyncKeyState(VK_F15) & 0x8000; break;
      case Key::F16: isPressed = GetAsyncKeyState(VK_F16) & 0x8000; break;
      case Key::F17: isPressed = GetAsyncKeyState(VK_F17) & 0x8000; break;
      case Key::F18: isPressed = GetAsyncKeyState(VK_F18) & 0x8000; break;
      case Key::F19: isPressed = GetAsyncKeyState(VK_F19) & 0x8000; break;
      case Key::F20: isPressed = GetAsyncKeyState(VK_F20) & 0x8000; break;
      case Key::F21: isPressed = GetAsyncKeyState(VK_F21) & 0x8000; break;
      case Key::F22: isPressed = GetAsyncKeyState(VK_F22) & 0x8000; break;
      case Key::F23: isPressed = GetAsyncKeyState(VK_F23) & 0x8000; break;
      case Key::F24: isPressed = GetAsyncKeyState(VK_F24) & 0x8000; break;
      //case Key::F25: isPressed = GetAsyncKeyState(VK_F25) & 0x8000; break;
      case Key::KP0: isPressed = GetAsyncKeyState(VK_NUMPAD0) & 0x8000; break;
      case Key::KP1: isPressed = GetAsyncKeyState(VK_NUMPAD1) & 0x8000; break;
      case Key::KP2: isPressed = GetAsyncKeyState(VK_NUMPAD2) & 0x8000; break;
      case Key::KP3: isPressed = GetAsyncKeyState(VK_NUMPAD3) & 0x8000; break;
      case Key::KP4: isPressed = GetAsyncKeyState(VK_NUMPAD4) & 0x8000; break;
      case Key::KP5: isPressed = GetAsyncKeyState(VK_NUMPAD5) & 0x8000; break;
      case Key::KP6: isPressed = GetAsyncKeyState(VK_NUMPAD6) & 0x8000; break;
      case Key::KP7: isPressed = GetAsyncKeyState(VK_NUMPAD7) & 0x8000; break;
      case Key::KP8: isPressed = GetAsyncKeyState(VK_NUMPAD8) & 0x8000; break;
      case Key::KP9: isPressed = GetAsyncKeyState(VK_NUMPAD9) & 0x8000; break;
      case Key::KPDecimal: isPressed = GetAsyncKeyState(VK_DECIMAL) & 0x8000; break;
      case Key::KPDivide: isPressed = GetAsyncKeyState(VK_DIVIDE) & 0x8000; break;
      case Key::KPMultiply: isPressed = GetAsyncKeyState(VK_MULTIPLY) & 0x8000; break;
      case Key::KPSubtract: isPressed = GetAsyncKeyState(VK_SUBTRACT) & 0x8000; break;
      case Key::KPAdd: isPressed = GetAsyncKeyState(VK_ADD) & 0x8000; break;
      case Key::KPEnter: isPressed = GetAsyncKeyState(VK_RETURN) & 0x8000; break;
      case Key::KPEqual: isPressed = GetAsyncKeyState(VK_OEM_NEC_EQUAL) & 0x8000; break;
      case Key::LeftShift: isPressed = GetAsyncKeyState(VK_LSHIFT) & 0x8000; break;
      case Key::LeftControl: isPressed = GetAsyncKeyState(VK_LCONTROL) & 0x8000; break;
      case Key::LeftAlt: isPressed = GetAsyncKeyState(VK_LMENU) & 0x8000; break;
      case Key::LeftSuper: isPressed = GetAsyncKeyState(VK_LWIN) & 0x8000; break;
      case Key::RightShift: isPressed = GetAsyncKeyState(VK_RSHIFT) & 0x8000; break;
      case Key::RightControl: isPressed = GetAsyncKeyState(VK_RCONTROL) & 0x8000; break;
      case Key::RightAlt: isPressed = GetAsyncKeyState(VK_RMENU) & 0x8000; break;
      case Key::RightSuper: isPressed = GetAsyncKeyState(VK_RWIN) & 0x8000; break;
      case Key::Menu: isPressed = GetAsyncKeyState(VK_APPS) & 0x8000; break;
      default: isPressed = false; break;
    };

    return isPressed;

  }

  bool Input::IsKeyDown(Key::KeyCode key){
    return false;
  }

  bool Input::IsKeyRelease(Key::KeyCode key){

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

    glfwGetCursorPos((GLFWwindow*)s_Data.window->get_native_window(), x, y);

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

  void Input::SetWindow(Window* w){
    s_Data.window = w;
  }


};