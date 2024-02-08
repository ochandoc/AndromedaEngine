#pragma once

#include "Andromeda/Misc/CoreMiscDefines.h"
#include "Andromeda/HAL/Types.h"

#include "Andromeda/ECS/ComponentBase.h"

namespace And
{
  class TransformComponent : public ComponentBase
  {
  public:
    float position[3];
    float rotation[3];
    float scale[3];
  };
}