#pragma once

#include "Andromeda/Misc/CoreMiscDefines.h"
#include "Andromeda/HAL/Types.h"
#include "Andromeda/Misc/ID.h"

namespace And
{
  class Scene;

  class Entity
  {
    NON_COPYABLE_CLASS(Entity)
    NON_MOVABLE_CLASS(Entity)
  private:
    Entity();

  public:
    ~Entity();

    const ID& get_id() const { return m_Id; }

    template<typename T>
    T* get_component();

    template<typename T>
    void add_component(T& comp);

    friend class EntityComponentSystem;
  private:
    class EntityComponentSystem* m_ECS;
    Scene* m_Scene;
    ID m_Id;
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