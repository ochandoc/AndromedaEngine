#include "Common/ObjLoader.h"
#include "tiny_obj_loader.h"

namespace And{


std::optional<ObjLoader> ObjLoader::load(std::string filename){




  std::cout << "Loading obj... " << filename << std::endl;

  tinyobj::attrib_t attrib;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;
  std::string warn;
  std::string err;

  // Si le pasamos la ruta y luego el nombre, cogera los .mtl del directorio
  bool error = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.c_str());

  if (!err.empty()) {
    //m_obj_info.replace(0, err.length(), err.c_str());
    printf("Obj not loaded correctly %s\n", err.c_str());
  }else{
    printf("Obj loaded correctly\n");
  }

  if (!error) {
      //std::cerr << "Failed to load/parse .obj" << std::endl;
      return std::nullopt;
  }

  ObjLoader obj{};

  return std::optional<ObjLoader>(std::move(obj));

}

ObjLoader::ObjLoader(){

}


std::string ObjLoader::get_error(){
  return m_error;
}

}