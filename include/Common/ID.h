#pragma once

#include "base.h"

namespace And
{
  class ID
  {
  public:
    ID();
    ID(uint64 value);
    ID(const ID& other);
    ID(ID&& other);

    ~ID() = default;

    ID& operator =(const ID& other);
    ID& operator =(ID&& other);

    uint64 get() const { return m_Id; }

    operator uint64() const { return m_Id; }

  private:
    uint64 m_Id;
  };
}

namespace std
{
  template<>
  struct hash<And::ID>
  {
    std::size_t operator()(const And::ID& id) const
    {
      return id.get();
    }
  };

}