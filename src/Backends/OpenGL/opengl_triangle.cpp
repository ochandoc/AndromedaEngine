#include "Common/Triangle.h"

namespace And{


Triangle::Triangle(Vertex v){
  m_vertex = v;
}

Triangle::~Triangle(){}


Vertex& Triangle::get_vertex(){
  return m_vertex;
}

const Vertex& Triangle::get_vertex() const{
  return m_vertex;
}

}