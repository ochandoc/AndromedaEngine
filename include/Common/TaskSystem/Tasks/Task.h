#pragma once

#include "Common/Misc/CoreMiscDefines.h"

#include "Common/Threads/Thread.h"

#include "Common/TaskSystem/Tasks/TaskFunction.h"
#include "Common/TaskSystem/Future/FutureAvailability.h"

#include <functional>
#include <memory>
#include <source_location>

namespace And
{
  class TaskSystem;

  /**
   * @class Task
   * @brief Represents a task in the TaskSystem, encapsulating a function to be executed asynchronously.
   */
  class Task
  {
  private:
    /**
     * @brief Private constructor used by TaskSystem for creating tasks.
     * @param DesiredThreadId The ID of the thread where the task is intended to be executed.
     * @param location Source code location where the task is created.
     * @param functionStr String representation of the function to be executed.
     */
    explicit Task(size_t DesiredThreadId, std::source_location location, const std::string& functionStr);

  public:
    /**
     * @brief Default constructor with no initialization.
     * @param ENoInit Enumeration indicating no initialization.
     */
    explicit Task(ENoInit);
    /**
     * @brief Copy constructor for creating a copy of another Task object.
     * @param other The Task object to be copied.
     */
    Task(const Task& other);
    /**
     * @brief Move constructor for moving the content of another Task object.
     * @param other The Task object to be moved.
     */
    Task(Task&& other);

    /**
     * @brief Destructor, cleans up resources associated with the Task.
     */
    ~Task();

    /**
     * @brief Copy assignment operator for assigning the content of another Task object.
     * @param other The Task object to be copied.
     * @return Reference to the assigned Task object.
     */
    Task& operator=(const Task& other);
    /**
     * @brief Move assignment operator for moving the content of another Task object.
     * @param other The Task object to be moved.
     * @return Reference to the assigned Task object.
     */
    Task& operator=(Task&& other);

    /**
     * @brief Checks if the task can be executed.
     * @return True if the task can be executed, false otherwise.
     */
    bool CanBeExecuted() const;

    /**
     * @brief Executes the task by calling its associated function.
     */
    void operator()();

    /**
     * @brief Gets the availability status of the associated future(s).
     * @return The FutureAvailability status.
     */
    FutureAvailability GetFutureAvailability() const;

    /**
     * @brief Checks if the task is waiting for a specific future.
     * @param FutureId Identifier of the future.
     * @return True if the task is waiting for the specified future, false otherwise.
     */
    bool IsWaitingFuture(size_t FutureId) const;

    /**
     * @brief Gets the ID of the task.
     * @return The ID of the task.
     */
    inline size_t GetId() const { return m_Id; }

    /**
     * @brief Gets the ID of the thread where the task is executed.
     * @return The ID of the thread where the task is executed.
     */
    inline size_t GetExecutionThreadId() const { return m_ExecutionThreadId; }
    /**
     * @brief Gets the desired execution thread ID specified during task creation.
     * @return The desired execution thread ID.
     */
    inline size_t GetDesiredExecutionThreadId() const { return m_DesiredThreadId; }

    /**
     * @brief Friend class declaration to allow TaskSystem access to private members.
     */
    friend class TaskSystem;
  private:
    size_t m_ExecutionThreadId; ///< ID of the thread where the task is executed.
    size_t m_DesiredThreadId; ///< Desired execution thread ID specified during task creation.
    size_t m_Id; ///< ID of the task.
    std::shared_ptr<ITaskFunctionBase> m_TaskFunction; ///< Shared pointer to the task function.
    std::source_location m_SourceLocation; ///< Source code location where the task is created.
    std::string m_FunctionString; ///< String representation of the function to be executed.
  };
}