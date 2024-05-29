#pragma once

#include "Andromeda/Misc/CoreMiscDefines.h"
#include "Andromeda/HAL/Types.h"

namespace And
{
  /**
   * @class ID
   * @brief Represents a unique identifier.
   *
   * This class encapsulates a unique identifier and provides comparison operators.
   */
  class ID
  {
  public:
    /**
     * @brief Default constructor.
     *
     * Initializes the ID with a default value.
     */
    ID();
    /**
     * @brief Constructs an ID with the given value.
     * @param value The value of the ID.
     */
    ID(uint64 value);
    /// Copy constructor.
    ID(const ID& other);
    /// Move constructor.
    ID(ID&& other);

    /// Destructor.
    ~ID() = default;

    /// Copy assignment operator.
    ID& operator =(const ID& other);
    /// Move assignment operator.
    ID& operator =(ID&& other);

    /**
     * @brief Retrieves the value of the ID.
     * @return The value of the ID.
     */
    uint64 get() const { return m_Id; }

    /**
     * @brief Implicit conversion to uint64.
     * @return The value of the ID as a uint64.
     */
    operator uint64() const { return m_Id; }
    /// Equality comparison operator.
    bool operator ==(const ID& other) const { return m_Id == other.m_Id; }
    /// Inequality comparison operator.
    bool operator !=(const ID& other) const { return m_Id != other.m_Id; }
    /// Greater than comparison operator.
    bool operator >(const ID& other) const { return m_Id > other.m_Id; }
    /// Greater than or equal to comparison operator.
    bool operator >=(const ID& other) const { return m_Id >= other.m_Id; }
    /// Less than comparison operator.
    bool operator <(const ID& other) const { return m_Id < other.m_Id; }
    /// Less than or equal to comparison operator.
    bool operator <=(const ID& other) const { return m_Id <= other.m_Id; }

  private:
    uint64 m_Id;  /**< The value of the ID. */
  };
}

namespace std
{
  /**
   * @brief Specialization of std::hash for ID.
   *
   * This specialization allows ID objects to be used as keys in hash-based containers.
   */
  template<>
  struct hash<And::ID>
  {
    /**
     * @brief Hash function for ID.
     * @param id The ID object to hash.
     * @return The hash value of the ID.
     */
    std::size_t operator()(const And::ID& id) const
    {
      return id.get();
    }
  };

}