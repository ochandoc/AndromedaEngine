#pragma once

#include "Common/KeyCodes.h"


namespace And{
  class Window;

  class Input{
    
    public:

    static bool IsKeyPressed(Key::KeyCode key);
    static bool IsKeyDown(Key::KeyCode key);
    static bool IsKeyRelease(Key::KeyCode key);
    
    static void SetWindow(Window *w);

    static bool IsMouseButtonPressed(Key::MouseCode key);

    static void GetMousePosition(double *x, double *y);
    static double GetMouseX();
    static double GetMouseY();


  };
  
};