#pragma once

/* Copyright (C) 2024
 * Author: Hector Ochando <ochandoca@esat-alumni.com>
 *
 * Events              
 * 
 */

#include <memory>
#include <vector>
#include <mutex>

template<typename ret_t, typename... args_t>
class ICallable
{
public:
  ICallable() = default;

  virtual ~ICallable() = default;

  virtual std::unique_ptr<ICallable> Copy() const = 0;
  virtual ret_t operator()(args_t...) = 0;
  virtual bool operator==(const ICallable&) const = 0;
  virtual bool operator!=(const ICallable& other) const { return !(*this == other); }
};

template<typename lambda_t, typename ret_t, typename... args_t>
class TLambdaWraper : public ICallable<ret_t, args_t...>
{
public:
  TLambdaWraper(lambda_t l) : Lambda(l) {}

  virtual ~TLambdaWraper() = default;

  virtual std::unique_ptr<ICallable<ret_t, args_t...>> Copy() const override
  {
    /** Create a new TLambdaWraper with the same lambda */
    return std::unique_ptr<ICallable<ret_t, args_t...>>(new TLambdaWraper(Lambda));
  }

  virtual bool operator==(const ICallable<ret_t, args_t...>& other) const override
  {
    /** Try to cast ICallable to TLambdaWraper */
    auto LambdaWraper = dynamic_cast<const TLambdaWraper*>(&other);
    if (LambdaWraper)
    {
      /** If cast succes compare the lambdas */
      return LambdaWraper->Lambda == Lambda;
    }
    return false;
  }

  virtual ret_t operator()(args_t... args) override
  {
    /** Call the function with the given arguments */
    return Lambda(args...);
  }
private:
  lambda_t Lambda;
};

template<typename ret_t, typename... args_t>
class TFunctionWraper : public ICallable<ret_t, args_t...>
{
public:
  using TFunction = ret_t(*)(args_t...);

  TFunctionWraper(TFunction f) : Function(f) {}

  virtual ~TFunctionWraper() = default;

  virtual std::unique_ptr<ICallable<ret_t, args_t...>> Copy() const override
  {
    /** Create a new TFunctionWraper with the same function */
    return std::unique_ptr<ICallable<ret_t, args_t...>>(new TFunctionWraper(Function));
  }

  virtual bool operator==(const ICallable<ret_t, args_t...>& other) const override
  {
    /** Try to cast ICallable to TFunctionWraper */
    auto FunctionWraper = dynamic_cast<const TFunctionWraper*>(&other);
    if (FunctionWraper)
    {
      /** If cast succes compare functions */
      return FunctionWraper->Function == Function;
    }
    return false;
  }

  virtual ret_t operator()(args_t... args) override
  {
    /** Call the function with the given arguments */
    return (*Function)(args...);
  }
private:
  TFunction Function;
};

template<typename obj_t, typename ret_t, typename... args_t>
class TMethodWraper : public ICallable<ret_t, args_t...>
{
public:
  using TMethod = ret_t(obj_t::*)(args_t...);
  using TConstMethod = ret_t(obj_t::*)(args_t...) const;

  TMethodWraper(obj_t* object, TMethod method) : Object(object), Method(method) {}
  TMethodWraper(obj_t* object, TConstMethod method) : Object(object) { Method = reinterpret_cast<TMethod>(method); }

  virtual ~TMethodWraper() = default;

  virtual std::unique_ptr<ICallable<ret_t, args_t...>> Copy() const override
  {
    /** Create a new TMethodWraper with the same object and method */
    return std::unique_ptr<ICallable<ret_t, args_t...>>(new TMethodWraper(Object, Method));
  }

  virtual bool operator==(const ICallable<ret_t, args_t...>& other) const override
  { 
    /** Try to cast ICallable to TMethodWraper */
    auto MethodWraper = dynamic_cast<const TMethodWraper*>(&other);
    if (MethodWraper)
    {
      /** If cast succes compare method and object */
      return MethodWraper->Method == Method && MethodWraper->Object == Object;
    }
    return false;
  }

