#pragma once
#include "Common/ResourceGenerator.h"
#include "Common/ObjLoader.h"

namespace And{

class ObjGenerator : public ResourceGenerator<ObjLoader>{

  public:

  ObjGenerator(){
    m_Default = ObjLoader::load("cube.obj");
  }


  virtual std::shared_ptr<ObjLoader> operator()(const std::string& Path) override{

    std::this_thread::sleep_for(std::chrono::seconds(2));
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