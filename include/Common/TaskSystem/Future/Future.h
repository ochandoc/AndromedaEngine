#pragma once

#include "Common/Misc/CoreMiscDefines.h"

#include <future>

namespace And
{
  /**
 * @class IAvailabilityBase
 * @brief Interface for the base class defining the availability of a future result.
 */
  class IAvailabilityBase
  {
    INTERFACE_BODY(IAvailabilityBase);
  public:
    /**
     * @brief Checks if the future result is available.
     * @return True if the future result is available, false otherwise.
     */
    virtual bool IsAvailable() const = 0;
  };

  /**
   * @class IFutureBase
   * @brief Interface for the base class defining common operations on futures.
   * @tparam T Type of the future result.
   */
  template<typename T>
  class IFutureBase : public IAvailabilityBase
  {

    INTERFACE_BODY(IFutureBase);
  public:
    /**
     * @brief Gets the future result.
     * @return The future result.
     */
    virtual T Get() = 0;
    /**
     * @brief Waits for the future result to become available.
     */
    virtual void Wait() const = 0;
  };

  /**
   * @class ResolvedFuture
   * @brief Represents a resolved future with a predefined value.
   * @tparam T Type of the future result.
   */
  template<typename T>
  class ResolvedFuture : public IFutureBase<T>
  {
  public:
    /**
     * @brief Constructs a ResolvedFuture with a predefined value.
     * @param value The predefined value of the future result.
     */
    explicit ResolvedFuture(T value) : m_Value(value) {}
    /**
     * @brief Destructor, default implementation.
     */
    virtual ~ResolvedFuture() = default;

    /**
     * @brief Gets the predefined value of the future result.
     * @return The predefined value of the future result.
     */
    virtual T Get() override { return m_Value; }
    /**
     * @brief Checks if the future result is available (always true for resolved futures).
     * @return Always returns true.
     */
    virtual bool IsAvailable() const { return true; }

    /**
     * @brief Waits for the future result (no waiting needed for resolved futures).
     */
    virtual void Wait() const override {}
  private:
    T m_Value; ///< The predefined value of the future result.
  };

  /**
   * @class TaskFuture
   * @brief Represents a future associated with an asynchronous task.
   * @tparam T Type of the future result.
   */
  template<typename T>
  class TaskFuture : public IFutureBase<T>
  {
  public:
    /**
     * @brief Constructs a TaskFuture with a std::future object.
     * @param future The std::future object associated with the asynchronous task.
     */
    TaskFuture(std::future<T>& future) { m_Future = std::move(future); }
    /**
     * @brief Destructor, default implementation.
     */
    virtual ~TaskFuture() = default;

    /**
     * @brief Gets the result of the asynchronous task.
     * @return The result of the asynchronous task.
     */
    virtual T Get() override { return m_Future.get(); }
    /**
     * @brief Checks if the future result is available.
     * @return True if the future result is available, false otherwise.
     */
    virtual bool IsAvailable() const override
    {
      return m_Future.wait_for(std::chrono::microseconds(0)) == std::future_status::ready;
    }

    /**
     * @brief Waits for the future result to become available.
     */
    virtual void Wait() const override { m_Future.wait(); }
  private:
    std::future<T> m_Future; ///< The std::future object associated with the asynchronous task.
  };

  /**
   * @class Future
   * @brief Represents a generic future that can be either resolved or associated with an asynchronous task.
   * @tparam T Type of the future result.
   */
  template<typename T>
  class Future
  {
  private:
    /**
     * @brief Private default constructor to prevent instantiation without initialization.
     */
    Future() = default;

  public:
    using type = T; ///< Type alias for the future result type.
    /**
     * @brief Constructs a Future with a predefined value.
     * @param value The predefined value of the future result.
     */
    Future(const T& value) { m_FutureBase = std::make_shared<ResolvedFuture<T>>(value); m_Id = (size_t)m_FutureBase.get(); }
    /**
     * @brief Constructs a Future with a std::future object representing an asynchronous task.
     * @param future The std::future object associated with the asynchronous task.
     */
    Future(std::future<T>&& future) { m_FutureBase = std::make_shared<TaskFuture<T>>(future); m_Id = (size_t)m_FutureBase.get(); }
    /**
     * @brief Copy constructor for creating a copy of another Future object.
     * @param other The Future object to be copied.
     */
    Future(const Future& other) { m_FutureBase = other.m_FutureBase; m_Id = other.m_Id; }
    /**
     * @brief Move constructor for moving the content of another Future object.
     * @param other The Future object to be moved.
     */
    Future(Future&& other) { m_FutureBase = std::move(other.m_FutureBase); std::swap(m_Id, other.m_Id); }

    /**
     * @brief Destructor, default implementation.
     */
    ~Future() = default;

