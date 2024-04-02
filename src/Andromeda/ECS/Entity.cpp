#include "Andromeda/ECS/Entity.h"

namespace And {

  Entity::Entity() {
    m_Id = ID();
    m_ECS = nullptr;
    m_Scene = nullptr;
  }

  Entity::~Entity() {}

}