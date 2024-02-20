#pragma once

#include "Andromeda/Misc/CoreMiscDefines.h"
#include "Andromeda/HAL/Types.h"

#include "Andromeda/ECS/ComponentBase.h"

namespace And
{
  struct Mat4
  {
    float d[16] = { 0.0f };
  };

  class TransformComponent : public ComponentBase
  {
  public:
    float position[3] = { 0.0f };
    float rotation[3] = { 0.0f };
    float scale[3] = { 1.0f };

    Mat4 matrix;
  };
}