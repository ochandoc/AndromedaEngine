namespace And
{
  template<typename ret_t, typename... args_t>
  TaskFunction<ret_t(args_t...)>::TaskFunction(std::packaged_task<ret_t(args_t...)>& task, Future<args_t>... args) : m_Args(std::make_tuple(std::move(args)...)), m_Function(std::move(task)), m_Future(m_Function.get_future())
  {

  }

  template<typename ret_t, typename... args_t>
  Future<ret_t> TaskFunction<ret_t(args_t...)>::GetFuture()
  {
    return m_Future;
  }

  template<typename ret_t, typename... args_t>
  void TaskFunction<ret_t(args_t...)>::operator()()
  {
    return CallFunction(std::make_integer_sequence<int, sizeof...(args_t)>{});
  }

  template<typename ret_t, typename... args_t>
  bool TaskFunction<ret_t(args_t...)>::CanBeCalled() const
  {
    return AreAllFutureAvailable(std::make_integer_sequence<int, sizeof...(args_t)>{});
  }

  template<typename ret_t, typename... args_t>
  FutureAvailability TaskFunction<ret_t(args_t...)>::GetFutureAvailability()
  {
    return FutureAvailability(m_Future);
  }

  template<typename ret_t, typename... args_t>
  bool TaskFunction<ret_t(args_t...)>::IsWaitingFuture(size_t FutureId) const
  {
    return CheckWaitingFutures(std::make_integer_sequence<int, sizeof...(args_t)>{}, FutureId);
  }

  template<typename ret_t, typename... args_t>
  template<int... ints>
  bool TaskFunction<ret_t(args_t...)>::AreAllFutureAvailable(std::integer_sequence<int, ints...> int_seq) const
  {
    return (std::get<ints>(m_Args).IsAvailable() && ...);
  }

  template<typename ret_t, typename... args_t>
  template<int... ints>
  void TaskFunction<ret_t(args_t...)>::CallFunction(std::integer_sequence<int, ints...> int_seq)
  {
    m_Function((std::get<ints>(m_Args).Get())...);
  }

  template<typename ret_t, typename... args_t>
  template<int... ints>
  bool TaskFunction<ret_t(args_t...)>::CheckWaitingFutures(std::integer_sequence<int, ints...> int_seq, size_t FutureId) const
  {
    return (IsSameFuture(std::get<ints>(m_Args), FutureId) || ...);
  }

  template<typename ret_t, typename... args_t>
  template<typename T>
  bool TaskFunction<ret_t(args_t...)>::IsSameFuture(const Future<T>& future, size_t FutureId) const
  {
    return future.GetId() == FutureId;
  }
}