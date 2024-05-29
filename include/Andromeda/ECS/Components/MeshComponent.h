#pragma once

#include "Andromeda/Misc/CoreMiscDefines.h"
#include "Andromeda/HAL/Types.h"

#include "Andromeda/ECS/ComponentBase.h"
#include "Andromeda/Graphics/Geometry.h"

#include "Andromeda/Graphics/Mesh.h"
#include "Andromeda/Physics/RigidBody.h"

namespace And
{
  class MeshComponent : public ComponentBase
  {
  public:
    MeshComponent() = default;

    void SetMesh(std::shared_ptr<Mesh> mesh) { m_Mesh = mesh; }

    std::shared_ptr<Mesh> GetMesh() { return m_Mesh; }
    const std::shared_ptr<Mesh> GetMesh() const { return m_Mesh; }

    std::shared_ptr<Geometry> MeshOBJ;


  private:
    std::shared_ptr<Mesh> m_Mesh;
  };
}