  virtual ret_t operator()(args_t... args) override
  {
    /** Call the method on the object stored with the given argumets */
    return (Object->*Method)(args...);
  }
private:
  obj_t* Object;
  TMethod Method;
};

template<typename func_t>
class TDelegate;

template<typename ret_t, typename... args_t>
class TDelegate<ret_t(args_t...)>
{
public:
  TDelegate() = default;
  TDelegate(const TDelegate& other)
  {
    if (other.Callable)
    {
      /** Lock the mutex fot thread safe */
      std::lock_guard<std::mutex> lock(Mutex);
      /** Copy the other callable */
      Callable = std::unique_ptr<ICallable<ret_t, args_t...>>(other.Callable->Copy());
    }
  }
  TDelegate(TDelegate&& other)
  {
    /** Lock the mutex fot thread safe */
    std::lock_guard<std::mutex> lock(Mutex);
    /** Move the other callable into this callable */
    Callable = std::move(other.Callable);
  }

  ~TDelegate() = default;

  TDelegate& operator=(const TDelegate& other)
  {
    if (this != &other)
    {
      /** Lock the mutex fot thread safe */
      std::lock_guard<std::mutex> lock(Mutex);
      /** Copy the other callable */
      Callable = std::unique_ptr<ICallable<ret_t, args_t...>>(other.Callable->Copy());
    }
    return *this;
  }
  TDelegate& operator=(TDelegate&& other)
  {
    if (this != &other)
    {
      /** Lock the mutex fot thread safe */
      std::lock_guard<std::mutex> lock(Mutex);
      /** Swap the callables */
      std::swap(Callable, other.Callable);
    }
    return *this;
  }

  void Release()
  {
    /** Lock the mutex fot thread safe */
    std::lock_guard<std::mutex> lock(Mutex);
    /** Release the ICallable poiner */
    Callable.release();
  }

  bool IsBound()
  {
    /** Return if the pointer has value */
    return !bool(Callable);
  }

  template<typename lambda_t>
  void Bind(lambda_t lambda)
  {
    /** Lock the mutex fot thread safe */
    std::lock_guard<std::mutex> lock(Mutex);
    /** Create a new TLambdaWraper */
    Callable = std::make_unique<TLambdaWraper<lambda_t, ret_t, args_t...>>(lambda);
  }

  void Bind(ret_t(*Function)(args_t...))
  {
    /** Lock the mutex fot thread safe */
    std::lock_guard<std::mutex> lock(Mutex);
    /** Create a new TFunctionWraper */
    Callable = std::make_unique<TFunctionWraper<ret_t, args_t...>>(Function);
  }

  template<typename obj_t>
  void Bind(obj_t* object, ret_t(obj_t::*method)(args_t...))
  {
    /** Lock the mutex fot thread safe */
    std::lock_guard<std::mutex> lock(Mutex);
    /** create a new TMethodWraper */
    Callable = std::make_unique<TMethodWraper<obj_t, ret_t, args_t...>>(object, method);
  }

  template<typename obj_t>
  void Bind(obj_t* object, ret_t(obj_t::* method)(args_t...) const)
  {
    /** Lock the mutex fot thread safe */
    std::lock_guard<std::mutex> lock(Mutex);
    /** Create a new TMethodWraper */
    Callable = std::make_unique<TMethodWraper<obj_t, ret_t, args_t...>>(object, method);
  }

  virtual ret_t operator()(args_t... args)
  {
    /** Lock the mutex fot thread safe */
    std::lock_guard<std::mutex> lock(Mutex);
    /** Call the callable, DON'T CHECK IF CALLABLE IS BOUNDED */
    return (*Callable)(args...);
  }
private:
  std::mutex Mutex;
  std::unique_ptr<ICallable<ret_t, args_t...>> Callable;
};  

template<typename... args_t>
class TMulticastDelegate
{
public:
  TMulticastDelegate() = default;
  TMulticastDelegate(const TMulticastDelegate& other)
  {
    /** Lock the mutex fot thread safe */
    std::lock_guard<std::mutex> lock(Mutex);
    /** Copy all the Listeners of the other MulticastDelegate */
    for (auto& Callable : other.Listeners)
    {
      Listeners.emplace_back(Callable->Copy());
    }
  }
  TMulticastDelegate(TMulticastDelegate&& other)
  {
    /** Lock the mutex fot thread safe */
    std::lock_guard<std::mutex> lock(Mutex);
    /** Move the other listeners into the listeners */
    Listeners = std::move(other.Listeners);
  }

