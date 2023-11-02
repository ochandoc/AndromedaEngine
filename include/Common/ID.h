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
<<<<<<< HEAD
    bool operator ==(const ID& other) const { return m_Id == other.m_Id; }
    bool operator !=(const ID& other) const { return m_Id != other.m_Id; }
    bool operator >(const ID& other) const { return m_Id > other.m_Id; }
    bool operator >=(const ID& other) const { return m_Id >= other.m_Id; }
    bool operator <(const ID& other) const { return m_Id < other.m_Id; }
    bool operator <=(const ID& other) const { return m_Id <= other.m_Id; }
=======
>>>>>>> b3521d9c72754a3e0e59649860349859e0810203

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