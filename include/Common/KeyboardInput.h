#pragma once

#include "Common/KeyCodes.h"


namespace And{
  class Window;

  class Input{
    
    public:

    Input(Window& w);
    ~Input();

    bool IsKeyPressed(Key::KeyCode key);
    bool IsKeyDown(Key::KeyCode key);
    bool IsKeyRelease(Key::KeyCode key);

    bool IsMouseButtonPressed(Key::MouseCode key);
    void GetMousePosition(double *x, double *y);
    double GetMouseX();
    double GetMouseY();

    void update_input();
    void init_input();

    const unsigned int kTotalKeys = 123;

    private:
    Window& m_window;
  };
  
};