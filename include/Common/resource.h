#pragma once

#include "base.h"

namespace And
{
  class ResourceManager;

  namespace internal
  {
    template<typename T>
    class resource_value
    {
    public:
      std::shared_ptr<T> m_Value;
    };
  }

  template<typename T>
  class resource
  {
  private:
    resource() = default;

  public:
    resource(const resource& other) { m_Resource = other.m_Resource; m_Id = other.m_Id; }
    resource(resource&& other) { m_Resource = std::move(other.m_Resource); m_Id = other.m_Id; }

    ~resource() = default;

    resource& operator =(const resource& other) { if (this != &other) { m_Resource = other.m_Resource; m_Id = other.m_Id; } return *this; }
    resource& operator =(resource&& other) { if (this != &other) { m_Resource = std::move(other.m_Resource); std::swap(m_Id, other.m_Id); } return *this; }

    T& operator*() { return *(m_Resource->m_Value); }
    const T& operator*() const { return *(m_Resource->m_Value); }
    T* operator->() { return m_Resource->m_Value.get(); }
    const T* operator->() const { return m_Resource->m_Value.get(); }

    friend class ResourceManager;
  private:
    std::shared_ptr<internal::resource_value<T>> m_Resource;
    size_t m_Id;
  };

  class ResourceManager
  {
    NON_COPYABLE_CLASS(ResourceManager)
    NON_MOVABLE_CLASS(ResourceManager)
  public:
    ResourceManager() = default;

    ~ResourceManager() = default;

    
    resource<int> new_resource()
    {
      resource<int> r;
      r.m_Resource = std::make_shared<internal::resource_value<int>>();
      r.m_Resource->m_Value = std::make_shared<int>();
      *(r.m_Resource->m_Value) = 10;

      auto sf = [this, r]() {r.m_Resource->m_Value = std::make_shared<int>(100); };

      swap_ = std::bind(sf);
      return r;
    }

    void swap()
    {
      swap_();
    }

  private:
    std::function<void()> swap_;
  };
}