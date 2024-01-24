#include "Andromeda/Threads/Thread.h"

#include <string>

#include <thread>
#include <condition_variable>

namespace And
{
  struct ThreadData
  {
    /** The id of the parent thread */
    size_t ParentId;

    /** The name of the thread */
    std::string Name;
    /** The id of the thread */
    size_t Id;

    /** The user function is going to be executed in the thread */
    std::function<void(Thread&)> UserFunction;
    /** The user data */
    void* UserData;

    /** Store if the thread is running */
    volatile bool Running;
    /** Store if the thread can start running the user function */
    volatile bool CanStart;
    /** Condition variable to wait to start */
    std::condition_variable StartCondition;
    /** The thread handle */
    std::unique_ptr<std::thread> Thread;
  };

  static struct
  {
    struct
    {
      std::string Name = "Main Thread";
      size_t Id = std::hash<std::thread::id>{}(std::this_thread::get_id());
    } MainThread;

    int NumThreads = 0;
    int NumActiveThreads = 0;

    std::mutex Mutex;
    std::unordered_map<std::string, size_t> ThreadIds;
  } ThreadGlobals;

  Thread::Thread(const ThreadCreationInfo& CreationInfo)
  {
    /** Alloc the data of the thread */
    m_Data = new ThreadData;

    /** Set the id of the parent */
    m_Data->ParentId = GetCurrentThreadId();

    /** Set the info of the thread */
    m_Data->Name = CreationInfo.Name;
    m_Data->UserFunction = CreationInfo.Function;
    m_Data->UserData = CreationInfo.UserData;
    m_Data->CanStart = true;
    m_Data->Running = false;

    /** Set the function of the thread */
    auto ThreadFunction = [this]()
      {
        {
          std::lock_guard<std::mutex> lock(ThreadGlobals.Mutex);
          ThreadGlobals.NumThreads++;
        }
        {
          std::mutex StartMutex;
          std::unique_lock<std::mutex> lock(StartMutex);
          /** Wait for start */
          m_Data->StartCondition.wait(lock, [this]() { return m_Data->Running || !m_Data->CanStart; });
          /** if can't start exit the thread */
          if (!m_Data->CanStart) { m_Data->Running = false; return; }
          ThreadGlobals.Mutex.lock();
          ThreadGlobals.NumActiveThreads++;
          ThreadGlobals.Mutex.unlock();
          /** Set the values of a running thread */
          m_Data->Running = true;
          m_Data->CanStart = false;
        }

        try
        {
          m_Data->UserFunction(*this);
        }
        catch (std::exception& exception)
        {
          printf("Error: %s\n", exception.what());
        }
        catch (std::string& msg)
        {
          printf("Error: %s\n", msg.c_str());
        }

        m_Data->Running = false;
        {
          std::lock_guard<std::mutex> lock(ThreadGlobals.Mutex);
          ThreadGlobals.NumThreads--;
          ThreadGlobals.NumActiveThreads--;
          ThreadGlobals.ThreadIds.erase(m_Data->Name);
        }
      };

    /** Create the thread and store the id */
    m_Data->Thread = std::make_unique<std::thread>(ThreadFunction);
    m_Data->Id = std::hash<std::thread::id>{}(m_Data->Thread->get_id());

    {
      std::lock_guard<std::mutex> lock(ThreadGlobals.Mutex);
      ThreadGlobals.ThreadIds.insert({ m_Data->Name, m_Data->Id });
    }

    printf("------------------------------------------------\n");
    printf("Thread Created:\n");
    printf("\tParent Thread Id: %zu\n", m_Data->ParentId);
    printf("\tThread Name: %s\n", m_Data->Name.c_str());
    printf("\tThread Id: %zu\n", m_Data->Id);
    printf("------------------------------------------------\n");
  }

  Thread::Thread(const ThreadCreationInfo& CreationInfo, EForceStart) : Thread(CreationInfo)
  {
    /** Start the thread thread and wait */
    StartThread();
  }

  Thread::~Thread()
  {
    /** Wait for the thread end */
    if (Joinable())
      Join();

    /** Free the data */
    delete m_Data;
  }

  void* Thread::GetData()
  {
    return m_Data->UserData;
  }

  const void* Thread::GetData() const
  {
    return m_Data->UserData;
  }

  const char* Thread::GetName() const
  {
    return m_Data->Name.c_str();
  }

  size_t Thread::GetId() const
  {
    return m_Data->Id;
  }

  bool Thread::Joinable() const
  {
    size_t CurrentThreadId = GetCurrentThreadId();
    if (m_Data->Id == CurrentThreadId)
      return false;

    if (!m_Data->Running)
      return true;

    return m_Data->Thread->joinable();
  }

  void Thread::Join()
  {
    size_t CurrentThreadId = GetCurrentThreadId();
    if (m_Data->Id == CurrentThreadId)
      return;

    if (!m_Data->Running && m_Data->CanStart)
    {
      m_Data->CanStart = false;
      m_Data->StartCondition.notify_all();
      printf("------------------------------------------------\n");
      printf("Thread Destroyed:\n");
      printf("\tParent Thread Id: %zu\n", m_Data->ParentId);
      printf("\tThread Name: %s\n", m_Data->Name.c_str());
      printf("\tThread Id: %zu\n", m_Data->Id);
      printf("------------------------------------------------\n");
    }

    if (m_Data->Thread->joinable())
    {
      m_Data->Thread->join();
      printf("------------------------------------------------\n");
      printf("Thread Destroyed:\n");
      printf("\tParent Thread Id: %zu\n", m_Data->ParentId);
      printf("\tThread Name: %s\n", m_Data->Name.c_str());
      printf("\tThread Id: %zu\n", m_Data->Id);
      printf("------------------------------------------------\n");
    }
  }

  void Thread::StartThread() const
  {
    if (!m_Data->Running && m_Data->CanStart)
    {
      m_Data->Running = true;
      m_Data->StartCondition.notify_all();
    }
  }

  size_t Thread::GetMainThreadId()
  {
    return ThreadGlobals.MainThread.Id;
  }

  size_t Thread::GetCurrentThreadId()
  {
    return std::hash<std::thread::id>{}(std::this_thread::get_id());
  }

  size_t Thread::GetThreadIdByName(const std::string& Name)
  {
    std::lock_guard<std::mutex> lock(ThreadGlobals.Mutex);
    if (ThreadGlobals.ThreadIds.contains(Name))
      return ThreadGlobals.ThreadIds[Name];

    return 0;
  }

  int Thread::GetNumActiveThreads()
  {
    return ThreadGlobals.NumActiveThreads;
  }
}

