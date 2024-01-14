namespace And
{
  template<typename func_t, typename... args_t>
  auto TaskSystem::AddTaskWithInfo(std::source_location location, const std::string& functionStr, func_t function, args_t... args) -> Future<decltype(function((MakeFuture(args).Get())...))>
  {
    return AddTaskWithInfo(location, functionStr, 0, function, args...);
  }

  template<typename func_t, typename... args_t>
  auto TaskSystem::AddTaskWithInfo(std::source_location location, const std::string& functionStr, const std::string& ThreadName, func_t function, args_t... args) -> Future<decltype(function((MakeFuture(args).Get())...))>
  {
    return AddTaskWithInfo(location, functionStr, Thread::GetThreadIdByName(ThreadName), function, args...);
  }

  template<typename func_t, typename... args_t>
  auto TaskSystem::AddTaskWithInfo(std::source_location location, const std::string& functionStr, size_t ThreadId, func_t function, args_t... args) -> Future<decltype(function((MakeFuture(args).Get())...))>
  {
    return AddTask(location, functionStr, ThreadId, function, MakeFuture(args)...);
  }

  template<typename func_t, typename... args_t>
  auto TaskSystem::AddTask(std::source_location location, const std::string& functionStr, size_t ThreadId, func_t function, Future<args_t>... args) -> Future<decltype(function((args.Get())...))>
  {
    using ret_t = decltype(function((args.Get())...));
    Task task(ThreadId, location, functionStr);
    std::packaged_task<ret_t(args_t...)> packagedTask(function);
    std::shared_ptr<TaskFunction<ret_t(args_t...)>> taskFunction(new TaskFunction<ret_t(args_t...)>(packagedTask, args...));

    Future<ret_t> future = taskFunction->GetFuture();
    task.m_TaskFunction = std::move(taskFunction);
    task.m_Id = (size_t)task.m_TaskFunction.get();

    if (task.CanBeExecuted())
    {
      AddTaskToWorker(task, task.GetDesiredExecutionThreadId());
    }
    else
    {
      AddWaitTask(task);
    }

    return future;
  }

}