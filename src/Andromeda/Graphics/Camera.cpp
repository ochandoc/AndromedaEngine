#include "Andromeda/Graphics/Camera.h"

#include "imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace And
{
  struct CameraData
  {
    glm::mat4 ViewMatrix;
    glm::mat4 ProjectionMatrix;
    glm::mat4 ProjectionViewMatrix;
    bool MustRecalculeView;
    bool MustRecalculeProjection;
  };
}

And::Camera::Camera(Window& w)
{
  m_Data = new CameraData;

  SetPosition(0.0f, 0.0f, 0.0f);
  SetDirection(0.0f, 0.0f, 1.0f);
  SetFov(90.0f);
  SetSize((float)w.get_width(), (float)w.get_height());
}

And::Camera::~Camera()
{
  delete m_Data;
}

void And::Camera::SetPosition(float* Position)
{
  m_Position[0] = Position[0];
  m_Position[1] = Position[1];
  m_Position[2] = Position[2];
  m_Data->MustRecalculeView = true;
}

void And::Camera::SetPosition(float x, float y, float z)
{
  m_Position[0] = x;
  m_Position[1] = y;
  m_Position[2] = z;
  m_Data->MustRecalculeView = true;
}

void And::Camera::SetDirection(float* Direction)
{
  glm::vec3 direction(Direction[0], Direction[1], Direction[2]);
  direction = glm::normalize(direction);
  m_Direction[0] = direction.x;
  m_Direction[1] = direction.y;
  m_Direction[2] = direction.z;
  m_Data->MustRecalculeView = true;
}

void And::Camera::SetDirection(float x, float y, float z)
{
  glm::vec3 direction(x, y, z);
  direction = glm::normalize(direction);
  m_Direction[0] = direction.x;
  m_Direction[1] = direction.y;
  m_Direction[2] = direction.z;
  m_Data->MustRecalculeView = true;
}

void And::Camera::SetFov(float Fov)
{
  m_Fov = Fov;
  m_Data->MustRecalculeProjection = true;
}

void And::Camera::SetSize(float width, float height)
{
  m_AspectRatio = width / height;
  m_Data->MustRecalculeProjection = true;
}

void And::Camera::SetNear(float Near)
{
  m_Near = Near;
  m_Data->MustRecalculeProjection = true;
}

void And::Camera::SetFar(float Far)
{
  m_Far = Far;
  m_Data->MustRecalculeProjection = true;
}

const float* And::Camera::GetViewMatrix()
{
  RecalculeViewMatrix();
  return glm::value_ptr(m_Data->ViewMatrix);
}

const float* And::Camera::GetProjectionMatrix()
{
  RecalculeProjectionMatrix();
  return glm::value_ptr(m_Data->ProjectionMatrix);
}

const float* And::Camera::GetProjectionViewMatrix()
{
  RecalculeViewMatrix();
  RecalculeProjectionMatrix();
  return glm::value_ptr(m_Data->ProjectionViewMatrix);
}

void And::Camera::ShowValues()
{
  bool recaluclueView = false;
  bool recaluclueProjection = false;
  ImGui::BulletText("Position");
  ImGui::SameLine();
  if (ImGui::DragFloat3("##CameraPosition", m_Position))
  {
    recaluclueView = true;
  }
  ImGui::BulletText("Direction");
  ImGui::SameLine();
  if (ImGui::DragFloat3("##CameraDirection", m_Direction))
  {
    recaluclueView = true;
  }

  ImGui::BulletText("Fov");
  ImGui::SameLine();
  if (ImGui::DragFloat("##CameraFov", &m_Fov))
  {
    recaluclueProjection = true;
  }
  ImGui::BulletText("Near");
  ImGui::SameLine();
  if (ImGui::DragFloat("##CameraNear", &m_Near))
  {
    recaluclueProjection = true;
  }

  ImGui::BulletText("Far");
  ImGui::SameLine();
  if (ImGui::DragFloat("##CameraFar", &m_Far))
  {
    recaluclueProjection = true;
  }

  if (recaluclueView)
  {
    RecalculeViewMatrix(true);
  }
  if (recaluclueProjection)
  {
    RecalculeProjectionMatrix(true);
  }
}

void And::Camera::RecalculeViewMatrix(bool bForce)
{
  if (m_Data->MustRecalculeView || bForce)
  {
    m_Data->MustRecalculeView = false;

    glm::vec3 position(m_Position[0], m_Position[1], m_Position[2]);
    glm::vec3 direction(m_Direction[0], m_Direction[1], m_Direction[2]);
    glm::vec3 up(0.0f, 1.0f, 0.0f);
    glm::vec3 cameraRight = glm::normalize(glm::cross(up, direction));
    glm::vec3 cameraUp = glm::cross(direction, cameraRight);
    m_Data->ViewMatrix = glm::lookAt(position, position + glm::normalize(direction), cameraUp);

    m_Data->ProjectionViewMatrix = m_Data->ProjectionMatrix * m_Data->ViewMatrix;
  }
}

