#include "configuration.h"

#include "Common/Input.h"
#include "Common/Window.h"
#include "Common/ActionInput.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "GLFW/glfw3.h"


namespace And{

  Input::Input(Window& w) : m_window(w), m_KeyBoard(w.m_KeyBoard){}


  bool Input::check_action(const ActionInput& action)
  {
    bool activated = false;
    for (int32 i = 0; i < action.m_Keys.size() && !activated; i++)
    {
      activated = (m_KeyBoard.keys[(int)action.m_Keys[i]] == action.m_State);
    }
    return activated;
  }

  bool Input::IsKeyPressed(KeyCode key){
    return m_KeyBoard.keys[(int)key] == KeyState::Repeat;
  }

  bool Input::IsKeyDown(KeyCode key){
    return m_KeyBoard.keys[(int)key] == KeyState::Press;
  }

  bool Input::IsKeyRelease(KeyCode key){
    return m_KeyBoard.keys[(int)key] == KeyState::Release;

  }

  bool Input::IsMouseButtonPressed(MouseCode key){

    bool isPressed = false;

    switch (key){
    case MouseCode::Left: isPressed = GetAsyncKeyState(VK_LBUTTON) & 0x8000; break;
      case MouseCode::Right: isPressed = GetAsyncKeyState(VK_RBUTTON) & 0x8000; break;
      default: isPressed = false; break;
    }

    return isPressed;
  }

  void Input::GetMousePosition(double *x, double *y){

    glfwGetCursorPos((GLFWwindow*)m_window.get_native_window(), x, y);
  }
  
  double Input::GetMouseX(){
    double x,y;

    glfwGetCursorPos((GLFWwindow*)m_window.get_native_window(), &x, &y);
    return x;
  }

  double Input::GetMouseY(){
    double x,y;

    glfwGetCursorPos((GLFWwindow*)m_window.get_native_window(), &x, &y);
    return y;
  }


  Input::~Input(){
  }
};