#pragma once

#include "Andromeda/Misc/CoreMiscDefines.h"
#include "Andromeda/HAL/Types.h"

#include "Andromeda/HAL/KeyCodes.h"

namespace And
{
struct KeyboardState;
class Window;
class ActionInput;

class Input
{
  NON_COPYABLE_CLASS(Input)
  NON_MOVABLE_CLASS(Input)
public:
  Input(Window& w);
  ~Input();

  bool check_action(const ActionInput& action);

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