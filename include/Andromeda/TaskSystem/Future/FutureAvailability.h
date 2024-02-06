#pragma once

#include "Andromeda/TaskSystem/Future/Future.h"

#include <memory>

namespace And
{
  /**
 * @class FutureAvailability
 * @brief Represents the availability status of a Future in the TaskSystem.
 */
  class FutureAvailability
  {
  public:
    /**
     * @brief Constructs a FutureAvailability object based on a specified Future.
     * @tparam T Type of the Future.
     * @param f The Future from which availability information is derived.
     */
    template<typename T>
    FutureAvailability(Future<T>& f) { AvailabilityBase = f.m_FutureBase; Id = f.m_Id; }
    /**
     * @brief Copy constructor for creating a copy of another FutureAvailability object.
     * @param other The FutureAvailability object to be copied.
     */
    FutureAvailability(const FutureAvailability& other);
    /**
     * @brief Move constructor for moving the content of another FutureAvailability object.
     * @param other The FutureAvailability object to be moved.
     */
    FutureAvailability(FutureAvailability&& other);

    /**
     * @brief Destructor, default implementation.
     */
    ~FutureAvailability() = default;

    /**
     * @brief Copy assignment operator for assigning the content of another FutureAvailability object.
     * @param other The FutureAvailability object to be copied.
     * @return Reference to the assigned FutureAvailability object.
     */
    FutureAvailability& operator=(const FutureAvailability& other);
    /**
     * @brief Move assignment operator for moving the content of another FutureAvailability object.
     * @param other The FutureAvailability object to be moved.
     * @return Reference to the assigned FutureAvailability object.
     */
    FutureAvailability& operator=(FutureAvailability&& other);

    /**
     * @brief Checks if the associated Future is available.
     * @return True if the Future is available, false otherwise.
     */
    bool IsAvailable() const;

    /**
     * @brief Gets the ID of the associated Future.
     * @return The ID of the associated Future.
     */
    size_t GetId() const;
  private:
    std::shared_ptr<IAvailabilityBase> AvailabilityBase; ///< Shared pointer to the availability base of the Future.
    size_t Id; ///< ID of the associated Future.
  };
}