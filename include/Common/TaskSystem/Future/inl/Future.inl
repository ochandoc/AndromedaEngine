namespace And
{
  template<typename T>
  Future<T> MakeFuture(Future<T>& future)
  {
    return future;
  }

  template<typename T>
  Future<T> MakeFuture(const T& value)
  {
    return Future<T>(value);
  }
}