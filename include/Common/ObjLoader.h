#pragma once
#include <optional>
#include <vector>
#include <string>


namespace And{

  struct Material_info{
    float ambient[3] = {0.0f, 0.0f, 0.0f};
    float diffuse[3] = {0.0f, 0.0f, 0.0f};
    float specular[3] = {0.0f, 0.0f, 0.0f};
  };

class ObjLoader{
  public:
  ObjLoader(const ObjLoader&) = default;
  ObjLoader(ObjLoader&&) = default;

  //static std::optional<ObjLoader> load(std::string path, std::string filename);
  static std::optional<ObjLoader> load(std::string filename, std::string base_path = "");

  const std::vector<float>& getVertices();
  const std::vector<float>& getNormals();
  const std::vector<float>& getColors();
  const std::vector<float>& getTexCoords();
  const Material_info& getMaterialInfo();


  private:

  ObjLoader(std::vector<float> vertices,  std::vector<float> vertices_weights, std::vector<float> normals, std::vector<float> tex_coords, std::vector<float> colors, Material_info mat);

  std::vector<float> m_vertex;            // 'v (x, y, z)
  std::vector<float> m_vertex_weights;    // 'v'(w)
  std::vector<float> m_normals;           // 'vn'
  std::vector<float> m_colors;
  std::vector<float> m_texcoords;

  Material_info m_mat_info;

};
}