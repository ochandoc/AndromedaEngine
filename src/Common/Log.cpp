#include "Common/Log.h"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

#include "Common/ImGuiSink.h"

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

  return spdlog::level::off;
}

namespace And
{
  namespace internal
  {
    static std::vector<internal::LogCategoryInfo> s_Categories;

    LogCategoryBase::LogCategoryBase(const char* name, LogLevel DefaultCategoryLevel, bool ConsoleLog) : m_CategoryName(name), m_DefaultLevel(DefaultCategoryLevel)
    {
      m_Id = GetNextId();
      m_Logger = std::make_shared<Logger>(name);

      auto sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
      if (ConsoleLog)
        sink->set_level(GetSpdlogLevel(DefaultCategoryLevel));
      else
        sink->set_level(spdlog::level::err);
      sink->set_pattern("%^%n: %l: %v%$");

      auto FileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("Andromeda.log");
      FileSink->set_level(GetSpdlogLevel(DefaultCategoryLevel));
      FileSink->set_pattern("[%A %H:%M:%S] %n: %l: %v");

      spdlog::sink_ptr ImSink;
      bool have_sink = false;
      spdlog::details::registry::instance().apply_all([&have_sink, &ImSink](const std::shared_ptr<Logger> logger) {
        if (!have_sink)
        {
          for (int i = 0; i < logger->sinks().size() && !have_sink; i++)
          {
            auto sink = logger->sinks()[i];
            ImGuiSink* ImguiSink = dynamic_cast<ImGuiSink*>(sink.get());
            if (ImguiSink)
            {
              have_sink = true;
              ImSink = sink;
            }
          }
        }
      });

      if (!have_sink)
      {
        ImSink = std::make_shared<ImGuiSink>();
      }

      ImSink->set_level(GetSpdlogLevel(DefaultCategoryLevel));
      ImSink->set_pattern("%^%n: %l: %v%$");
      static_cast<ImGuiSink*>(ImSink.get())->m_LogCategoriesIds.insert({name, m_Id});

      m_Logger->sinks().push_back(ImSink);
      m_Logger->sinks().push_back(sink);
      m_Logger->sinks().push_back(FileSink);
      m_Logger->set_level(spdlog::level::trace);

      spdlog::register_logger(m_Logger);
    }

    void LogCategoryBase::log(LogLevel Level, const char* Message)
    {
      spdlog::get(m_CategoryName)->log(GetSpdlogLevel(Level), Message);
      if (!Registered)
      {
        Registered = true;
        internal::LogCategoryInfo CategoryInfo;
        CategoryInfo.Name = m_CategoryName;
        CategoryInfo.Id = m_Id;

        s_Categories.push_back(CategoryInfo);
      }
    }

    uint32 LogCategoryBase::GetNextId()
    {
      static uint8 CurrentId = 0;
      uint32 id = 0;
      if (CurrentId <= 64)
      {
        id = 1ll << CurrentId;
        CurrentId++;
      }
      return id;
    }

  }

  void LogWindow::Draw()
  {
    static char SearchText[1024];
    static bool EnableAutoScrolling = true;
    static uint32 LogCategoryId = -1;
    static bool bTrace = false;
    static bool bDebug = false;
    static bool bInfo = true;
    static bool bWarning = true;
    static bool bError = true;
    static bool bCritical = false;
    static bool bGoDown;

    if (bOpen)
    {
      if (ImGui::Begin("Console Log", &bOpen))
      {
        ImGui::InputText("##ConsoleLogSearchText", SearchText, sizeof(SearchText));
        ImGui::SameLine();
        if (ImGui::Button("Filters"))
          ImGui::OpenPopup("Filters");
        if (ImGui::BeginPopup("Filters"))
        {
          ImGui::MenuItem("Trace", NULL, &bTrace);
          ImGui::MenuItem("Debug", NULL, &bDebug);
          ImGui::MenuItem("Info", NULL, &bInfo);
          ImGui::MenuItem("Warning", NULL, &bWarning);
          ImGui::MenuItem("Error", NULL, &bError);
          ImGui::MenuItem("Critical", NULL, &bCritical);
          ImGui::Separator();
          if (ImGui::BeginMenu("Categories"))
          {
            ImGui::CheckboxFlags("Show All", (int*)&LogCategoryId, -1);
            ImGui::Separator();
            for (const internal::LogCategoryInfo& CategoryInfo : internal::s_Categories)
            {
              ImGui::CheckboxFlags(CategoryInfo.Name.c_str(), &LogCategoryId, CategoryInfo.Id);
            }
            ImGui::EndMenu();
          }
          ImGui::EndPopup();
        }

        if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
          ImGui::OpenPopup("OptionsRightClick");

        if (ImGui::BeginPopup("OptionsRightClick"))
        {
          if (ImGui::MenuItem("Clear Log"))
          {
            ClearLog();
          }
          ImGui::Separator();
          if (ImGui::MenuItem("AutoScroll", NULL, &EnableAutoScrolling))
          {
            bGoDown = EnableAutoScrolling;
          }
          if (ImGui::MenuItem("Go Down"))
          {
            bGoDown = true;
          }
          ImGui::EndPopup();
        }

        uint8 Level = 0;
        if (bTrace) Level |= Trace;
        if (bDebug) Level |= Debug;
        if (bInfo) Level |= Info;
        if (bWarning) Level |= Warning;
        if (bError) Level |= Error;
        if (bCritical) Level |= Critical;

        ImGui::Separator();
        if (ImGui::BeginChild("ConsoleLogView", ImVec2(0, 0), false, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_AlwaysHorizontalScrollbar))
        {

          for (const SinkLineContent& LineContent : ImGuiSink::LogData)
          {
            if (LineContent.CategoryId & LogCategoryId)
            {
              if (LineContent.Level & Level)
              {
                const char* Begin = LineContent.Text.c_str();
                if (strnlen(SearchText, sizeof(SearchText)) > 0)
                {
                  if (LineContent.Text.find(SearchText) != std::string::npos)
                  {
                    ImGui::TextColored(*LineContent.Color, Begin);
                  }
                }
                else
                {
                  ImGui::TextColored(*LineContent.Color, Begin);
                }
              }
            }
          }

          if (bGoDown)
          {
            ImGui::SetScrollHereY(1.0f);
            bGoDown = false;
          }

          if (EnableAutoScrolling && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
            ImGui::SetScrollHereY(1.0f);

        }
        ImGui::EndChild();


      }
      ImGui::End();
    }

  }

  void LogWindow::ClearLog()
  {
    std::lock_guard<std::mutex> lock(ImGuiSink::SinkMutex);
    ImGuiSink::LogData.clear();
  }
}