#pragma once

#include "base.h"
#include "Common/GraphicsContext.h"
#include "OpenGL.h"

class OpenGLUniformBuffer
{
  NON_COPYABLE_CLASS(OpenGLUniformBuffer)
  NON_MOVABLE_CLASS(OpenGLUniformBuffer)
public:
  OpenGLUniformBuffer(uint32 block_index, uint32 size);

  ~OpenGLUniformBuffer();

  void bind() const;
  void unbind() const;

  void upload_data(const void* data, uint32 size);
private:
  GLuint m_BufferId;
  GLuint m_Size;
  GLuint m_BlockIndex;
};