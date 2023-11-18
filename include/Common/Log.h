#pragma once

#include "andpch.hpp"
#include "base.h"

namespace spdlog
{
  class logger;
}

namespace And
{
  enum LogLevel
  {
    Trace     = 1 << 0,
    Debug     = 1 << 1,
    Info      = 1 << 2,
    Warning   = 1 << 3,
    Error     = 1 << 4,
    Critical  = 1 << 5,
  };

  class LogWindow;

  namespace internal
  {
    struct LogCategoryInfo
    {
      std::string Name;
      uint64 Id;
    };

    struct LogCategoryBase
    {
      using Logger = class spdlog::logger;

      LogCategoryBase(const char* name, LogLevel DefaultCategoryLevel);

      void log(LogLevel Level, const char* Message);

      friend class LogWindow;

    private:
      static uint64 GetNextId();

    private:
      LogLevel m_DefaultLevel;
      const std::string m_CategoryName;
      uint64 m_Id;
      std::shared_ptr<Logger> m_Logger;
      bool Registered = false;
    };

    
  }

  template<LogLevel InCategoryLevel>
  struct LogCategory : public internal::LogCategoryBase
  {
    static constexpr LogLevel CategoryLevel = InCategoryLevel;

    inline constexpr LogLevel GetCategoryLevel() const { return CategoryLevel; }

    inline LogCategory(const char* CategoryName) : LogCategoryBase(CategoryName, InCategoryLevel) {}
  };

  class LogWindow
  {
  public:
    void Draw();

    void ClearLog();

    inline void Open() { bOpen = true; }
    inline void Close() { bOpen = false; }
  private:
    bool bOpen;
  };
}

#define DECLARE_LOG_CATEGORY(CategoryName, DefaultLevel) \
  extern struct LogCategory##CategoryName : public ::And::LogCategory<::And::DefaultLevel> \
  { \
    inline LogCategory##CategoryName() : LogCategory(#CategoryName) {} \
  } CategoryName;

#define DEFINE_LOG_CATEGORY(CategoryName) LogCategory##CategoryName CategoryName;

#define AND_LOG(CategoryName, Level, Format, ...) CategoryName.log(Level, std::format(Format, __VA_ARGS__).c_str())