#pragma once

#include "Andromeda/Misc/CoreMiscDefines.h"
#include "Andromeda/HAL/Types.h"
#include "Andromeda/Misc/ID.h"

namespace And
{
  class Scene;

  /**
   * @class Entity
   * @brief Represents an entity in the entity-component system.
   */
  class Entity
  {
    NON_COPYABLE_CLASS(Entity)  /**< Macro to disable copy operations for Entity class. */
    NON_MOVABLE_CLASS(Entity)   /**< Macro to disable move operations for Entity class. */
  private:
    /**
     * @brief Private constructor to enforce non-instantiability from outside.
     */
    Entity();

  public:
    /**
     * @brief Destructor for Entity.
     */
    ~Entity();

    /**
     * @brief Gets the ID of the entity.
     * @return Constant reference to the ID.
     */
    const ID& get_id() const { return m_Id; }

    /**
     * @brief Gets a component of type T attached to the entity.
     * @tparam T Type of the component.
     * @return Pointer to the component of type T.
     */
    template<typename T>
    T* get_component();

    /**
     * @brief Adds a component of type T to the entity.
     * @tparam T Type of the component.
     * @param comp Reference to the component to be added.
     */
    template<typename T>
    void add_component(T& comp);

    friend class EntityComponentSystem;
  private:
    class EntityComponentSystem* m_ECS;   /**< Pointer to the EntityComponentSystem managing this entity. */
    Scene* m_Scene;                       /**< Pointer to the scene this entity belongs to. */
    ID m_Id;                              /**< ID of the entity. */

  };
}

#include "Andromeda/ECS/EntityComponentSystem.h"

namespace And
{
  template<typename T>
  inline T* Entity::get_component()
  {
    return m_ECS->get_entity_component<T>(this);
  }

  template<typename T>
  inline void Entity::add_component(T& comp)
  {
    m_ECS->add_entity_component<T>(this, comp);
  }
}