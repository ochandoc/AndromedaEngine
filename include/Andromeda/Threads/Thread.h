#pragma once

#include "Andromeda/Misc/CoreMiscDefines.h"

#include <functional>
#include <string>

namespace And
{
  /**
 * @struct ThreadCreationInfo
 * @brief Structure containing information for creating a new thread.
 */
  struct ThreadCreationInfo
  {
    const char* Name;
    void* UserData;
    std::function<void(class Thread&)> Function;
  };

  /**
   * @class Thread
   * @brief Represents a thread in the system.
   */
  class Thread final
  {
    NON_COPYABLE_CLASS(Thread)
    NON_MOVABLE_CLASS(Thread)
  public:
    /**
     * @brief Constructs a Thread object with the provided creation information.
     * @param CreationInfo Information required for thread creation.
     */
    explicit Thread(const ThreadCreationInfo& CreationInfo);
    /**
     * @brief Constructs a Thread object with the provided creation information and starts the thread.
     * @param CreationInfo Information required for thread creation.
     * @param EForceStart Enum indicating to forcefully start the thread.
     */
    Thread(const ThreadCreationInfo& CreationInfo, EForceStart);

    /**
     * @brief Destructor, cleans up resources associated with the thread.
     */
    ~Thread();

    /**
     * @brief Starts the thread execution.
     */
    void StartThread() const;

    /**
     * @brief Checks if the thread is joinable.
     * @return True if the thread is joinable, false otherwise.
     */
    bool Joinable() const;
    /**
     * @brief Joins the thread, blocking until it completes.
     */
    void Join();

    /**
     * @brief Gets user data associated with the thread.
     * @return A pointer to the user data.
     */
    void* GetData();
    /**
     * @brief Gets const user data associated with the thread.
     * @return A const pointer to the user data.
     */
    const void* GetData() const;

    /**
     * @brief Gets the name of the thread.
     * @return A const pointer to the thread name.
     */
    const char* GetName() const;
    /**
     * @brief Gets the ID of the thread.
     * @return The ID of the thread.
     */
    size_t GetId() const;

    /**
     * @brief Gets the ID of the main thread.
     * @return The ID of the main thread.
     */
    static size_t GetMainThreadId();
    /**
     * @brief Gets the ID of the currently executing thread.
     * @return The ID of the currently executing thread.
     */
    static size_t GetCurrentThreadId();

    /**
     * @brief Gets the ID of a thread by its name.
     * @param Name The name of the thread.
     * @return The ID of the thread with the specified name.
     */
    static size_t GetThreadIdByName(const std::string& Name);

    /**
     * @brief Gets the number of active threads.
     * @return The number of active threads.
     */
    static int GetNumActiveThreads();

  private:
    std::unique_ptr<struct ThreadData> m_Data;
  };
}