#pragma once

#include "andpch.hpp"
#include "Andromeda/Misc/CoreMiscDefines.h"
#include "Andromeda/HAL/Types.h"
#include "Andromeda/Editor/EditorWindow.h"
#include <string>

namespace spdlog
{
  class logger;
}

namespace And
{
  /**
   * @enum LogLevel
   * @brief Enumeration of different log levels.
   */
  enum LogLevel
  {
    Trace     = 1 << 0,   /**< Trace log level. */
    Debug     = 1 << 1,   /**< Debug log level. */
    Info      = 1 << 2,   /**< Info log level. */
    Warning   = 1 << 3,   /**< Warning log level. */
    Error     = 1 << 4,   /**< Error log level. */
    Critical  = 1 << 5,   /**< Critical log level. */
  };

  class LogWindow;

  namespace internal
  {
    /**
     * @struct LogCategoryInfo
     * @brief Contains information about a log category.
     */
    struct LogCategoryInfo
    {
      std::string Name;
      uint32 Id;
    };

    /**
     * @struct LogCategoryBase
     * @brief Base structure for log categories.
     */
    struct LogCategoryBase
    {
      using Logger = class spdlog::logger;

      /**
       * @brief Constructs a LogCategoryBase.
       * @param name Name of the category.
       * @param DefaultCategoryLevel Default log level for the category.
       * @param ConsoleLog Whether to log to console.
       */
      LogCategoryBase(const char* name, LogLevel DefaultCategoryLevel, bool ConsoleLog);

      /**
       * @brief Logs a message.
       * @param Level Log level.
       * @param Message Message to log.
       */
      void log(LogLevel Level, const char* Message);

      friend class LogWindow;

    private:
      static uint32 GetNextId();

    private:
      LogLevel m_DefaultLevel;              /**< Default log level. */
      const std::string m_CategoryName;     /**< Name of the category. */
      uint32 m_Id;                          /**< ID of the category. */
      std::shared_ptr<Logger> m_Logger;     /**< Logger instance. */
      bool Registered = false;              /**< Whether the category is registered. */
    };

    
  }

  /**
   * @struct LogCategory
   * @brief Template structure for log categories.
   * @tparam InCategoryLevel Default log level for the category.
   * @tparam InConsoleLog Whether to log to console.
   */
  template<LogLevel InCategoryLevel, bool InConsoleLog>
  struct LogCategory : public internal::LogCategoryBase
  {
    static constexpr LogLevel CategoryLevel = InCategoryLevel;  /**< Category log level. */
    static constexpr bool ConsoleLog = InConsoleLog;            /**< Whether to log to console. */

    /**
     * @brief Gets the log level for the category.
     * @return The log level.
     */
    inline constexpr LogLevel GetCategoryLevel() const { return CategoryLevel; }

    /**
     * @brief Constructs a LogCategory.
     * @param CategoryName Name of the category.
     */
    inline LogCategory(const char* CategoryName) : LogCategoryBase(CategoryName, InCategoryLevel, InConsoleLog) {}
  };

  /**
   * @class LogWindow
   * @brief A window for displaying logs in the editor.
   */
  class LogWindow : public EditorWindow
  {
  public:
    /**
     * @brief Constructs a LogWindow.
     * @param Title Title of the window.
     */
    LogWindow(const std::string& Title);

    /**
    * @brief Shows the log window.
    */
    void Show() override;

    /**
    * @brief Clears the log.
    */
    void ClearLog();

    /**
     * @brief Opens the log window.
     */
    inline void Open() { m_is_open = true; }
    /**
     * @brief Closes the log window.
     */
    inline void Close() { m_is_open = false; }
  private:
    char SearchText[1024];              /**< Search text buffer. */
    bool EnableAutoScrolling = true;    /**< Whether auto-scrolling is enabled. */
    uint32 LogCategoryId = -1;          /**< ID of the log category. */
    bool bTrace = false;                /**< Whether trace logs are enabled. */
    bool bDebug = false;                /**< Whether debug logs are enabled. */
    bool bInfo = true;                  /**< Whether info logs are enabled. */
    bool bWarning = true;               /**< Whether warning logs are enabled. */
    bool bError = true;                 /**< Whether error logs are enabled. */
    bool bCritical = false;             /**< Whether critical logs are enabled. */
    bool bGoDown;                       /**< Whether to scroll down. */
  };
}

#define DECLARE_LOG_CATEGORY(CategoryName, DefaultLevel, LogInConsole) \
  extern struct LogCategory##CategoryName : public ::And::LogCategory<::And::DefaultLevel, LogInConsole> \
  { \
    inline LogCategory##CategoryName() : LogCategory(#CategoryName) {} \
  } CategoryName;

#define DEFINE_LOG_CATEGORY(CategoryName) LogCategory##CategoryName CategoryName;

#define AND_LOG(CategoryName, Level, Format, ...) CategoryName.log(Level, std::format(Format, __VA_ARGS__).c_str())