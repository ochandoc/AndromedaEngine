#include "Andromeda/Graphics/Triangle.h"
#include "Backends/OpenGL/OpenGL.h"
#include "GLFW/glfw3.h"
#include <string>
#include <iostream>
#include <sstream>

namespace And{


Triangle::Triangle(Vertext v[3]){
  m_vertex[0] = v[0];
  m_vertex[1] = v[1];
  m_vertex[2] = v[2];


  glGenVertexArrays(1, &m_vao);
  glGenBuffers(1, &m_vbo);


}

Triangle::~Triangle(){}


Vertext* Triangle::get_vertex(){
  return m_vertex;
}

const Vertext* Triangle::get_vertex() const{
  return m_vertex;
}

}