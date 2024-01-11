#pragma once

#include "andpch.hpp"
#include "base.h"
#include "Editor/EditorWindow.h"
#include <string>

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
      uint32 Id;
    };

    struct LogCategoryBase
    {
      using Logger = class spdlog::logger;

      LogCategoryBase(const char* name, LogLevel DefaultCategoryLevel, bool ConsoleLog);

      void log(LogLevel Level, const char* Message);

      friend class LogWindow;

    private:
      static uint32 GetNextId();

    private:
      LogLevel m_DefaultLevel;
      const std::string m_CategoryName;
      uint32 m_Id;
      std::shared_ptr<Logger> m_Logger;
      bool Registered = false;
    };

    
  }

  template<LogLevel InCategoryLevel, bool InConsoleLog>
  struct LogCategory : public internal::LogCategoryBase
  {
    static constexpr LogLevel CategoryLevel = InCategoryLevel;
    static constexpr bool ConsoleLog = InConsoleLog;

    inline constexpr LogLevel GetCategoryLevel() const { return CategoryLevel; }

    inline LogCategory(const char* CategoryName) : LogCategoryBase(CategoryName, InCategoryLevel, InConsoleLog) {}
  };

  class LogWindow : public EditorWindow
  {
  public:

    LogWindow(std::string title);
    LogWindow();
    void Show() override;

    void ClearLog();

    inline void Open() { m_is_open = true; }
    inline void Close() { m_is_open = false; }
  };
}

#define DECLARE_LOG_CATEGORY(CategoryName, DefaultLevel, LogInConsole) \
  extern struct LogCategory##CategoryName : public ::And::LogCategory<::And::DefaultLevel, LogInConsole> \
  { \
    inline LogCategory##CategoryName() : LogCategory(#CategoryName) {} \
  } CategoryName;

#define DEFINE_LOG_CATEGORY(CategoryName) LogCategory##CategoryName CategoryName;

#define AND_LOG(CategoryName, Level, Format, ...) CategoryName.log(Level, std::format(Format, __VA_ARGS__).c_str())