#pragma once

#include "base.h"

#include "Common/Log.h"

#include "imgui.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/base_sink.h"

namespace And
{
  struct SinkLineContent
  {
    uint8 Level;
    std::string Text;
    uint32 CategoryId;
    const ImVec4* Color;
  };

  class ImGuiSink : public spdlog::sinks::base_sink<std::mutex>
  {
    using sink_t = spdlog::sinks::base_sink<std::mutex>;
  public:
    ImGuiSink();

    virtual ~ImGuiSink();

    std::unordered_map<std::string, uint32> m_LogCategoriesIds;
  protected:
    virtual void sink_it_(const spdlog::details::log_msg& msg) override;
    virtual void flush_() override;


  public:

    static std::mutex SinkMutex;
    static std::vector<SinkLineContent> LogData;
  };
}