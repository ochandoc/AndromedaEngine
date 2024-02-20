#include "opengl_uniform_buffer.h"


namespace And{


UniformBuffer::UniformBuffer(unsigned int block_index, unsigned int size){
  unsigned int id_ubo;
  
  glGenBuffers(1, &id_ubo);
  glBindBuffer(GL_UNIFORM_BUFFER, id_ubo);
  printf("Size del buffer antes de hacer buffer data: %u\n", size);
  glBufferData(GL_UNIFORM_BUFFER, size, 0, GL_DYNAMIC_DRAW);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);

  m_Id = id_ubo;
  m_BlockIndex = block_index;
  m_Size = size;
}

UniformBuffer::~UniformBuffer(){
  glDeleteBuffers(1, &m_Id);
}

}

}

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