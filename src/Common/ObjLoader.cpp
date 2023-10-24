#include "Common/ObjLoader.h"

namespace And{


std::optional<ObjLoader> ObjLoader::load(std::string path){

  ObjInfo obj_info;

  std::string err;
  std::string warn;
  bool error = tinyobj::LoadObj(&(obj_info.attrib), &(obj_info.shapes), &(obj_info.materials), &err, path.c_str(), NULL, true); 
  //bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename, basepath, triangulate);

  if (!err.empty()) {
    m_obj_info.err = err;
  }

  if (!error) {
      //std::cerr << "Failed to load/parse .obj" << std::endl;
      //return optional;
  }

  ObjLoader obj{obj_info};

  return std::optional<ObjLoader>(std::move(obj));

}

ObjLoader::ObjLoader(ObjInfo info) : m_obj_info(info){

}

ObjLoader::ObjLoader(const ObjLoader& other){

}

ObjLoader::ObjLoader(ObjLoader&& other){

}

Attribute ObjLoader::get_attrib(){

}

std::vector<Shape> ObjLoader::get_shapes(){

}

std::vector<Material> ObjLoader::get_materials(){

}


std::string ObjLoader::get_error(){

}

}