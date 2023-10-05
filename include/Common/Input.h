#pragma once

#include "base.h"

#include "Common/KeyCodes.h"

namespace And
{
struct KeyboardState;
class Window;

class Input
{
  NON_COPYABLE_CLASS(Input)
  NON_MOVABLE_CLASS(Input)
public:
  Input(Window& w);
  ~Input();

  bool IsKeyPressed(KeyCode key);
  bool IsKeyDown(KeyCode key);
  bool IsKeyRelease(KeyCode key);

  bool IsMouseButtonPressed(MouseCode key);
  void GetMousePosition(double *x, double *y);

  double GetMouseX();
  double GetMouseY();
    
private:
  Window& m_window;
  KeyboardState& m_KeyBoard;
};
  
};