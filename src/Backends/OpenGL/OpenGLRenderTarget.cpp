#include "Andromeda/Graphics/RenderTarget.h"

#include "Backends/OpenGL/OpenGL.h"

#include "imgui.h"

namespace And
{
  struct RenderTargetData
  {
    uint32 Id;
    uint32 ColorTexture;
    uint32 DepthTexture;
  };
}


And::RenderTarget::RenderTarget(uint32 width, uint32 height) : m_Data(new RenderTargetData)
{
  glGenTextures(1, &m_Data->ColorTexture);
  glBindTexture(GL_TEXTURE_2D, m_Data->ColorTexture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

  glBindTexture(GL_TEXTURE_2D, 0);

  glGenTextures(1, &m_Data->DepthTexture);
  glBindTexture(GL_TEXTURE_2D, m_Data->DepthTexture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

  glBindTexture(GL_TEXTURE_2D, 0);

  glGenFramebuffers(1, &m_Data->Id);
  glBindFramebuffer(GL_FRAMEBUFFER, m_Data->Id);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Data->ColorTexture, 0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_Data->DepthTexture, 0);
  assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

And::RenderTarget::~RenderTarget()
{
  glDeleteTextures(1, &m_Data->ColorTexture);
  glDeleteTextures(1, &m_Data->DepthTexture);
  glDeleteFramebuffers(1, &m_Data->Id);
}

void And::RenderTarget::Bind()
{
  glBindFramebuffer(GL_FRAMEBUFFER, m_Data->Id);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void And::RenderTarget::Unbind()
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void And::RenderTarget::Resize(uint32 width, uint32 height)
{
}

void And::RenderTarget::Test()
{
  ImGuiStyle& style = ImGui::GetStyle();
  ImVec2 padding = style.WindowPadding;
  style.WindowPadding = ImVec2(0.0f, 0.0f);
  static ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoScrollbar;
  windowFlags |= ImGuiWindowFlags_NoCollapse;
  if (ImGui::Begin("Render Target Color", nullptr, windowFlags))
  {
    ImGui::Image((void*)(intptr_t)m_Data->ColorTexture, ImGui::GetWindowSize(), ImVec2(0, 1), ImVec2(1, 0));
  }
  ImGui::End();

  if (ImGui::Begin("Render Target depth", nullptr, windowFlags))
  {
    ImGui::Image((void*)(intptr_t)m_Data->DepthTexture, ImGui::GetWindowSize(), ImVec2(0, 1), ImVec2(1, 0));
  }
  ImGui::End();

  style.WindowPadding = padding;
}