  ~TMulticastDelegate() = default;

  TMulticastDelegate& operator=(const TMulticastDelegate& other)
  {
    if (this != &other)
    {
      /** Lock the mutex fot thread safe */
      std::lock_guard<std::mutex> lock(Mutex);
      /** Clear the current listeners vector */
      Listeners.clear();
      /** Copy all the Listeners of the other MulticastDelegate */
      for (auto& Callable : other.Listeners)
      {
        Listeners.emplace_back(Callable->Copy());
      }
    }
    return *this;
  }
  TMulticastDelegate& operator=(TMulticastDelegate&& other)
  {
    if (this != &other)
    {
      /** Lock the mutex fot thread safe */
      std::lock_guard<std::mutex> lock(Mutex);
      /** Swap the listeners vectors */
      std::swap(Listeners, other.Listeners);
    }
    return *this;
  }

  template<typename lambda_t>
  void AddDynamic(lambda_t lambda)
  {
    /** Lock the mutex fot thread safe */
    std::lock_guard<std::mutex> lock(Mutex);
    /** Create a new ICallable of the type TLambdaWraper */
    auto Callable = std::make_unique<TLambdaWraper<lambda_t, void, args_t...>>(lambda);
    /** Add the Icallable to the vector */
    Listeners.push_back(std::move(Callable));
  }

  void AddDynamic(void(*Function)(args_t...))
  {
    /** Lock the mutex fot thread safe */
    std::lock_guard<std::mutex> lock(Mutex);
    /** Create a new ICallable of the type TFunctionWraper */
    auto Callable = std::make_unique<TFunctionWraper<void, args_t...>>(Function);
    /** Add the Icallable to the vector */
    Listeners.push_back(std::move(Callable));
  }

  template<typename obj_t>
  void AddDynamic(obj_t* object, void(obj_t::* method)(args_t...))
  {
    /** Lock the mutex fot thread safe */
    std::lock_guard<std::mutex> lock(Mutex);
    /** Create a new ICallable of the type TMethodWraper */
    auto Callable = std::make_unique<TMethodWraper<obj_t, void, args_t...>>(object, method);
    /** Add the Icallable to the vector */
    Listeners.push_back(std::move(Callable));
  }

  template<typename obj_t>
  void AddDynamic(obj_t* object, void(obj_t::* method)(args_t...) const)
  {
    /** Lock the mutex fot thread safe */
    std::lock_guard<std::mutex> lock(Mutex);
    /** Create a new ICallable of the type TMethodWraper */
    std::unique_ptr<ICallable<void, args_t...>> Callable = std::make_unique<TMethodWraper<obj_t, void, args_t...>>(object, method);
    /** Add the Icallable to the vector */
    Listeners.push_back(std::move(Callable));
  }
  
  template<typename lambda_t>
  void Remove(lambda_t lambda)
  {
    /** Lock the mutex fot thread safe */
    std::lock_guard<std::mutex> lock(Mutex);
    /** Create a temporal TLambdaWraper to comare */
    TLambdaWraper<lambda_t, void, args_t...> Callable(lambda);
    for (auto it = Listeners.begin(); it != Listeners.end(); ++it)
    {
      /** Compare with the listeners of the vector */
      if (*(*it) == Callable)
      {
        /** If there is in the vector, remive it from there */
        Listeners.erase(it);
        return;
      }
    }
  }

  void Remove(void(*Function)(args_t...))
  {
    /** Lock the mutex fot thread safe */
    std::lock_guard<std::mutex> lock(Mutex);
    /** Create a temporal TFunctionWraper to comare */
    TFunctionWraper<void, args_t...> Callable(Function);
    for (auto it = Listeners.begin(); it != Listeners.end(); ++it)
    {
      /** Compare with the listeners of the vector */
      if (*(*it) == Callable)
      {
        /** If there is in the vector, remive it from there */
        Listeners.erase(it);
        return;
      }
    }
  }

