#pragma once

#include "Andromeda/Misc/CoreMiscDefines.h"
#include "Andromeda/HAL/Types.h"
#include "Andromeda/Misc/ID.h"

namespace And
{
  class Entity
  {
  public:
    Entity();
    Entity(const Entity& other);
    Entity(Entity&& other);

    ~Entity();

    Entity& operator =(const Entity& other);
    Entity& operator =(Entity&& other);

    const ID& get_id() const { return m_Id; }

  private:
    ID m_Id;
  };
}