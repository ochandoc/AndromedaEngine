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


class Geometry{
  public:
  Geometry(const Geometry&) = default;
  Geometry(Geometry&&) = default;
  ~Geometry();

  /**
   * @brief Creates a Geometry based on a .obj
   * 
   * @param filename 
   * @param base_path path of the folder container (optional, you shouldn't use, working progress)
   * @return std::shared_ptr<Geometry> 
   */
  static std::shared_ptr<Geometry> load(std::string filename, std::string base_path = "");

  /**
   * @brief Get the Indices of the geometry
   * 
   * @return const std::vector<unsigned int>& 
   */
  const std::vector<unsigned int>& getIndices();

  /**
   * @brief Get the Vertex Info struct
   * 
   * @return const std::vector<Vertex_info>& 
   */
  const std::vector<Vertex_info>& getVertexInfo();

  /**
   * @brief Set VAO of object
   * 
   * @param v 
   */
  void set_VAO(unsigned int v){m_VAO = v;};

  /**
   * @brief Get the VAO object
   * 
   * @return const unsigned int 
   */
  const unsigned int get_vao(){ return m_VAO;};
  
  /**
   * @brief Set the VBO of the geometry
   * 
   * @param v 
   */
  void set_VBO(unsigned int v){m_VBO = v;};

  /**
   * @brief Get the VBO
   * 
   * @return const unsigned int 
   */
  const unsigned int get_vbo(){ return m_VBO;};


  const std::vector<Vertex_info>& get_vertex_info(){return m_vertex_info;}

  /**
   * @brief Geometry file name
   * 
   */
  std::string filename_;

  private:

  Geometry(std::vector<unsigned int> indices, std::vector<Vertex_info> vertex_info, Material_info mat);

  std::vector<unsigned int> m_indices;

  std::vector<Vertex_info> m_vertex_info;
  Material_info m_mat_info;

  unsigned int m_VAO = 0;
  unsigned int m_VBO = 0;


};
}