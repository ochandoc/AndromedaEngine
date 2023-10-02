#pragma once

//#include "configuration.h"
#include "Common/KeyCodes.h"

#ifndef AND_WINDOWS
#define AND_WINDOWS
#endif 

#ifdef AND_WINDOWS
  struct GLFWwindow;
#endif

namespace And{
  
  class Input{
    public:

    static bool IsKeyPressed(Key::KeyCode key);
    static bool IsKeyDown(Key::KeyCode key);
    static bool IsKeyRelease(Key::KeyCode key);

    static bool IsMouseButtonPressed(Key::MouseCode key);
#ifdef AND_WINDOWS
    static void GetMousePosition(GLFWwindow *window ,double *x, double *y);
#else
    static void GetMousePosition(double *x, double *y);
#endif
    static double GetMouseX();
    static double GetMouseY();

  };
};