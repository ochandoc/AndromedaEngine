#include "Andromeda/ECS/Entity.h"

namespace And {

  Entity::Entity() {
    m_Id = ID();
  }

  Entity::Entity(const Entity& other) {
    m_Id = other.get_id();
  }

  Entity::Entity(Entity&& other) {
    m_Id = other.get_id();
  }

  Entity::~Entity() {}

  Entity& Entity::operator =(const Entity& other) {
    if (this != &other)
    {
      m_Id = other.get_id();
    }
    return *this;
  }

  Entity& Entity::operator =(Entity&& other) {
    if (this != &other)
    {
      ID tmp = m_Id;
      m_Id = other.m_Id;
      other.m_Id = tmp;
    }
    return *this;
  }

}