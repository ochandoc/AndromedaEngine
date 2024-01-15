#include "opengl_uniform_buffer.h"

UniformBuffer::UniformBuffer(int block_index, int size){
  unsigned int id;
  glGenBuffers(1, &id);
  glBindBuffer(GL_UNIFORM_BUFFER, id);
  glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_STATIC_DRAW);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);

  m_Id = id;
}

UniformBuffer::~UniformBuffer(){
  glDeleteBuffers(m_Id);
}

void UniformBuffer::upload_data(const void* data, int size){
  glBindBuffer(GL_UNIFORM_BUFFER, m_Id);
  glBufferSubData(GL_UNIFORM_BUFFER, 0, size, data);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);

}

void OpenGLUniformBuffer::bind() const{
  glBindBufferBase(GL_UNIFORM_BUFFER, m_BlockIndex, m_Id);
}

void OpenGLUniformBuffer::unbind() const{
  
  glBindBufferBase(GL_UNIFORM_BUFFER, m_BlockIndex, 0);
  
}