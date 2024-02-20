#pragma once
#include <optional>
#include <vector>
#include <string>
#include <memory>



namespace And{

  struct Material_info{
    float ambient[3] = {0.0f, 0.0f, 0.0f};
    float diffuse[3] = {0.0f, 0.0f, 0.0f};
    float specular[3] = {0.0f, 0.0f, 0.0f};
  };

  // x, y, z,    nx, ny, nz
  struct Vertex_info{
    float position[3];
    float normal[3];
    float uv[2];
  };

  struct Transform{
    float position[3];
    float rotation[3];
    float scale[3];
  };


class ObjLoader{
  public:
  ObjLoader(const ObjLoader&) = default;
  ObjLoader(ObjLoader&&) = default;
  ~ObjLoader();

  static std::shared_ptr<ObjLoader> load(std::string filename, std::string base_path = "");

  const std::vector<unsigned int>& getIndices();
  const std::vector<Vertex_info>& getVertexInfo();

  void set_VAO(unsigned int v){m_VAO = v;};
  const unsigned int get_vao(){ return m_VAO;};
  
  void set_VBO(unsigned int v){m_VBO = v;};
  const unsigned int get_vbo(){ return m_VBO;};

  const std::vector<Vertex_info>& get_vertex_info(){return m_vertex_info;}

  std::string filename_;

  private:

  ObjLoader(std::vector<unsigned int> indices, std::vector<Vertex_info> vertex_info, Material_info mat);

  std::vector<unsigned int> m_indices;

  std::vector<Vertex_info> m_vertex_info;
  Material_info m_mat_info;

  unsigned int m_VAO = 0;
  unsigned int m_VBO = 0;


};
}