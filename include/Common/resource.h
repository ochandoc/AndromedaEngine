#pragma once

#include "base.h"

namespace And
{
  class ResourceManager;

  namespace internal
  {
    class resource_base
    {
    public:
      resource_base() = default;
      ~resource_base() = default;

      size_t get_id() const { return m_Id; }

      friend class ResourceManager;
    protected:
      size_t m_Id;
    };

    template<typename T>
    class resource_value : public resource_base
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
    resource(const resource& other) { m_Resource = other.m_Resource; }
    resource(resource&& other) { m_Resource = std::move(other.m_Resource); }

    ~resource() = default;

    resource& operator =(const resource& other) { if (this != &other) { m_Resource = other.m_Resource; } return *this; }
    resource& operator =(resource&& other) { if (this != &other) { m_Resource = std::move(other.m_Resource); } return *this; }

    size_t get_id() const { return m_Resource->get_id(); }

    T& operator*() { return *(m_Resource->m_Value); }
    const T& operator*() const { return *(m_Resource->m_Value); }
    T* operator->() { return m_Resource->m_Value.get(); }
    const T* operator->() const { return m_Resource->m_Value.get(); }

    friend class ResourceManager;
  private:
    std::shared_ptr<internal::resource_value<T>> m_Resource;
  };
}