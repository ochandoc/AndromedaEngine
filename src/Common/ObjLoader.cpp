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

  std::vector<float> vertices;
  std::vector<float> vertices_wheights;
  std::vector<float> colors;
  std::vector<float> normals;
  std::vector<float> tex_coords;

  // Si le pasamos la ruta y luego el nombre, cogera los .mtl del directorio
  bool error = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.c_str());

  if (!err.empty()) {
    //m_obj_info.replace(0, err.length(), err.c_str());
    printf("Obj not loaded correctly %s\n", err.c_str());
    return std::nullopt;
  }else{
    printf("Obj loaded correctly\n");

    // Guardar vertices en vector de floats
    vertices = attrib.GetVertices();
    vertices_wheights = attrib.GetVertexWeights();
    normals = attrib.normals;
    tex_coords = attrib.texcoords;
    colors = attrib.colors;
    //vertices.swap(m_vertices);


  }



  ObjLoader obj{vertices, vertices_wheights, normals, tex_coords, colors };
  return std::optional<ObjLoader>(std::move(obj));
}

ObjLoader::ObjLoader(std::vector<float> v, std::vector<float> v_w, std::vector<float> normals, std::vector<float> tex_coords, std::vector<float> colors) : m_vertex(v), m_vertex_weights(v_w), m_normals(normals), m_texcoords(tex_coords), m_colors(colors){

}


std::string ObjLoader::get_error(){
  return m_error;
}

}