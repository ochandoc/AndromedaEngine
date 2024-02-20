#pragma once

#include "Andromeda/Misc/CoreMiscDefines.h"
#include "Andromeda/HAL/Types.h"

#include "Andromeda/HAL/Window.h"
#include "Andromeda/TaskSystem/TaskSystem.h"

#include "Andromeda/Resources/Resource.h"
#include "Andromeda/Resources/ResourceGenerator.h"

namespace And
{
  class ResourceManager
  {
    NON_COPYABLE_CLASS(ResourceManager)
    NON_MOVABLE_CLASS(ResourceManager)
  public:
    ResourceManager(Window& w, TaskSystem& ts);

    ~ResourceManager();

    template<typename T>
    void AddGenerator();

    template<typename T>
    Resource<T> NewResource(const std::string& path);

    template<typename T>
    void ReloadResource(const std::string& path);

  private:
    TaskSystem& m_TaskSystem;
    std::unordered_map<size_t, std::unordered_map<size_t, std::shared_ptr<IResourceBase>>> m_Resources;
    std::unordered_map<size_t, std::shared_ptr<IResourceGeneratorBase>> m_Generators;
  };
}

#include "inl/ResourceManager.inl"