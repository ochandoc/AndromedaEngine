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

  /**
   * @brief Check input of a action
   * 
   * @param action 
   * @return true 
   * @return false 
   */
  bool check_action(const ActionInput& action);

  /**
   * @brief Return if selected key is down
   * 
   * @param key 
   * @return true 
   * @return false 
   */
  bool IsKeyDown(KeyCode key);

  /**
   * @brief Return if key selected is just released
   * 
   * @param key 
   * @return true 
   * @return false 
   */
  bool IsKeyRelease(KeyCode key);

  /**
   * @brief Get the Key State of a key
   * 
   * @param key 
   * @return KeyState 
   */
  KeyState GetKeyState(KeyCode key);

  /**
   * @brief Return if mouse key is pressed 
   * 
   * @param key 
   * @return true 
   * @return false 
   */
  bool IsMouseButtonPressed(MouseCode key);

  /**
   * @brief Get the Mouse Position
   * 
   * @param x 
   * @param y 
   */
  void GetMousePosition(double *x, double *y);

  /**
   * @brief Get mouse position X
   * 
   */
  double GetMouseX();

  /**
   * @brief Get mouse position Y
   * 
   * @return double 
   */
  double GetMouseY();
    
private:
  Window& m_window;
};
  
};