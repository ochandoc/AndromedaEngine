#pragma once

#include "Andromeda/Misc/CoreMiscDefines.h"
#include "Andromeda/HAL/Types.h"

#include "Andromeda/ECS/Entity.h"

namespace And
{
  /**
   * @class ComponentBase
   * @brief Base class for all components in the entity-component system.
   */
  class ComponentBase
  {
  public:
    /**
     * @brief Gets the owner entity of this component.
     * @return Pointer to the owner entity.
     */
    Entity* GetOwner() { return m_Owner; }

    friend class EntityComponentSystem;
  private:
    /**
    * @brief Sets the owner entity of this component.
    * @param ett Pointer to the entity to set as owner.
    */
    void SetOwner(Entity* ett) { m_Owner = ett; }

    Entity* m_Owner = nullptr;  /**< Pointer to the owner entity. */
  };
}
