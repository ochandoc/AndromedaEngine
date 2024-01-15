#pragma once

#include "Common/Misc/CoreMiscDefines.h"

#include "Common/Window.h"
#include "Common/TaskSystem/TaskSystem.h"

#include "Common/Resources/Resource.h"
#include "Common/Resources/ResourceGenerator.h"

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