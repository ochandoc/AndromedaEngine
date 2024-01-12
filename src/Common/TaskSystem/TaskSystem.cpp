#include "Common/TaskSystem/TaskSystem.h"

namespace And
{
  struct WorkerThreadInternalData
  {
    std::unique_ptr<WorkerThreadData> Data;
    std::unique_ptr<Thread> Handle;
    std::function<void(struct WorkerThreadData& Data)> Function;
  };

  struct TaskSystemData
  {
    std::vector<std::unique_ptr<WorkerThreadInternalData>> Threads;
    std::vector<Task> WaitingTasks;
    std::mutex Mutex;
  };

  TaskSystem::TaskSystem() : m_Data(new TaskSystemData)
  {
  }

  TaskSystem::~TaskSystem()
  {
    for (auto& thread : m_Data->Threads)
    {
      thread->Data->Stop = true;
      thread->Data->ConditionVariable.notify_all();
    }
    for (auto& thread : m_Data->Threads)
    {
      if (thread->Handle->Joinable())
        thread->Handle->Join();
    }
  }

  void TaskSystem::AddWorker(const WorkerCreationInfo& CreationInfo)
  {
    std::lock_guard<std::mutex> lock(m_Data->Mutex);
    m_Data->Threads.push_back(std::make_unique<WorkerThreadInternalData>());

    WorkerThreadInternalData& internalData = *m_Data->Threads.back();

    internalData.Data = std::make_unique<WorkerThreadData>();
    internalData.Data->UserData = CreationInfo.UserData;
    internalData.Data->Stop = false;
    internalData.Function = CreationInfo.Function;
    internalData.Data->TaskSystemOwner = this;

    ThreadCreationInfo threadCreationInfo;
    threadCreationInfo.Name = CreationInfo.Name;
    threadCreationInfo.UserData = &internalData;

    auto threadFunction = [](Thread& thread)
      {
        WorkerThreadInternalData* internalData = static_cast<WorkerThreadInternalData*>(thread.GetData());
        WorkerThreadData* workerThreadData = static_cast<WorkerThreadData*>(internalData->Data.get());
        internalData->Function(*workerThreadData);
      };

    threadCreationInfo.Function = threadFunction;
    internalData.Handle = std::make_unique<Thread>(threadCreationInfo, ForceStart);
  }

  void TaskSystem::MarkTaskAsResolved(const Task& task, float time)
  {
    std::lock_guard<std::mutex> lock(m_Data->Mutex);
    size_t FutureId = task.GetFutureAvailability().GetId();
    for (int i = 0; i < m_Data->WaitingTasks.size(); i++)
    {
      m_Data->WaitingTasks[i].CanBeExecuted();
      if (m_Data->WaitingTasks[i].IsWaitingFuture(FutureId))
      {
        if (m_Data->WaitingTasks[i].CanBeExecuted())
        {
          Task waitingTask = std::move(m_Data->WaitingTasks[i]);
          m_Data->WaitingTasks.erase(m_Data->WaitingTasks.begin() + i);

          size_t DesiredThread = waitingTask.GetDesiredExecutionThreadId();

          if (DesiredThread == 0)
            DesiredThread = Thread::GetCurrentThreadId();

          AddTaskToWorker(waitingTask, DesiredThread);
        }
      }
    }
  }

  void TaskSystem::AddWaitTask(Task& task)
  {
    std::lock_guard<std::mutex> lock(m_Data->Mutex);
    m_Data->WaitingTasks.push_back(task);
  }

  void TaskSystem::AddTaskToWorker(Task& task, size_t ThreadId)
  {
    bool bInserted = false;

    if (ThreadId != 0)
    {
      for (int i = 0; i < m_Data->Threads.size() && !bInserted; i++)
      {
        if (m_Data->Threads[i]->Handle->GetId() == ThreadId)
        {
          {
            std::lock_guard<std::mutex> lock(m_Data->Threads[i]->Data->QueueMutex);
            m_Data->Threads[i]->Data->TasksQueue.push(std::move(task));
          }
          m_Data->Threads[i]->Data->ConditionVariable.notify_all();
          bInserted = true;
        }
      }
    }
    else
    {
      size_t Index = 0;
      size_t NumTasks = -1;
      for (int i = 0; i < m_Data->Threads.size(); i++)
      {
        std::lock_guard<std::mutex> lock(m_Data->Threads[i]->Data->QueueMutex);
        if (m_Data->Threads[i]->Data->TasksQueue.size() < NumTasks)
        {
          Index = i;
          NumTasks = m_Data->Threads[i]->Data->TasksQueue.size();
        }
      }

      {
        std::lock_guard<std::mutex> lock(m_Data->Threads[Index]->Data->QueueMutex);
        m_Data->Threads[Index]->Data->TasksQueue.push(std::move(task));
      }
      m_Data->Threads[Index]->Data->ConditionVariable.notify_all();
    }
  }

  std::function<void(struct WorkerThreadData& Data)> GetGenericWorkerFunction()
  {
    std::function<void(WorkerThreadData& Data)> function = [](WorkerThreadData& Data)
      {
        while (true)
        {
          Task task(NoInit);
          {
            std::unique_lock<std::mutex> lock(Data.QueueMutex);
            Data.ConditionVariable.wait(lock, [&Data]() { return !Data.TasksQueue.empty() || Data.Stop; });
            if (Data.TasksQueue.empty() && Data.Stop) return;
            task = std::move(Data.TasksQueue.front());
            Data.TasksQueue.pop();
          }
          auto start = std::chrono::system_clock::now();
          task();
          auto endt = std::chrono::system_clock::now();

          Data.TaskSystemOwner->MarkTaskAsResolved(task, std::chrono::duration<float>(endt - start).count());
        }
      };
    return function;
  }
}

