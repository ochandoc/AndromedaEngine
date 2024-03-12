#pragma once
#include "Andromeda/Resources/ResourceGenerator.h"
#include "Andromeda/Graphics/Geometry.h"

namespace And{

class ObjGenerator : public ResourceGenerator<Geometry>{

  public:

  ObjGenerator(){
   std::shared_ptr<Geometry> obj = Geometry::load("cube.obj");
   m_Default = obj;
  }


  virtual std::shared_ptr<Geometry> operator()(const std::string& Path) override{
    std::shared_ptr<Geometry> tmp = Geometry::load(Path);
    if(tmp)return tmp;
    

    return m_Default;
  }
		



	virtual uint64 GenerateId(const std::string& Path) override{
		return std::hash<std::string>{}(Path);
	}

	virtual std::shared_ptr<Geometry> GetDefault() override{
		return m_Default;
	}


private:
	std::shared_ptr<Geometry> m_Default;


};

}