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

  bool buffers_created = false;

  void set_buffers(unsigned int vao, unsigned int vbo);
  
  const unsigned int get_vao();
  const unsigned int get_vbo();

  private:

  Vertex m_vertex[3];
  unsigned int m_VAO;
  unsigned int m_VBO;

};
}
