#include "Andromeda/Graphics/RenderTarget.h"

#include "Backends/OpenGL/OpenGL.h"

#include "imgui.h"

namespace And
{
  struct RenderTargetData
  {
    uint32 Id;
    uint32 RenderBuffer;
    uint32 Texture;
  };
}


And::RenderTarget::RenderTarget(const RenderTargetCreationInfo& InCreationInfo) : m_Data(new RenderTargetData)
{
  glGenRenderbuffers(1, &m_Data->RenderBuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, m_Data->RenderBuffer);

  switch (InCreationInfo.format)
  {
  case ETextureFormat::Depth:
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, InCreationInfo.width, InCreationInfo.height);
    break;
  case ETextureFormat::RGBA8:
    glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, InCreationInfo.width, InCreationInfo.height);
    break;
  }

  glBindRenderbuffer(GL_RENDERBUFFER, 0);

  glGenTextures(1, &m_Data->Texture);
  glBindTexture(GL_TEXTURE_2D, m_Data->Texture);

  switch (InCreationInfo.format)
  {
  case ETextureFormat::Depth:
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, InCreationInfo.width, InCreationInfo.height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
    break;
  case ETextureFormat::RGBA8:
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, InCreationInfo.width, InCreationInfo.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    break;
  }

  glBindTexture(GL_TEXTURE_2D, 0);


  glGenFramebuffers(1, &m_Data->Id);
  glBindFramebuffer(GL_FRAMEBUFFER, m_Data->Id);

  switch (InCreationInfo.format)
  {
  case ETextureFormat::Depth:
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_Data->Texture, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_Data->RenderBuffer);
    break;
  case ETextureFormat::RGBA8:
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_Data->RenderBuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Data->Texture, 0);
    break;
  }

  assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

And::RenderTarget::~RenderTarget()
{
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
  style.WindowPadding = ImVec2(0.0f, 0.0f);
  static ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoScrollbar;
  windowFlags |= ImGuiWindowFlags_NoCollapse;
  if (ImGui::Begin("Render Target", nullptr, windowFlags))
  {
    ImGui::Image((void*)(intptr_t)m_Data->Texture, ImGui::GetWindowSize(), ImVec2(1, 1), ImVec2(0, 0));
  }
  ImGui::End();
}


