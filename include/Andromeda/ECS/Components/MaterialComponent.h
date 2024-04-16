#pragma once 

#include "Andromeda/ECS/ComponentBase.h"
#include "Andromeda/Graphics/Material.h"

namespace And
{
  class MaterialComponent : public ComponentBase
  {
  public:
    MaterialComponent();
    ~MaterialComponent();

    void SetMaterial(std::shared_ptr<Material> mat) { m_Material = mat; }

    inline std::shared_ptr<Material> GetMaterial() { return m_Material; }
    inline const std::shared_ptr<Material> GetMaterial() const { return m_Material; }

  private:
    std::shared_ptr<Material> m_Material;
  };
}