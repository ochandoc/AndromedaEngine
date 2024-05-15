#pragma once

#include "Andromeda/Misc/CoreMiscDefines.h"
#include "Andromeda/HAL/Types.h"

#include "Andromeda/Graphics/RenderTarget.h"
#include "Andromeda/HAL/Window.h"

namespace And
{
  class CameraBase
  {
  public:
    CameraBase() = default;
    
    virtual ~CameraBase() = default;
    
    virtual void SetPosition(float* Position) = 0;
    virtual void SetPosition(float x, float y, float z) = 0;
    virtual void SetDirection(float* Direction) = 0;
    virtual void SetDirection(float x, float y, float z) = 0;
    virtual void SetNear(float Near) = 0;
    virtual void SetFar(float Far) = 0;
    
    virtual const float* GetPosition() const  = 0;
    virtual const float* GetDirection() const = 0;
    virtual float GetNear() const = 0;
    virtual float GetFar() const = 0;
    
    virtual const float* GetViewMatrix() = 0;
    virtual const float* GetProjectionMatrix() = 0;
    virtual const float* GetProjectionViewMatrix() = 0;
  };

  class Camera : public CameraBase
  {
  public:
    Camera(Window& w);

    virtual ~Camera();
  
    virtual void SetPosition(float* Position) override;
    virtual void SetPosition(float x, float y, float z) override;
    virtual void SetDirection(float* Direction) override;
    virtual void SetDirection(float x, float y, float z) override;
    void SetFov(float Fov);
    void SetSize(float width, float height);
    virtual void SetNear(float Near) override;
    virtual void SetFar(float Far) override;

    virtual const float* GetPosition() const override  { return m_Position; }
    virtual const float* GetDirection() const override { return m_Direction; }
    const float* GetUp() { return m_up; }
    const float* GetRight() { return m_right; }
    float GetFov() const { return m_Fov; }
    float GetAspectRation() const { return m_AspectRatio; }
    virtual float GetNear() const override { return m_Near; }
    virtual float GetFar() const override { return m_Far; }
    const float* GetViewMatrix();
    const float* GetProjectionMatrix();
    const float* GetProjectionViewMatrix();

    

    virtual void ShowValues();  

  private:
    void RecalculeViewMatrix(bool bForce = false);
    void RecalculeProjectionMatrix(bool bForce = false);

    float m_Position[3];
    float m_Direction[3];
    float m_up[3];
    float m_right[3];
    float m_Fov;
    float m_AspectRatio;
    float m_Near;
    float m_Far;

    struct CameraData* m_Data;
  };
  
  class OrthographicCamera : public CameraBase
  {
  public:
    OrthographicCamera(Window& w);

    virtual ~OrthographicCamera();
  
    virtual void SetPosition(float* Position) override;
    virtual void SetPosition(float x, float y, float z) override;
    virtual void SetDirection(float* Direction) override;
    virtual void SetDirection(float x, float y, float z) override;
    virtual void SetNear(float Near) override;
    virtual void SetFar(float Far) override;
    void SetQuad(float left, float rignt, float top, float bottom);

    virtual const float* GetPosition() const override  { return m_Position; }
    virtual const float* GetDirection() const override { return m_Direction; }
    virtual float GetNear() const override { return m_Near; }
    virtual float GetFar() const override { return m_Far; }
    const float* GetViewMatrix();
    const float* GetProjectionMatrix();
    const float* GetProjectionViewMatrix();

  private:
    void RecalculeViewMatrix(bool bForce = false);
    void RecalculeProjectionMatrix(bool bForce = false);

    float m_Position[3];
    float m_Direction[3];
    float m_Left, m_Right, m_Top, m_Bottom;
    float m_Near;
    float m_Far;

    struct OrthographicCameraData* m_Data;
  };
}