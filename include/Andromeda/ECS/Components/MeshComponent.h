#pragma once

#include "Andromeda/Misc/CoreMiscDefines.h"
#include "Andromeda/HAL/Types.h"

#include "Andromeda/ECS/ComponentBase.h"
#include "Andromeda/Resources/Resource.h"
#include "Andromeda/Graphics/ObjLoader.h"

namespace And
{
  class MeshComponent : public ComponentBase
  {
  public:
    Resource<ObjLoader> Mesh;
    std::shared_ptr<ObjLoader> MeshOBJ;
  };
}