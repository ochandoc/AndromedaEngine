#pragma once
#include "Andromeda/Resources/ResourceGenerator.h"
#include "Andromeda/Graphics/Shader.h"

namespace And{

class ShaderGenerator : public ResourceGenerator<Shader>{

  public:

  ShaderGenerator(){

    std::shared_ptr<Shader> shader = Shader::make("default/deafult_shader.shader");
    m_Default = shader;
  }
  ShaderGenerator(const ShaderGenerator&);
  ShaderGenerator(ShaderGenerator&&);
  ~ShaderGenerator(){}

  virtual std::shared_ptr<Shader> operator()(const std::string& path) override{
    std::shared_ptr<Shader> tmp = Shader::make(path);
    if(tmp)return tmp;

    return m_Default;
  }

  virtual uint64 GenerateId(const std::string& Path) override{
		return std::hash<std::string>{}(Path);
	}

	virtual std::shared_ptr<Shader> GetDefault() override{
		return m_Default;
	}

  private:

  std::shared_ptr<Shader> m_Default;
};

}