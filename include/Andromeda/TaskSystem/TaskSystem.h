#pragma once

#include "Andromeda/Misc/CoreMiscDefines.h"

#include "Tasks/Task.h"
#include "Andromeda/Editor/EditorWindow.h"

#include <mutex>
#include <condition_variable>
#include <queue>
#include <source_location>

namespace And
{
  /**
 * @struct WorkerCreationInfo
 * @brief Structure containing information for creating a worker thread.
 */
  struct WorkerCreationInfo
  {
    const char* Name;
    void* UserData;
    std::function<void(struct WorkerThreadData& Data)> Function;
  };

  /**
   * @struct WorkerThreadData
   * @brief Data structure representing the state of a worker thread.
   */
  struct WorkerThreadData
  {
    std::mutex QueueMutex;
    std::condition_variable ConditionVariable;
    std::queue<Task> TasksQueue;
    class TaskSystem* TaskSystemOwner;
    bool Stop;
    void* UserData;
  };

  /**
   * @class TaskSystem
   * @brief Manages a system of worker threads for executing tasks asynchronously.
   */
  class TaskSystem
  {
    NON_COPYABLE_CLASS(TaskSystem)
    NON_MOVABLE_CLASS(TaskSystem)
  public:
    /**
     * @brief Constructs a TaskSystem object.
     */
    TaskSystem();

    /**
     * @brief Destructor, cleans up resources associated with the TaskSystem.
     */
    ~TaskSystem();

    /**
     * @brief Adds a worker thread to the TaskSystem.
     * @param CreationInfo Information required for worker thread creation.
     */
    void AddWorker(const WorkerCreationInfo& CreationInfo);

    /**
     * @brief Adds a task with additional information for execution in the TaskSystem.
     * @param location Source code location where the task is added.
     * @param functionStr String representation of the function to be executed.
     * @param function Function to be executed.
     * @param args Arguments to be passed to the function.
     * @return A Future object representing the result of the task.
     */
    template<typename func_t, typename... args_t>
    auto AddTaskWithInfo(std::source_location location, const std::string& functionStr, func_t function, args_t... args) -> Future<decltype(function((MakeFuture(args).Get())...))>;

    /**
     * @brief Adds a task with additional information for execution in the TaskSystem in a specific thread.
     * @param location Source code location where the task is added.
     * @param functionStr String representation of the function to be executed.
     * @param ThreadName Name of the worker thread where the task should be executed.
     * @param function Function to be executed.
     * @param args Arguments to be passed to the function.
     * @return A Future object representing the result of the task.
     */
    template<typename func_t, typename... args_t>
    auto AddTaskWithInfo(std::source_location location, const std::string& functionStr, const std::string& ThreadName, func_t function, args_t... args) -> Future<decltype(function((MakeFuture(args).Get())...))>;

    /**
     * @brief Adds a task with additional information for execution in the TaskSystem in a specific thread.
     * @param location Source code location where the task is added.
     * @param functionStr String representation of the function to be executed.
     * @param ThreadId ID of the worker thread where the task should be executed.
     * @param function Function to be executed.
     * @param args Arguments to be passed to the function.
     * @return A Future object representing the result of the task.
     */
    template<typename func_t, typename... args_t>
    auto AddTaskWithInfo(std::source_location location, const std::string& functionStr, size_t ThreadId, func_t function, args_t... args) -> Future<decltype(function((MakeFuture(args).Get())...))>;

    /**
     * @brief Marks a task as resolved, providing an optional time parameter.
     * @param task The task to be marked as resolved.
     * @param time Optional duration time of the task resolution.
     */
    void MarkTaskAsResolved(const Task& task, float taskDurationTime = 0);

    std::shared_ptr<EditorWindow> GetEditorWindow() const;

  private:
    /**
     * @brief Adds a task to the TaskSystem.
     * @param location Source code location where the task is added.
     * @param functionStr String representation of the function to be executed.
     * @param ThreadId ID of the worker thread where the task should be executed.
     * @param function Function to be executed.
     * @param args Arguments to be passed to the function.
     * @return A Future object representing the result of the task.
     */
    template<typename func_t, typename... args_t>
    auto AddTask(std::source_location location, const std::string& functionStr, size_t ThreadId, func_t function, Future<args_t>... args) -> Future<decltype(function((args.Get())...))>;
    /**
     * @brief Adds a wait task to the TaskSystem.
     * @param task The wait task to be added.
     */
    void AddWaitTask(Task& task);
    /**
     * @brief Adds a task to a specific worker thread.
     * @param task The task to be added to the worker thread.
     * @param ThreadId ID of the worker thread where the task should be added.
     */
    void AddTaskToWorker(Task& task, size_t ThreadId);

    std::unique_ptr<struct TaskSystemData> m_Data; ///< Private data associated with the TaskSystem.
  };

  /**
   * @brief Retrieves a generic worker function for use in WorkerCreationInfo.
   * @return A function taking WorkerThreadData& as a parameter.
   */
  std::function<void(WorkerThreadData& Data)> GetGenericWorkerFunction();
}

#include "inl/TaskSystem.inl"

/**
 * @brief Macro for adding a task to the TaskSystem with source code location information.
 * @param function The function to be executed.
 * @param ...args Arguments to be passed to the function.
 */
#define AddTask(function, ...) AddTaskWithInfo(std::source_location::current(), #function, function, __VA_ARGS__)
/**
 * @brief Macro for adding a task to a specific worker thread with source code location information.
 * @param thread Name of the worker thread where the task should be executed.
 * @param function The function to be executed.
 * @param ...args Arguments to be passed to the function.
 */
#define AddTaskInThread(thread, function, ...) AddTaskWithInfo(std::source_location::current(), #function, thread, function, __VA_ARGS__)

