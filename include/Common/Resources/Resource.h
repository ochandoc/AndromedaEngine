#pragma once

#include "Common/Misc/CoreMiscDefines.h"

namespace And
{
  class IResourceBase
  {
    INTERFACE_BODY(IResourceBase)
  public:
    size_t GetId() const { return m_Id; }

    friend class ResourceManager;
  protected:
    size_t m_Id;
  };

  template<typename T>
  class ResourceValue : public IResourceBase
  {
  public:
    std::shared_ptr<T> m_Resource;
  };

  template<typename T>
  class Resource
  {
  public:
    Resource() = default;
    Resource(const Resource& other) { m_ResourceValue = other.m_ResourceValue; }
    Resource(Resource&& other) { m_ResourceValue = std::move(other.m_ResourceValue); }

    ~Resource() = default;

    Resource& operator =(const Resource& other) { if (this != &other) { m_ResourceValue = other.m_ResourceValue; } return *this; }
    Resource& operator =(Resource&& other) { if (this != &other) { m_ResourceValue = std::move(other.m_ResourceValue); } return *this; }

    size_t GetId() const { return m_ResourceValue->GetId(); }

    T& operator*() { return *(m_ResourceValue->m_Resource); }
    const T& operator*() const { return *(m_ResourceValue->m_Resource); }
    T* operator->() { return m_ResourceValue->m_Resource.get(); }
    const T* operator->() const { return m_ResourceValue->m_Resource.get(); }

    friend class ResourceManager;
  private:
    std::shared_ptr<ResourceValue<T>> m_ResourceValue;
  };
}