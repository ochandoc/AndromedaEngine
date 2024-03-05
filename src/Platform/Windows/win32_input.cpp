#include "Andromeda/HAL/configuration.h"

#include "Andromeda/HAL/Input.h"
#include "Andromeda/HAL/Window.h"
#include "Andromeda/HAL/ActionInput.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "GLFW/glfw3.h"


namespace And {

  Input::Input(Window& w) : m_window(w) {}


  bool Input::check_action(const ActionInput& action)
  {
    bool activated = false;
    for (int32 i = 0; i < action.m_Keys.size() && !activated; i++)
    {
      activated = (GetKeyState(action.m_Keys[i]) == action.m_State);
    }
   return activated;
  }

  bool Input::IsKeyDown(KeyCode key) {
    return glfwGetKey((GLFWwindow*)m_window.get_native_window(), (int)key) == GLFW_PRESS;
  }

  bool Input::IsKeyRelease(KeyCode key) {
    return glfwGetKey((GLFWwindow*)m_window.get_native_window(), (int)key) == GLFW_RELEASE;
  }

  KeyState Input::GetKeyState(KeyCode key)
  {
    int state = glfwGetKey((GLFWwindow*)m_window.get_native_window(), (int)key);
    switch (state)
    {
    case GLFW_PRESS:
      return KeyState::Press;
      break;
    case GLFW_RELEASE:
      return KeyState::Release;
      break;
    }

    return KeyState::Default;
  }

  bool Input::IsMouseButtonPressed(MouseCode key) {

    bool isPressed = false;

    switch (key) {
    case MouseCode::Left: isPressed = GetAsyncKeyState(VK_LBUTTON) & 0x8000; break;
    case MouseCode::Right: isPressed = GetAsyncKeyState(VK_RBUTTON) & 0x8000; break;
    default: isPressed = false; break;
    }

    return isPressed;
  }

  void Input::GetMousePosition(double* x, double* y) {

    glfwGetCursorPos((GLFWwindow*)m_window.get_native_window(), x, y);
  }

  double Input::GetMouseX() {
    double x, y;

    glfwGetCursorPos((GLFWwindow*)m_window.get_native_window(), &x, &y);
    return x;
  }

  double Input::GetMouseY() {
    double x, y;

    glfwGetCursorPos((GLFWwindow*)m_window.get_native_window(), &x, &y);
    return y;
  }


  Input::~Input() {
  }
};