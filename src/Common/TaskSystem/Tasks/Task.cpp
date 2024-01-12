#include "Common/TaskSystem/Tasks/Task.h"

namespace And
{
  Task::Task(ENoInit) : m_DesiredThreadId(0), m_ExecutionThreadId(0)
  {

  }

  Task::Task(size_t DesiredThreadId, std::source_location location, const std::string& functionStr) : m_DesiredThreadId(DesiredThreadId), m_ExecutionThreadId(0), m_SourceLocation(location), m_FunctionString(functionStr)
  {
  }

  Task::Task(const Task& other) : m_DesiredThreadId(0)
  {
    m_DesiredThreadId = other.m_DesiredThreadId;
    m_ExecutionThreadId = other.m_ExecutionThreadId;
    m_TaskFunction = other.m_TaskFunction;
    m_SourceLocation = other.m_SourceLocation;
    m_FunctionString = other.m_FunctionString;
    m_Id = other.m_Id;
  }

  Task::Task(Task&& other) : m_DesiredThreadId(0)
  {
    std::swap(m_DesiredThreadId, other.m_DesiredThreadId);
    std::swap(m_ExecutionThreadId, other.m_ExecutionThreadId);
    m_TaskFunction = std::move(other.m_TaskFunction);
    std::swap(m_Id, other.m_Id);
    std::swap(m_SourceLocation, other.m_SourceLocation);
    std::swap(m_FunctionString, other.m_FunctionString);
  }

  Task::~Task()
  {

  }

  Task& Task::operator=(const Task& other)
  {
    if (this != &other)
    {
      m_DesiredThreadId = other.m_DesiredThreadId;
      m_ExecutionThreadId = other.m_ExecutionThreadId;
      m_TaskFunction = other.m_TaskFunction;
      m_Id = other.m_Id;
      m_SourceLocation = other.m_SourceLocation;
      m_FunctionString = other.m_FunctionString;
    }
    return *this;
  }

  Task& Task::operator=(Task&& other)
  {
    if (this != &other)
    {
      std::swap(m_DesiredThreadId, other.m_DesiredThreadId);
      std::swap(m_ExecutionThreadId, other.m_ExecutionThreadId);
      std::swap(m_TaskFunction, other.m_TaskFunction);
      std::swap(m_Id, other.m_Id);
      std::swap(m_SourceLocation, other.m_SourceLocation);
      std::swap(m_FunctionString, other.m_FunctionString);
    }
    return *this;
  }

  bool Task::CanBeExecuted() const
  {
    if (m_ExecutionThreadId != 0) return false;
    return m_TaskFunction->CanBeCalled();
  }

  void Task::operator()()

  {
    m_ExecutionThreadId = Thread::GetCurrentThreadId();
    (*m_TaskFunction)();
  }

  bool Task::IsWaitingFuture(size_t FutureId) const
  {
    return m_TaskFunction->IsWaitingFuture(FutureId);
  }

  FutureAvailability Task::GetFutureAvailability() const
  {
    return m_TaskFunction->GetFutureAvailability();
  }
}

