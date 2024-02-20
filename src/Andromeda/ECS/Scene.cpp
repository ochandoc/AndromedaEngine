#include "Andromeda/ECS/Scene.h"

#include "Andromeda/Graphics/Shader.h"
#include "Andromeda/ECS/Entity.h"

namespace And
{
  Scene::Scene()
  {
    m_ECS.add_component_class<SceneComponent>();
    m_ECS.add_component_class<TransformComponent>();
    m_ECS.add_component_class<MeshComponent>();
  }

  Scene::~Scene()
  {
  }

  Entity* Scene::NewSceneEntity()
  {
    TransformComponent tr;
    SceneComponent sc;
    Entity* ett = m_ECS.new_entity(tr, sc);

    m_Entities.push_back(ett);
    return ett;
  }

  Entity* Scene::NewEmptyEntity()
  {
    Entity* ett = m_ECS.new_entity();

    m_Entities.push_back(ett);
    return ett;
  }
}