  template<typename obj_t>
  void Remove(obj_t* object, void(obj_t::* method)(args_t...))
  {
    /** Lock the mutex fot thread safe */
    std::lock_guard<std::mutex> lock(Mutex);
    /** Create a temporal TMethodWraper to comare */
    TMethodWraper<obj_t, void, args_t...> Callable(object, method);
    for (auto it = Listeners.begin(); it != Listeners.end(); it++)
    {
      /** Compare with the listeners of the vector */
      if (*(*it) == Callable)
      {
        /** If there is in the vector, remive it from there */
        Listeners.erase(it);
        return;
      }
    }
  }

  template<typename obj_t>
  void Remove(obj_t* object, void(obj_t::* method)(args_t...) const)
  {
    /** Lock the mutex fot thread safe */
    std::lock_guard<std::mutex> lock(Mutex);
    /** Create a temporal TMethodWraper to comare */
    TMethodWraper<obj_t, void, args_t...> Callable(object, method);
    for (auto it = Listeners.begin(); it != Listeners.end(); ++it)
    {
      /** Compare with the listeners of the vector */
      if (*(*it) == Callable)
      {
        /** If there is in the vector, remive it from there */
        Listeners.erase(it);
        return;
      }
    }
  }

  void RemoveAll()
  {
    /** Lock the mutex fot thread safe */
    std::lock_guard<std::mutex> lock(Mutex);
    /** Clear all the listeners */
    Listeners.clear();
  }

  bool IsBounded() const
  {
    /** Check if there are listeners */
    return Listeners.size() != 0;
  }

  void Broadcast(args_t... args)
  {
    /** Lock the mutex for thread safe */
    Mutex.lock();
    /** Copy the listeners for sure the correct call */
    std::vector<std::unique_ptr<ICallable<void, args_t...>>> TmpListeners;
    for (auto& Delegate : Listeners)
    {
      TmpListeners.push_back(Delegate->Copy());
    }
    Mutex.unlock();
    /** Call all the listeners, DON'T CHECK IF LISTENER IS BOUNDED */
    for (auto& Delegate : TmpListeners)
    {
      (*Delegate)(args...);
    }
  }

private:
  std::mutex Mutex;
  std::vector<std::unique_ptr<ICallable<void, args_t...>>> Listeners;
};

/** Help macro to declare delegate DO NOT USE */
#define FUNC_DECLARE_DELEGATE(name, return_type, ...) \
        typedef TDelegate<return_type(__VA_ARGS__)> name;

/** Help macro to declare multicast delegate DO NOT USE */
#define FUNC_DECLARE_MULTICAST_DELEGATE(name, ...) \
        typedef  TMulticastDelegate<__VA_ARGS__> name;

