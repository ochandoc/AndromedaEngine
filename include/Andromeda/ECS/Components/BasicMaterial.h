#pragma once

#include "Andromeda/ECS/ComponentBase.h"
#include "Andromeda/Graphics/Texture.h"

namespace And
{
  class BasicMaterial : public ComponentBase
  {
  public:
    std::shared_ptr<Texture> texture;
  };
}