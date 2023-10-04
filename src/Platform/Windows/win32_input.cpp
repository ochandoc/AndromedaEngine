#include "configuration.h"

#include "Common/KeyboardInput.h"
#include "Common/Window.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "GLFW/glfw3.h"


namespace And{

  Input::Input(Window& w) : m_window(w), m_KeyBoard(w.m_KeyBoard){}

  void Input::init_input(){
    
  }

  void Input::update_input(){
      
    // Swap input buffers
    for(unsigned int i = 0; i < kTotalKeys; i++){
      m_KeyBoard.keysBufferAfter[i] = m_KeyBoard.keysBufferBefore[i];
      m_KeyBoard.keysBufferBefore[i] = false;
    }
  
  }

  bool Input::IsKeyPressed(Key::KeyCode key){

    // Si en el frame de antes estaba presionada y en este tambien
    if(m_KeyBoard.keysBufferAfter[key]){
      return m_KeyBoard.keysBufferBefore[key];
    }

    return false;
  }

  bool Input::IsKeyDown(Key::KeyCode key){

    // Si antes no estaba presionada y ahora si
    if(m_KeyBoard.keysBufferAfter[key] == false){
      return m_KeyBoard.keysBufferBefore[key];
    }

    // Si en el frame de antes estaba siendo presionada, ya no puede ser true
    return false;
  }

  bool Input::IsKeyRelease(Key::KeyCode key){

    // Si en el frame de antes estaba presionada y en este frame ya no
    if(m_KeyBoard.keysBufferAfter[key] && !m_KeyBoard.keysBufferBefore[key]){
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
    // Desinstalar el hook antes de salir
    //UnhookWindowsHookEx(s_Data.hook);
  }
};