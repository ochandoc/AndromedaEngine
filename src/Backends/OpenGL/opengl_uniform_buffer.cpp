#include "opengl_uniform_buffer.h"


namespace And{


UniformBuffer::UniformBuffer(unsigned int block_index, unsigned int size){
  unsigned int id;
  glGenBuffers(1, &id);
  glBindBuffer(GL_UNIFORM_BUFFER, id);
  glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_DYNAMIC_DRAW);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);

  m_Id = id;
  m_BlockIndex = block_index;
  m_Size = size;
}

UniformBuffer::~UniformBuffer(){
  glDeleteBuffers(m_Size, &m_Id);
}

void UniformBuffer::upload_data(const void* data, unsigned int size){
  //glBindBuffer(GL_UNIFORM_BUFFER, m_Id);
  //glBufferSubData(GL_UNIFORM_BUFFER, 0, size, data);
  glNamedBufferSubData(m_Id, 0, size, data);
  //glBindBuffer(GL_UNIFORM_BUFFER, 0);

}

void UniformBuffer::bind() const{
  glBindBufferBase(GL_UNIFORM_BUFFER, m_BlockIndex, m_Id);
}

void UniformBuffer::unbind() const{
  glBindBufferBase(GL_UNIFORM_BUFFER, m_BlockIndex, 0);
}

}