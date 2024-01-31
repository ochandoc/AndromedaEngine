#pragma once

#include "Andromeda/Graphics/RenderTarget.h"
#include "Andromeda/HAL/Window.h"

namespace And
{
  class Camera
  {
  public:
    Camera(Window& w);
  
    void SetPosition(float* Position);
    void SetPosition(float x, float y, float z);

    const float* GetPosition() const { return m_Position; }
    const float* GetDirection() const { return m_Direction; }

    RenderTarget& GetRenderTarget() { return m_RenderTarget; }

  private:
    RenderTarget m_RenderTarget;
    float m_Position[3];
    float m_Direction[3];
    float m_Fov;
    float m_AspectRatio;
  };
}