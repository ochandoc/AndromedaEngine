#pragma once

#include "Common/Misc/CoreMiscDefines.h"
#include "Common/TaskSystem/Future/Future.h"
#include "Common/TaskSystem/Future/FutureAvailability.h"

#include <functional>
#include <future>
#include <tuple>

namespace And
{
  /**
 * @class ITaskFunctionBase
 * @brief Interface for task function objects in the task system.
 */
  class ITaskFunctionBase
  {
    INTERFACE_BODY(ITaskFunctionBase)
  public:
    /**
     * @brief Checks if the task function can be called.
     * @return True if the task function can be called, false otherwise.
     */
    virtual bool CanBeCalled() const = 0;
    /**
     * @brief Executes the task function.
     */
    virtual void operator()() = 0;
    /**
     * @brief Checks if the task function is waiting for a specific future.
     * @param FutureId Identifier of the future.
     * @return True if the task function is waiting for the specified future, false otherwise.
     */
    virtual bool IsWaitingFuture(size_t FutureId) const = 0;
    /**
     * @brief Gets the availability status of the associated future(s).
     * @return The FutureAvailability status.
     */
    virtual FutureAvailability GetFutureAvailability() = 0;
  };

  template<typename func_t>
  class TaskFunction;


  /**
   * @class TaskFunction
   * @brief Represents a task function with associated future(s).
   * @tparam ret_t Return type of the task function.
   * @tparam args_t Argument types of the task function.
   */
  template<typename ret_t, typename... args_t>
  class TaskFunction<ret_t(args_t...)> : public ITaskFunctionBase
  {
  public:
    /**
     * @brief Constructs a TaskFunction object with a packaged task and associated futures.
     * @param task Packaged task containing the task function.
     * @param args Futures representing the arguments of the task function.
     */
    explicit TaskFunction(std::packaged_task<ret_t(args_t...)>& task, Future<args_t>... args);

    /**
     * @brief Gets the future associated with the return value of the task function.
     * @return The Future object representing the return value.
     */
    Future<ret_t> GetFuture();

    /**
     * @brief Executes the task function.
     */
    virtual void operator()() override;

    /**
     * @brief Checks if the task function can be called.
     * @return True if the task function can be called, false otherwise.
     */
    virtual bool CanBeCalled() const override;

    /**
     * @brief Gets the availability status of the associated future(s).
     * @return The FutureAvailability status.
     */
    virtual FutureAvailability GetFutureAvailability() override;

    /**
     * @brief Checks if the task function is waiting for a specific future.
     * @param FutureId Identifier of the future.
     * @return True if the task function is waiting for the specified future, false otherwise.
     */
    virtual bool IsWaitingFuture(size_t FutureId) const;

  private:
    /**
     * @brief Checks if all associated futures are available.
     * @param int_seq Integer sequence for parameter pack expansion.
     * @return True if all futures are available, false otherwise.
     */
    template<int... ints>
    bool AreAllFutureAvailable(std::integer_sequence<int, ints...> int_seq) const;

    /**
     * @brief Calls the task function with associated arguments.
     * @param int_seq Integer sequence for parameter pack expansion.
     */
    template<int... ints>
    void CallFunction(std::integer_sequence<int, ints...> int_seq);

    /**
     * @brief Checks if the task function is waiting for a specific future.
     * @param int_seq Integer sequence for parameter pack expansion.
     * @param FutureId Identifier of the future.
     * @return True if the task function is waiting for the specified future, false otherwise.
     */
    template<int... ints>
    bool CheckWaitingFutures(std::integer_sequence<int, ints...> int_seq, size_t FutureId) const;

    /**
     * @brief Checks if a specific future is the same as the one identified by FutureId.
     * @tparam T Type of the future.
     * @param future The future to compare.
     * @param FutureId Identifier of the future to compare against.
     * @return True if the futures are the same, false otherwise.
     */
    template<typename T>
    bool IsSameFuture(const Future<T>& future, size_t FutureId) const;

    std::packaged_task<ret_t(args_t...)> m_Function; ///< Packaged task containing the task function.
    std::tuple<Future<args_t>...> m_Args; ///< Tuple of futures representing the arguments of the task function.
    Future<ret_t> m_Future; ///< Future representing the return value of the task function.
  };
}

#include "inl/TaskFunction.inl"