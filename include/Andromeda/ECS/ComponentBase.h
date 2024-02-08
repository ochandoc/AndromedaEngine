#pragma once

#include "Andromeda/Misc/CoreMiscDefines.h"
#include "Andromeda/HAL/Types.h"

#include "Andromeda/ECS/Entity.h"

namespace And
{
  class ComponentBase
  {
  public:
    Entity* GetOwner() { return m_Owner; }

    friend class EntityComponentSystem;
  private:
    void SetOwner(Entity* ett) { m_Owner = ett; }

    Entity* m_Owner = nullptr;
  };
}
