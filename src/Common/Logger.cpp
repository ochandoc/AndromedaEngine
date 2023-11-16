#include "Common/Logger.h"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace And
{

  struct LoggerData
  {
    std::shared_ptr<spdlog::logger> log;
  };

  static Logger* s_Instace = nullptr;

  Logger::Logger() : m_Data(new LoggerData)
  {
    auto ConsoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    ConsoleSink->set_level(spdlog::level::trace);
    ConsoleSink->set_pattern("%^%n: %l: %v%$");

    m_Data->log = std::make_shared<spdlog::logger>("AndLog");
    m_Data->log->set_level(spdlog::level::trace);

    m_Data->log->sinks().push_back(ConsoleSink);
  }

  Logger::~Logger()
  {

  }

  Logger* Logger::GetInstane()
  {
    return s_Instace;
  }

  void Logger::Log(LogLevel level, const char* msg)
  {
    switch (level)
    {
    case And::LogLevel::Trace:
      m_Data->log->trace(msg);
      break;
    case And::LogLevel::Debug:
      m_Data->log->debug(msg);
      break;
    case And::LogLevel::Info:
      m_Data->log->info(msg);
      break;
    case And::LogLevel::Warning:
      m_Data->log->warn(msg);
      break;
    case And::LogLevel::Error:
      m_Data->log->error(msg);
      break;
    case And::LogLevel::Critical:
      m_Data->log->critical(msg);
      break;
    }
  }

  void Logger::InitLogger()
  {
    if (s_Instace) return;
    s_Instace = new Logger();
  }

  void Logger::TerminateLogger()
  {
    delete s_Instace;
    s_Instace = nullptr;
  }


}