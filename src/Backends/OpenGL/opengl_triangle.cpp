#include "Common/Triangle.h"

namespace And{


Triangle::Triangle(Vertex v[3]){
  m_vertex[0] = v[0];
  m_vertex[1] = v[1];
  m_vertex[2] = v[2];
}

Triangle::~Triangle(){}


Vertex* Triangle::get_vertex(){
  return m_vertex;
}

const Vertex* Triangle::get_vertex() const{
  return m_vertex;
}

}