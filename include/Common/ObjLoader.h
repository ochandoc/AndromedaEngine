#pragma once


namespace And{

 
  /*
    struct ObjInfo{
      std::string inputfile;
      //tinyobj::attrib_t attrib;
      //std::vector<tinyobj::shape_t> shapes;
      //std::vector<tinyobj::material_t> materials;
      //std::string err;
      Attribute attrib;
      Shape shapes;
      Material materials;
      std::string err;
    };
  */

class ObjLoader{
  public:
  ObjLoader(const ObjLoader&) = default;
  ObjLoader(ObjLoader&&) = default;

  //static std::optional<ObjLoader> load(std::string path, std::string filename);
  static std::optional<ObjLoader> load(std::string filename);

  //Attribute get_attrib();
  //Shape get_shapes();
  //Material get_materials();

  std::string get_error();


  private:

  ObjLoader();

  float m_points;


  std::string m_error;
  //ObjInfo m_obj_info;


};
}