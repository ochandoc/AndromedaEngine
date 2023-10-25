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
  Triangle(const Triangle&) = delete;
  Triangle(Triangle&&) = delete;
  ~Triangle();

  Vertex* get_vertex();

  const Vertex* get_vertex() const;


  private:

  Vertex m_vertex[3];

};
}