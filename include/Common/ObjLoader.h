#pragma once


namespace And{

  struct Material_info{
    float ambient[3];
    float diffuse[3];
    float specular[3];
  };

class ObjLoader{
  public:
  ObjLoader(const ObjLoader&) = default;
  ObjLoader(ObjLoader&&) = default;

  //static std::optional<ObjLoader> load(std::string path, std::string filename);
  static std::optional<ObjLoader> load(std::string filename);

  std::string get_error();
  std::vector<float> getVertices();


  private:

  ObjLoader(std::vector<float> vertices,  std::vector<float> vertices_weights, std::vector<float> normals, std::vector<float> tex_coords, std::vector<float> colors);
  std::string m_error;

  std::vector<float> m_vertex;            // 'v (x, y, z)
  std::vector<float> m_vertex_weights;    // 'v'(w)
  std::vector<float> m_normals;           // 'vn'
  std::vector<float> m_colors;
  std::vector<float> m_texcoords;

};
}