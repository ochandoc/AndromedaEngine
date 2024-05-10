#include "Andromeda/Graphics/FlyCamera.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

And::FlyCamera::FlyCamera(Window& w) : Camera(w), m_Input(w), m_Window(w)
{
  m_Speed = 10.0f;
  m_FirstMouse = true;
  m_LastMouseX = 0.0f;
  m_LastMouseY = 0.0f;
  m_Pitch = 0.0f;
  m_Yaw = 0.0f;
  m_Sensitivity = 0.1f;
  m_can_move = true;
}

And::FlyCamera::~FlyCamera()
{
}

void And::FlyCamera::SetFixed(bool move){
  m_can_move = move;
}

bool And::FlyCamera::GetFixed(){
  return m_can_move;
}

void And::FlyCamera::ProcessInput()
{
  glm::vec3 position = glm::make_vec3(GetPosition());
  glm::vec3 forward = glm::make_vec3(GetDirection());
  glm::vec3 up(0.0f, -1.0f, 0.0f);

  const float speed = m_Speed * m_Window.get_delta_time();

  if (m_Input.IsKeyDown(KeyCode::W))
    position += speed * forward;
  if (m_Input.IsKeyDown(KeyCode::S))
    position -= speed * forward;
  if (m_Input.IsKeyDown(KeyCode::A))
    position += glm::normalize(glm::cross(forward, up)) * speed;
  if (m_Input.IsKeyDown(KeyCode::D))
    position -= glm::normalize(glm::cross(forward, up)) * speed;

  const float MouseX = static_cast<float>(m_Input.GetMouseX());
  const float MouseY = static_cast<float>(m_Input.GetMouseY());

  if (m_Input.IsMouseButtonPressed(MouseCode::Right))
  {

    if (m_FirstMouse)
    {
      m_LastMouseX = MouseX;
      m_LastMouseY = MouseY;
      //m_Pitch = asin(forward.y / forward.length());
      //m_Yaw = asin(forward.x / (cos(m_Pitch) * forward.length()));
      m_FirstMouse = false;
    }

    float OffsetX = MouseX - m_LastMouseX;
    float OffsetY = MouseY - m_LastMouseY;

    OffsetX *= m_Sensitivity;
    OffsetY *= m_Sensitivity;

    m_Yaw += OffsetX;
    m_Pitch -= OffsetY;

    if (m_Pitch > 89.0f)
      m_Pitch = 89.0f;
    if (m_Pitch < -89.0f)
      m_Pitch = -89.0f;

    forward.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    forward.y = sin(glm::radians(m_Pitch));
    forward.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));

    SetDirection(forward.x, forward.y, forward.z);
  }

  m_LastMouseX = MouseX;
  m_LastMouseY = MouseY;

  SetPosition(position.x, position.y, position.z);
}

void And::FlyCamera::SetSpeed(const float speed) {
    m_Speed = speed;
}
