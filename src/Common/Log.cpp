#include "Common/Log.h"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

static inline spdlog::level::level_enum GetSpdlogLevel(And::LogLevel level)
{
  switch (level)
  {
  case And::LogLevel::Trace:
    return spdlog::level::trace;
    break;
  case And::LogLevel::Debug:
    return spdlog::level::debug;
    break;
  case And::LogLevel::Info:
    return spdlog::level::info;
    break;
  case And::LogLevel::Warning:
    return spdlog::level::warn;
    break;
  case And::LogLevel::Error:
    return spdlog::level::err;
    break;
  case And::LogLevel::Critical:
    return spdlog::level::critical;
    break;
  }
}

namespace And
{
  namespace internal
  {
    LogCategoryBase::LogCategoryBase(const char* name, LogLevel DefaultCategoryLevel) : m_CategoryName(name), m_DefaultLevel(DefaultCategoryLevel)
    {
      m_Logger = std::make_shared<Logger>(name);

      auto sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
      sink->set_level(GetSpdlogLevel(DefaultCategoryLevel));
      sink->set_pattern("%^%n: %l: %v%$");

      auto FileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("Andromeda.log");
      FileSink->set_level(GetSpdlogLevel(DefaultCategoryLevel));
      FileSink->set_pattern("[%A %H:%M:%S] %n: %l: %v");

      m_Logger->sinks().push_back(sink);
      m_Logger->sinks().push_back(FileSink);
      m_Logger->set_level(spdlog::level::trace);

      spdlog::register_logger(m_Logger);
    }

    void LogCategoryBase::log(LogLevel Level, const char* Message)
    {
      spdlog::get(m_CategoryName)->log(GetSpdlogLevel(Level), Message);
    }
  }
}