#pragma once
#include "tiny_obj_loader.h"

namespace And{
class ObjLoader{

  // Tiny obj wrappers
  struct Attribute{
    tinyobj::attrib_t attrib;
  };

  struct Shape{
     std::vector<tinyobj::shape_t> shapes;
  };

  struct Material{
    std::vector<tinyobj::material_t> materials;
  };

  struct ObjInfo{
    std::string inputfile;
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string err;
  };

  public:
  ObjLoader(const ObjLoader&);
  ObjLoader(ObjLoader&&);

  static std::optional<ObjLoader> load(std::string path);

  Attribute get_attrib();
  std::vector<Shape> get_shapes();
  std::vector<Material> get_materials();

  std::string get_error();


  private:
  ObjLoader();

  ObjInfo m_obj_info;


};
}