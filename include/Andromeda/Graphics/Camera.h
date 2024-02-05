#pragma once

#include "Andromeda/Graphics/RenderTarget.h"
#include "Andromeda/HAL/Window.h"

namespace And
{
  class Camera
  {
  public:
    Camera(Window& w);

    ~Camera();
  
    void SetPosition(float* Position);
    void SetPosition(float x, float y, float z);
    void SetDirection(float* Direction);
    void SetDirection(float x, float y, float z);
    void SetFov(float Fov);
    void SetSize(float width, float height);
    void SetNear(float Near);
    void SetFar(float Far);

    const float* GetPosition() const { return m_Position; }
    const float* GetDirection() const { return m_Direction; }
    float GetFov() const { return m_Fov; }
    float GetAspectRation() const { return m_AspectRatio; }
    float GetNear() const { return m_Near; }
    float GetFar() const { return m_Far; }
    const float* GetViewMatrix();
    const float* GetProjectionMatrix();
    const float* GetProjectionViewMatrix();

    void ShowValues();

  private:
    void RecalculeViewMatrix(bool bForce = false);
    void RecalculeProjectionMatrix(bool bForce = false);

    float m_Position[3];
    float m_Direction[3];
    float m_Fov;
    float m_AspectRatio;
    float m_Near;
    float m_Far;

    struct CameraData* m_Data;
  };
}