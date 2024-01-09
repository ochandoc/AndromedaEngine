#include "opengl_uniform_buffer.h"

OpenGLUniformBuffer::OpenGLUniformBuffer(uint32 block_index, uint32 size) : m_Size(size), m_BlockIndex(block_index), m_BufferId(0)
{
  if (size == 0)
  {
    AND_LOG(GraphicsContextLog, And::Error, "Invalid size in Uniform Buffer creation!");
  }
  else
  {
    glGenBuffers(1, &m_BufferId);
    glBindBuffer(GL_UNIFORM_BUFFER, m_BufferId);
    glBufferData(GL_UNIFORM_BUFFER, m_Size, NULL, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
  }
  
}

OpenGLUniformBuffer::~OpenGLUniformBuffer()
{
  if (m_BufferId != 0) glDeleteBuffers(1, &m_BufferId);
}

void OpenGLUniformBuffer::bind() const
{
  glBindBufferBase(GL_UNIFORM_BUFFER, m_BlockIndex, m_BufferId);
}

void OpenGLUniformBuffer::unbind() const
{
  glBindBufferBase(GL_UNIFORM_BUFFER, m_BlockIndex, 0);
}

void OpenGLUniformBuffer::upload_data(const void* data, uint32 size)
{
  if (size > m_Size) { AND_LOG(GraphicsContextLog, And::Error, "Memory overflow in Uniform Buffer!"); return; }
  if (size < m_Size) { AND_LOG(GraphicsContextLog, And::Warning, "Memory underflow in Uniform Buffer!"); }
  if (m_BufferId == 0) { AND_LOG(GraphicsContextLog, And::Error, "Trying to add memory to a not valid uniform buffer!"); return; }

  glBindBuffer(GL_UNIFORM_BUFFER, m_BufferId);
  glBufferSubData(GL_UNIFORM_BUFFER, 0, size, data);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
