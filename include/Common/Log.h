#pragma once

namespace spdlog
{
  class logger;
}

namespace And
{
  enum LogLevel
  {
    Trace,
    Debug,
    Info,
    Warning,
    Error,
    Critical,
  };

  namespace internal
  {
    struct LogCategoryBase
    {
      using Logger = class spdlog::logger;

      LogCategoryBase(const char* name, LogLevel DefaultCategoryLevel);

      void log(LogLevel Level, const char* Message);

    private:
      LogLevel m_DefaultLevel;
      const std::string m_CategoryName;
      std::shared_ptr<Logger> m_Logger;
    };
  }

  template<LogLevel InCategoryLevel>
  struct LogCategory : public internal::LogCategoryBase
  {
    static constexpr LogLevel CategoryLevel = InCategoryLevel;

    inline constexpr LogLevel GetCategoryLevel() const { return CategoryLevel; }

    inline LogCategory(const char* CategoryName) : LogCategoryBase(CategoryName, InCategoryLevel) {}
  };
}

#define DECLARE_LOG_CATEGORY(CategoryName, DefaultLevel) \
  extern struct LogCategory##CategoryName : public ::And::LogCategory<::And::DefaultLevel> \
  { \
    inline LogCategory##CategoryName() : LogCategory(#CategoryName) {} \
  } CategoryName;

#define DEFINE_LOG_CATEGORY(CategoryName) LogCategory##CategoryName CategoryName;

#define AND_LOG(CategoryName, Level, Format, ...) CategoryName.log(Level, std::format(Format, __VA_ARGS__).c_str())