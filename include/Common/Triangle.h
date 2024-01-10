#pragma once

namespace And{

struct Vertex{
  float position[3]; 
  float normal[3]; 
  float color[4]; 
  unsigned int indices[3];
};

class Triangle{

  public:


  Triangle(Vertex v[3]);
  Triangle(){};
  Triangle(const Triangle&) = default;
  Triangle(Triangle&&) = default;
  ~Triangle();


  Triangle& operator=(const Triangle& other) = default;

  Vertex* get_vertex();

  const Vertex* get_vertex() const;

  unsigned int m_vao;
  unsigned int m_vbo;

  private:

  Vertex m_vertex[3];


};
}
