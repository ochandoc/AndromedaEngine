#pragma once

#include "Andromeda/ECS/ComponentBase.h"
#include "Andromeda/ECS/Components/TransformComponent.h"

namespace And
{
  class SceneComponent : public ComponentBase
  {
  public:
    SceneComponent* Parent = nullptr;
    Mat4 matrix;
  };
}
