#pragma once

#include "base.h"

namespace And
{
  enum class LogLevel
  {
    Trace,
    Debug,
    Info,
    Warning,
    Error,
    Critical,
  };

  class Logger
  {
    NON_COPYABLE_CLASS(Logger)
    NON_MOVABLE_CLASS(Logger)
  private:
    Logger();

  public:
    ~Logger();

    static Logger* GetInstane();

    void Log(LogLevel level, const char* msg);

    friend class Engine;
  private:
    static void InitLogger();
    static void TerminateLogger();

  private:
    std::unique_ptr<struct LoggerData> m_Data;
  };
}