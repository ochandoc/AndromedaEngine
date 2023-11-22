#include "Common/Triangle.h"
#include "GL/glew.h"
#include "GLFW/glfw3.h"

namespace And{


Triangle::Triangle(Vertex v[3]){
  m_vertex[0] = v[0];
  m_vertex[1] = v[1];
  m_vertex[2] = v[2];


  glGenVertexArrays(1, &m_vao);
  glGenBuffers(1, &m_vbo);


}

Triangle::~Triangle(){}


Vertex* Triangle::get_vertex(){
  return m_vertex;
}

const Vertex* Triangle::get_vertex() const{
  return m_vertex;
}


bool Triangle::Load(){
  return true;
}

bool Triangle::Save(){

  return true;
}

}