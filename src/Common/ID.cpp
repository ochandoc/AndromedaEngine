#include "Common/ID.h"

#include <random>

namespace And
{
  ID::ID()
  {
    static std::random_device s_RandomDevice;
    static std::mt19937_64 s_Engine(s_RandomDevice());
    static std::uniform_int_distribution<uint64> s_UniformDistribution;

    m_Id = s_UniformDistribution(s_Engine);
  }

  ID::ID(uint64 value)
  {
    m_Id = value;
  }

  ID::ID(const ID& other)
  {
    m_Id = other.m_Id;
  }

  ID::ID(ID&& other)
  {
    m_Id = other.m_Id;
    other.m_Id = 0;
  }

  ID& ID::operator =(const ID& other)
  {
    if (this != &other)
    {
      m_Id = other.m_Id;
    }
    return *this;
  }

  ID& ID::operator =(ID&& other)
  {
    if (this != &other)
    {
      std::swap(m_Id, other.m_Id);
    }
    return *this;
  }
}