void And::Camera::RecalculeProjectionMatrix(bool bForce)
{
  if (m_Data->MustRecalculeProjection || bForce)
  {
    m_Data->MustRecalculeProjection = false;

    m_Data->ProjectionMatrix = glm::perspective(glm::radians(m_Fov), m_AspectRatio, m_Near, m_Far);
    m_Data->ProjectionViewMatrix = m_Data->ProjectionMatrix * m_Data->ViewMatrix;
  }
}

namespace And
{
  struct OrthographicCameraData
  {
    glm::mat4 ViewMatrix;
    glm::mat4 ProjectionMatrix;
    glm::mat4 ProjectionViewMatrix;
    bool MustRecalculeView;
    bool MustRecalculeProjection;
  };
}

And::OrthographicCamera::OrthographicCamera(Window& w)
{
  m_Data = new And::OrthographicCameraData;

  SetPosition(0.0f, 0.0f, 0.0f);
  SetDirection(0.0f, 0.0f, 1.0f);
}

And::OrthographicCamera::~OrthographicCamera()
{
  delete m_Data;
}

void And::OrthographicCamera::SetPosition(float* Position)
{
  m_Position[0] = Position[0];
  m_Position[1] = Position[1];
  m_Position[2] = Position[2];
  m_Data->MustRecalculeView = true;
}

void And::OrthographicCamera::SetPosition(float x, float y, float z)
{
  m_Position[0] = x;
  m_Position[1] = y;
  m_Position[2] = z;
  m_Data->MustRecalculeView = true;
}

void And::OrthographicCamera::SetDirection(float* Direction)
{
  glm::vec3 direction(Direction[0], Direction[1], Direction[2]);
  direction = glm::normalize(direction);
  m_Direction[0] = direction.x;
  m_Direction[1] = direction.y;
  m_Direction[2] = direction.z;
  m_Data->MustRecalculeView = true;
}

void And::OrthographicCamera::SetDirection(float x, float y, float z)
{
  glm::vec3 direction(x, y, z);
  direction = glm::normalize(direction);
  m_Direction[0] = direction.x;
  m_Direction[1] = direction.y;
  m_Direction[2] = direction.z;
  m_Data->MustRecalculeView = true;
}

void And::OrthographicCamera::SetNear(float Near)
{
  m_Near = Near;
  m_Data->MustRecalculeProjection = true;
}

void And::OrthographicCamera::SetFar(float Far)
{
  m_Far = Far;
  m_Data->MustRecalculeProjection = true;
}

void And::OrthographicCamera::SetQuad(float left, float right, float top, float bottom)
{
  m_Right = right;
  m_Left = left;
  m_Top = top;
  m_Bottom = bottom;
  m_Data->MustRecalculeProjection;
}

const float* And::OrthographicCamera::GetViewMatrix()
{
  RecalculeViewMatrix();
  return glm::value_ptr(m_Data->ViewMatrix);
}

const float* And::OrthographicCamera::GetProjectionMatrix()
{
  RecalculeProjectionMatrix();
  return glm::value_ptr(m_Data->ProjectionMatrix);
}

const float* And::OrthographicCamera::GetProjectionViewMatrix()
{
  RecalculeViewMatrix();
  RecalculeProjectionMatrix();
  return glm::value_ptr(m_Data->ProjectionViewMatrix);
}

void And::OrthographicCamera::RecalculeViewMatrix(bool bForce)
{
  if (m_Data->MustRecalculeView || bForce)
  {
    m_Data->MustRecalculeView = false;

    glm::vec3 position(m_Position[0], m_Position[1], m_Position[2]);
    glm::vec3 direction(m_Direction[0], m_Direction[1], m_Direction[2]);
    glm::vec3 up(0.0f, 1.0f, 0.0f);
    glm::vec3 cameraRight = glm::normalize(glm::cross(up, direction));
    glm::vec3 cameraUp = glm::cross(direction, cameraRight);
    m_Data->ViewMatrix = glm::lookAt(position, position + glm::normalize(direction), cameraUp);

    m_Data->ProjectionViewMatrix = m_Data->ProjectionMatrix * m_Data->ViewMatrix;
  }
}

void And::OrthographicCamera::RecalculeProjectionMatrix(bool bForce)
{
  if (m_Data->MustRecalculeProjection || bForce)
  {
    m_Data->MustRecalculeProjection = false;

    m_Data->ProjectionMatrix = glm::ortho(m_Left, m_Right, m_Top, m_Bottom, m_Near, m_Far);
    m_Data->ProjectionViewMatrix = m_Data->ProjectionMatrix * m_Data->ViewMatrix;
  }
}
