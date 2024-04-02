#pragma once

#include "Andromeda/ECS/Components/SceneComponent.h"
#include "Andromeda/ECS/Components/TransformComponent.h"
#include "Andromeda/ECS/EntityComponentSystem.h"
#include "Andromeda/Graphics/Renderer.h"

namespace And
{
  class Scene
  {
  public:
    Scene();
    ~Scene();

    Entity* NewSceneEntity();
    Entity* NewEmptyEntity();

    friend class Renderer;
  private:
    std::vector<Entity*> m_Entities;
    EntityComponentSystem m_ECS;
  };
}