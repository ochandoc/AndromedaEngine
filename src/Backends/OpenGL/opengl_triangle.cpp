#include "Common/Triangle.h"
#include "GL/glew.h"

namespace And{


Triangle::Triangle(Vertex v[3]){
  m_vertex[0] = v[0];
  m_vertex[1] = v[1];
  m_vertex[2] = v[2];

}

Triangle::~Triangle(){
  // delete buffers
  glDeleteBuffers(1, &m_VAO);
  glDeleteBuffers(1, &m_VBO);
}


Vertex* Triangle::get_vertex(){
  return m_vertex;
}

const Vertex* Triangle::get_vertex() const{
  return m_vertex;
}

void Triangle::set_buffers(unsigned int vao, unsigned int vbo){
  m_VAO = vao;
  m_VBO = vbo;
  buffers_created = true;
}

const unsigned int Triangle::get_vao(){
  return m_VAO;
}

const unsigned int Triangle::get_vbo(){
  return m_VBO;
}


}