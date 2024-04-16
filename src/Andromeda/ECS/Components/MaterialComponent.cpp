#include "Andromeda/ECS/Components/MaterialComponent.h"

namespace And
{
  MaterialComponent::MaterialComponent()
  {
    m_Material = std::make_shared<Material>();
  }
  MaterialComponent::~MaterialComponent()
  {
  }
}