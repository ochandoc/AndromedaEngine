#pragma once
#include "Andromeda/Resources/ResourceGenerator.h"
#include "Andromeda/Graphics/Shader.h"

namespace And{

class ShaderGenerator : public ResourceGenerator<OldShader>{

  public:

    ShaderGenerator(){

    std::shared_ptr<OldShader> OldShader = OldShader::make("default/deafult_shader.shader");
    m_Default = OldShader;
  }
  ShaderGenerator(const ShaderGenerator&);
  ShaderGenerator(ShaderGenerator&&);
  ~ShaderGenerator(){}

  virtual std::shared_ptr<OldShader> operator()(const std::string& path) override{
    std::shared_ptr<OldShader> tmp = OldShader::make(path);
    if(tmp)return tmp;

    return m_Default;
  }

  virtual uint64 GenerateId(const std::string& Path) override{
		return std::hash<std::string>{}(Path);
	}

	virtual std::shared_ptr<OldShader> GetDefault() override{
		return m_Default;
	}

  private:

  std::shared_ptr<OldShader> m_Default;
};

}