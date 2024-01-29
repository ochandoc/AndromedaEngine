#include "opengl_uniform_buffer.h"


namespace And{


UniformBuffer::UniformBuffer(unsigned int block_index, unsigned int size){
  unsigned int id;
  //GLenum err = glGetError();
  //printf("Error-> %d\n", err);
  glGenBuffers(1, &id);
  //err = glGetError();
  //printf("Error-> %d\n", err);
  glBindBuffer(GL_UNIFORM_BUFFER, id);
  //err = glGetError();
  //printf("Error-> %d\n", err);
  glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_DYNAMIC_DRAW);
  //err = glGetError();
  //printf("Error-> %d\n", err);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
  //glNamedBufferData(id, size, nullptr, GL_DYNAMIC_DRAW);
  //err = glGetError();
  //printf("Error-> %d\n", err);

  m_Id = id;
  m_BlockIndex = block_index;
  m_Size = size;
}

UniformBuffer::~UniformBuffer(){
  glDeleteBuffers(m_Size, &m_Id);
}

void UniformBuffer::upload_data(const void* data, unsigned int size){
  glBindBuffer(GL_UNIFORM_BUFFER, m_Id);
  glBufferSubData(GL_UNIFORM_BUFFER, 0, size, data);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
  //glNamedBufferSubData(m_Id, 0, size, data);

}

// Estas funciones no hacen falta si hago el glNamedBufferSubData
void UniformBuffer::bind() const{
  glBindBufferBase(GL_UNIFORM_BUFFER, m_BlockIndex, m_Id);
}

void UniformBuffer::unbind() const{
  glBindBufferBase(GL_UNIFORM_BUFFER, m_BlockIndex, 0);
}

}