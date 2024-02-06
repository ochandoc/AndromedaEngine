#pragma once
#include "Andromeda/Resources/ResourceGenerator.h"
#include "Andromeda/Graphics/ObjLoader.h"

namespace And{

class ObjGenerator : public ResourceGenerator<ObjLoader>{

  public:

  ObjGenerator(){
   std::shared_ptr<ObjLoader> obj = ObjLoader::load("cube.obj");
   m_Default = obj;
  }


  virtual std::shared_ptr<ObjLoader> operator()(const std::string& Path) override{
    std::shared_ptr<ObjLoader> tmp = ObjLoader::load(Path);
    if(tmp)return tmp;
    

    return m_Default;
  }
		



	virtual uint64 GenerateId(const std::string& Path) override{
		return std::hash<std::string>{}(Path);
	}

	virtual std::shared_ptr<ObjLoader> GetDefault() override{
		return m_Default;
	}


private:
	std::shared_ptr<ObjLoader> m_Default;


};

}