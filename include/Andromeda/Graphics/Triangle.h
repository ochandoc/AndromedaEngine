#pragma once

namespace And{

struct Vertext{
  float position[3]; 
  float normal[3]; 
  float color[4]; 
  unsigned int indices[3];
};

class Triangle{

  public:


  Triangle(Vertext v[3]);
  Triangle(){};
  Triangle(const Triangle&) = default;
  Triangle(Triangle&&) = default;
  ~Triangle();


  Triangle& operator=(const Triangle& other) = default;

  Vertext* get_vertex();

  const Vertext* get_vertex() const;

  unsigned int m_vao;
  unsigned int m_vbo;

  private:

  Vertext m_vertex[3];


};
}
