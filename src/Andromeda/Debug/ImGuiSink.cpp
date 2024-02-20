#include "ImGuiSink.h"

namespace And
{
  std::vector<SinkLineContent> ImGuiSink::LogData;
  std::mutex ImGuiSink::SinkMutex;

  const ImVec4 LogLevelColor[] = {
    {0.0f, 1.0f, 0.0f, 1.0,},
    {0.0f, 0.0f, 1.0f, 1.0,},
    {1.0f, 1.0f, 1.0f, 1.0,},
    {1.0f, 0.9137f, 0.0f, 1.0,},
    {1.0f, 0.2f, 0.2f, 1.0,},
    {1.0f, 0.0f, 0.0f, 1.0,},
  };

  ImGuiSink::ImGuiSink()  {      }

  ImGuiSink::~ImGuiSink()
  {

  }

  void ImGuiSink::sink_it_(const spdlog::details::log_msg& msg)
  {
    spdlog::memory_buf_t FormattedBuffer;
    sink_t::formatter_->format(
      msg, FormattedBuffer);
    std::string FormattedText = fmt::to_string(FormattedBuffer);
    FormattedText[FormattedText.find("\n")] = 0;

    SinkLineContent MessageData;
    MessageData.Level = 1 << msg.level;
    MessageData.CategoryId = m_LogCategoriesIds[msg.logger_name.begin()];
    MessageData.Text.append(FormattedText.c_str());
    MessageData.Color = &LogLevelColor[(int)msg.level];

    {
      std::lock_guard<std::mutex> Lock(SinkMutex);
      LogData.push_back(MessageData);
    }
  }

  void ImGuiSink::flush_() {}

}