    /**
     * @brief Copy assignment operator for assigning the content of another Future object.
     * @param other The Future object to be copied.
     * @return Reference to the assigned Future object.
     */
    Future& operator=(const Future& other) { if (this != &other) { m_FutureBase = other.m_FutureBase; m_Id = other.m_Id; } return *this; }
    /**
     * @brief Move assignment operator for moving the content of another Future object.
     * @param other The Future object to be moved.
     * @return Reference to the assigned Future object.
     */
    Future& operator=(Future&& other) { if (this != &other) { m_FutureBase = std::move(other.m_FutureBase); std::swap(m_Id, other.m_Id); } return *this; }

    /**
     * @brief Gets the value associated with the future.
     * @return The value of the future.
     */
    T Get() const
    {
      return m_FutureBase->Get();
    }

    /**
     * @brief Checks if the future is available.
     * @return True if the future is available, false otherwise.
     */
    bool IsAvailable() const
    {
      return m_FutureBase->IsAvailable();
    }

    /**
     * @brief Waits for the future to be ready.
     */
    void Wait() const
    {
      m_FutureBase->Wait();
    }

    /**
     * @brief Gets the unique identifier of the future.
     * @return The unique identifier of the future.
     */
    inline size_t GetId() const { return m_Id; }

    /**
     * @brief Friend class declaration to allow FutureAvailability access to private members.
     */
    friend class FutureAvailability;
  private:
    size_t m_Id; ///< Unique identifier of the future.
    std::shared_ptr<IFutureBase<T>> m_FutureBase; ///< Shared pointer to the base class of the future.
  };

  /**
   * @class Future
   * @brief Represents a generic future that can be either resolved or associated with an asynchronous task.
   * @tparam T Type of the future result.
   */
  template<>
  class Future<void>
  {
  private:
    /**
     * @brief Private default constructor to prevent instantiation without initialization.
     */
    Future() = default;

  public:
    using type = void; ///< Type alias for the future result type.

    /**
     * @brief Constructs a Future with a std::future object representing an asynchronous task.
     * @param future The std::future object associated with the asynchronous task.
     */
    Future(std::future<void>&& future) { m_FutureBase = std::make_shared<TaskFuture<void>>(future); m_Id = (size_t)m_FutureBase.get(); }
    /**
     * @brief Copy constructor for creating a copy of another Future object.
     * @param other The Future object to be copied.
     */
    Future(const Future& other) { m_FutureBase = other.m_FutureBase; m_Id = other.m_Id; }
    /**
     * @brief Move constructor for moving the content of another Future object.
     * @param other The Future object to be moved.
     */
    Future(Future&& other) { m_FutureBase = std::move(other.m_FutureBase); std::swap(m_Id, other.m_Id); }

    /**
     * @brief Destructor, default implementation.
     */
    ~Future() = default;

    /**
     * @brief Copy assignment operator for assigning the content of another Future object.
     * @param other The Future object to be copied.
     * @return Reference to the assigned Future object.
     */
    Future& operator=(const Future& other) { if (this != &other) { m_FutureBase = other.m_FutureBase; m_Id = other.m_Id; } return *this; }
    /**
     * @brief Move assignment operator for moving the content of another Future object.
     * @param other The Future object to be moved.
     * @return Reference to the assigned Future object.
     */
    Future& operator=(Future&& other) { if (this != &other) { m_FutureBase = std::move(other.m_FutureBase); std::swap(m_Id, other.m_Id); } return *this; }

    /**
     * @brief Gets the value associated with the future.
     * @return The value of the future.
     */
    void Get() const
    {
      return m_FutureBase->Get();
    }

    /**
     * @brief Checks if the future is available.
     * @return True if the future is available, false otherwise.
     */
    bool IsAvailable() const
    {
      return m_FutureBase->IsAvailable();
    }

    /**
     * @brief Waits for the future to be ready.
     */
    void Wait() const
    {
      m_FutureBase->Wait();
    }

    /**
     * @brief Gets the unique identifier of the future.
     * @return The unique identifier of the future.
     */
    inline size_t GetId() const { return m_Id; }

    /**
     * @brief Friend class declaration to allow FutureAvailability access to private members.
     */
    friend class FutureAvailability;
  private:
    size_t m_Id; ///< Unique identifier of the future.
    std::shared_ptr<IFutureBase<void>> m_FutureBase; ///< Shared pointer to the base class of the future.
  };

  /**
   * @brief Constructs a Future with a specified value.
   * @tparam T Type of the future.
   * @param value The resolved value of the future.
   * @return The constructed Future object.
   */
  template<typename T>
  Future<T> MakeFuture(Future<T>& future);

  /**
   * @brief Constructs a Future based on another Future.
   * @tparam T Type of the future.
   * @param future The source Future.
   * @return The constructed Future object.
   */
  template<typename T>
  Future<T> MakeFuture(const T& value);
}

#include "inl/Future.inl"