/** Macros to declare delegate */
#define DECLARE_DELEGATE(name, return_type) FUNC_DECLARE_DELEGATE(name, return_type)
#define DECLARE_DELEGATE_ONE_PARAM(name, return_type, param1_type, param1_name) FUNC_DECLARE_DELEGATE(name, return_type, param1_type)
#define DECLARE_DELEGATE_TWO_PARAM(name, return_type, param1_type, param1_name, param2_type, param2_name) FUNC_DECLARE_DELEGATE(name, return_type, param1_type, param2_type)
#define DECLARE_DELEGATE_THREE_PARAM(name, return_type, param1_type, param1_name, param2_type, param2_name, param3_type, param3_name) FUNC_DECLARE_DELEGATE(name, return_type, param1_type, param2_type, param3_type)
#define DECLARE_DELEGATE_FOUR_PARAM(name, return_type, param1_type, param1_name, param2_type, param2_name, param3_type, param3_name, param4_type, param4_name) FUNC_DECLARE_DELEGATE(name, return_type, param1_type, param2_type, param3_type, param4_type)
#define DECLARE_DELEGATE_FIVE_PARAM(name, return_type, param1_type, param1_name, param2_type, param2_name, param3_type, param3_name, param4_type, param4_name, param5_type, param5_name) FUNC_DECLARE_DELEGATE(name, return_type, param1_type, param2_type, param3_type, param4_type, param5_type)
#define DECLARE_DELEGATE_SIX_PARAM(name, return_type, param1_type, param1_name, param2_type, param2_name, param3_type, param3_name, param4_type, param4_name, param5_type, param5_name, param6_type, param6_name) FUNC_DECLARE_DELEGATE(name, return_type, param1_type, param2_type, param3_type, param4_type, param5_type, param6_type)
#define DECLARE_DELEGATE_SEVEN_PARAM(name, return_type, param1_type, param1_name, param2_type, param2_name, param3_type, param3_name, param4_type, param4_name, param5_type, param5_name, param6_type, param6_name, param7_type, param7_name) FUNC_DECLARE_DELEGATE(name, return_type, param1_type, param2_type, param3_type, param4_type, param5_type, param6_type, param7_type)
#define DECLARE_DELEGATE_EIGHT_PARAM(name, return_type, param1_type, param1_name, param2_type, param2_name, param3_type, param3_name, param4_type, param4_name, param5_type, param5_name, param6_type, param6_name, param7_type, param7_name, param8_type, param8_name) FUNC_DECLARE_DELEGATE(name, return_type, param1_type, param2_type, param3_type, param4_type, param5_type, param6_type, param7_type, param8_type)

/** Macros to declare multicast delegate */
#define DECLARE_MULTICAST_DELEGATE(name) FUNC_DECLARE_MULTICAST_DELEGATE(name)
#define DECLARE_MULTICAST_DELEGATE_ONE_PARAM(name, param1_type, param1_name) FUNC_DECLARE_MULTICAST_DELEGATE(name, param1_type)
#define DECLARE_MULTICAST_DELEGATE_TWO_PARAM(name, param1_type, param1_name, param2_type, param2_name) FUNC_DECLARE_MULTICAST_DELEGATE(name, param1_type, param2_type)
#define DECLARE_MULTICAST_DELEGATE_THREE_PARAM(name, param1_type, param1_name, param2_type, param2_name, param3_type, param3_name) FUNC_DECLARE_MULTICAST_DELEGATE(name, param1_type, param2_type, param3_type)
#define DECLARE_MULTICAST_DELEGATE_FOUR_PARAM(name, param1_type, param1_name, param2_type, param2_name, param3_type, param3_name, param4_type, param4_name) FUNC_DECLARE_MULTICAST_DELEGATE(name, param1_type, param2_type, param3_type, param4_type)
#define DECLARE_MULTICAST_DELEGATE_FIVE_PARAM(name, param1_type, param1_name, param2_type, param2_name, param3_type, param3_name, param4_type, param4_name, param5_type, param5_name) FUNC_DECLARE_MULTICAST_DELEGATE(name, param1_type, param2_type, param3_type, param4_type, param5_type)
#define DECLARE_MULTICAST_DELEGATE_SIX_PARAM(name, param1_type, param1_name, param2_type, param2_name, param3_type, param3_name, param4_type, param4_name, param5_type, param5_name, param6_type, param6_name) FUNC_DECLARE_MULTICAST_DELEGATE(name, param1_type, param2_type, param3_type, param4_type, param5_type, param6_type)
#define DECLARE_MULTICAST_DELEGATE_SEVEN_PARAM(name, param1_type, param1_name, param2_type, param2_name, param3_type, param3_name, param4_type, param4_name, param5_type, param5_name, param6_type, param6_name, param7_type, param7_name) FUNC_DECLARE_MULTICAST_DELEGATE(name, param1_type, param2_type, param3_type, param4_type, param5_type, param6_type, param7_type)
#define DECLARE_MULTICAST_DELEGATE_EIGHT_PARAM(name, param1_type, param1_name, param2_type, param2_name, param3_type, param3_name, param4_type, param4_name, param5_type, param5_name, param6_type, param6_name, param7_type, param7_name, param8_type, param8_name) FUNC_DECLARE_MULTICAST_DELEGATE(name, param1_type, param2_type, param3_type, param4_type, param5_type, param6_type, param7_type, param8_type)

