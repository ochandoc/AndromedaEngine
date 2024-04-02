#include "Andromeda/Graphics/Mateial.h"

And::MaterialParam::MaterialParam(const std::string& Name)
  : m_Name(Name), m_Type(EMaterialParamType::None)
{
}

And::MaterialParam::MaterialParam(const MaterialParam& other)
  : m_Name(other.m_Name), m_Type(EMaterialParamType::None)
{
  switch (other.m_Type)
  {
  case EMaterialParamType::Bool:
    SetValueAsBool(other.boolValue);
    break;
  case EMaterialParamType::Int:
    SetValueAsInt(other.intValue);
    break;
  case EMaterialParamType::UInt:
    SetValueAsUInt(other.uintValue);
    break;
  case EMaterialParamType::Float:
    SetValueAsFloat(other.floatValue);
    break;
  case EMaterialParamType::Vec2:
    SetValueAsVec2(other.vec2Value[0], other.vec2Value[1]);
    break;
  case EMaterialParamType::Vec3:
    SetValueAsVec3(other.vec3Value[0], other.vec3Value[1], other.vec3Value[2]);
    break;
  case EMaterialParamType::Vec4:
    SetValueAsVec4(other.vec4Value[0], other.vec4Value[1], other.vec4Value[2], other.vec4Value[3]);
    break;
  case EMaterialParamType::Mat3:
    SetValueAsMat3(other.mat3Value);
    break;
  case EMaterialParamType::Mat4:
    SetValueAsMat4(other.mat4Value);
    break;
  case EMaterialParamType::Texture2D:
    SetValueAsTexture2D(other.texture2DValue);
    break;
  }
}

And::MaterialParam::MaterialParam(MaterialParam&& other)
  : m_Name(other.m_Name), m_Type(EMaterialParamType::None)
{
  switch (other.m_Type)
  {
  case EMaterialParamType::Bool:
    SetValueAsBool(other.boolValue);
    break;
  case EMaterialParamType::Int:
    SetValueAsInt(other.intValue);
    break;
  case EMaterialParamType::UInt:
    SetValueAsUInt(other.uintValue);
    break;
  case EMaterialParamType::Float:
    SetValueAsFloat(other.floatValue);
    break;
  case EMaterialParamType::Vec2:
    SetValueAsVec2(other.vec2Value[0], other.vec2Value[1]);
    break;
  case EMaterialParamType::Vec3:
    SetValueAsVec3(other.vec3Value[0], other.vec3Value[1], other.vec3Value[2]);
    break;
  case EMaterialParamType::Vec4:
    SetValueAsVec4(other.vec4Value[0], other.vec4Value[1], other.vec4Value[2], other.vec4Value[3]);
    break;
  case EMaterialParamType::Mat3:
    SetValueAsMat3(other.mat3Value);
    break;
  case EMaterialParamType::Mat4:
    SetValueAsMat4(other.mat4Value);
    break;
  case EMaterialParamType::Texture2D:
    SetValueAsTexture2D(other.texture2DValue);
    break;
  }
}

And::MaterialParam::~MaterialParam()
{
}

And::MaterialParam& And::MaterialParam::operator=(const MaterialParam& other)
{
  if (this != &other)
  {
    m_Name = other.m_Name;
    m_Type = other.m_Type;
    switch (other.m_Type)
    {
    case EMaterialParamType::Bool:
      SetValueAsBool(other.boolValue);
      break;
    case EMaterialParamType::Int:
      SetValueAsInt(other.intValue);
      break;
    case EMaterialParamType::UInt:
      SetValueAsUInt(other.uintValue);
      break;
    case EMaterialParamType::Float:
      SetValueAsFloat(other.floatValue);
      break;
    case EMaterialParamType::Vec2:
      SetValueAsVec2(other.vec2Value[0], other.vec2Value[1]);
      break;
    case EMaterialParamType::Vec3:
      SetValueAsVec3(other.vec3Value[0], other.vec3Value[1], other.vec3Value[2]);
      break;
    case EMaterialParamType::Vec4:
      SetValueAsVec4(other.vec4Value[0], other.vec4Value[1], other.vec4Value[2], other.vec4Value[3]);
      break;
    case EMaterialParamType::Mat3:
      SetValueAsMat3(other.mat3Value);
      break;
    case EMaterialParamType::Mat4:
      SetValueAsMat4(other.mat4Value);
      break;
    case EMaterialParamType::Texture2D:
      SetValueAsTexture2D(other.texture2DValue);
      break;
    }
  }
  return *this;
}

And::MaterialParam& And::MaterialParam::operator=(MaterialParam&& other)
{
  if (this != &other)
  {
    m_Name.swap(other.m_Name);
    m_Type = other.m_Type;
    switch (other.m_Type)
    {
    case EMaterialParamType::Bool:
      SetValueAsBool(other.boolValue);
      break;
    case EMaterialParamType::Int:
      SetValueAsInt(other.intValue);
      break;
    case EMaterialParamType::UInt:
      SetValueAsUInt(other.uintValue);
      break;
    case EMaterialParamType::Float:
      SetValueAsFloat(other.floatValue);
      break;
    case EMaterialParamType::Vec2:
      SetValueAsVec2(other.vec2Value[0], other.vec2Value[1]);
      break;
    case EMaterialParamType::Vec3:
      SetValueAsVec3(other.vec3Value[0], other.vec3Value[1], other.vec3Value[2]);
      break;
    case EMaterialParamType::Vec4:
      SetValueAsVec4(other.vec4Value[0], other.vec4Value[1], other.vec4Value[2], other.vec4Value[3]);
      break;
    case EMaterialParamType::Mat3:
      SetValueAsMat3(other.mat3Value);
      break;
    case EMaterialParamType::Mat4:
      SetValueAsMat4(other.mat4Value);
      break;
    case EMaterialParamType::Texture2D:
      SetValueAsTexture2D(other.texture2DValue);
      break;
    }
  }
  return *this;
}

void And::MaterialParam::SetValueAsBool(bool value)
{
  m_Type = EMaterialParamType::Bool;
  boolValue = value;
}

void And::MaterialParam::SetValueAsInt(int value)
{
  m_Type = EMaterialParamType::Int;
  intValue = value;
}

void And::MaterialParam::SetValueAsUInt(uint32 value)
{
  m_Type = EMaterialParamType::UInt;
  uintValue = value;
}

void And::MaterialParam::SetValueAsFloat(float value)
{
  m_Type = EMaterialParamType::Float;
  floatValue = value;
}

void And::MaterialParam::SetValueAsVec2(float x, float y)
{
  m_Type = EMaterialParamType::Vec2;
  vec2Value[0] = x;
  vec2Value[1] = y;
}

void And::MaterialParam::SetValueAsVec3(float x, float y, float z)
{
  m_Type = EMaterialParamType::Vec3;
  vec3Value[0] = x;
  vec3Value[1] = y;
  vec3Value[2] = z;
}

void And::MaterialParam::SetValueAsVec4(float x, float y, float z, float w)
{
  m_Type = EMaterialParamType::Vec4;
  vec3Value[0] = x;
  vec3Value[1] = y;
  vec3Value[2] = z;
  vec3Value[3] = w;
}

void And::MaterialParam::SetValueAsMat3(const float* value)
{
  m_Type = EMaterialParamType::Mat3;
  for (int i = 0; i < 9; ++i)
    mat3Value[i] = value[i];
}

void And::MaterialParam::SetValueAsMat4(const float* value)
{
  m_Type = EMaterialParamType::Mat4;
  for (int i = 0; i < 16; ++i)
    mat3Value[i] = value[i];
}

void And::MaterialParam::SetValueAsTexture2D(std::shared_ptr<Texture> value)
{
  m_Type = EMaterialParamType::Texture2D;
  texture2DValue = value;
}

And::Material::Material(const std::string& name)
  : m_Name(name)
{
}

And::Material::~Material()
{
}

void And::Material::AddParam(const std::string& name)
{
  m_MaterialParams.push_back(MaterialParam(name));
  m_ParamIndexs.insert({name, m_MaterialParams.size() - 1});
}

void And::Material::AddParamWithValueAsBool(const std::string& name, bool value)
{
  m_MaterialParams.push_back(MaterialParam(name));
  m_MaterialParams.back().SetValueAsBool(value);
  m_ParamIndexs.insert({ name, m_MaterialParams.size() - 1 });
}

void And::Material::AddParamWithValueAsInt(const std::string& name, int value)
{
  m_MaterialParams.push_back(MaterialParam(name));
  m_MaterialParams.back().SetValueAsInt(value);
  m_ParamIndexs.insert({ name, m_MaterialParams.size() - 1 });
}

void And::Material::AddParamWithValueAsUInt(const std::string& name, uint32 value)
{
  m_MaterialParams.push_back(MaterialParam(name));
  m_MaterialParams.back().SetValueAsUInt(value);
  m_ParamIndexs.insert({ name, m_MaterialParams.size() - 1 });
}

void And::Material::AddParamWithValueAsFloat(const std::string& name, float value)
{
  m_MaterialParams.push_back(MaterialParam(name));
  m_MaterialParams.back().SetValueAsFloat(value);
  m_ParamIndexs.insert({ name, m_MaterialParams.size() - 1 });
}

void And::Material::AddParamWithValueAsVec2(const std::string& name, float x, float y)
{
  m_MaterialParams.push_back(MaterialParam(name));
  m_MaterialParams.back().SetValueAsVec2(x, y);
  m_ParamIndexs.insert({ name, m_MaterialParams.size() - 1 });
}

void And::Material::AddParamWithValueAsVec3(const std::string& name, float x, float y, float z)
{
  m_MaterialParams.push_back(MaterialParam(name));
  m_MaterialParams.back().SetValueAsVec3(x, y, z);
  m_ParamIndexs.insert({ name, m_MaterialParams.size() - 1 });
}

void And::Material::AddParamWithValueAsVec4(const std::string& name, float x, float y, float z, float w)
{
  m_MaterialParams.push_back(MaterialParam(name));
  m_MaterialParams.back().SetValueAsVec4(x, y, z, w);
  m_ParamIndexs.insert({ name, m_MaterialParams.size() - 1 });
}

void And::Material::AddParamWithValueAsMat3(const std::string& name, float* value)
{
  m_MaterialParams.push_back(MaterialParam(name));
  m_MaterialParams.back().SetValueAsMat3(value);
  m_ParamIndexs.insert({ name, m_MaterialParams.size() - 1 });
}

void And::Material::AddParamWithValueAsMat4(const std::string& name, float* value)
{
  m_MaterialParams.push_back(MaterialParam(name));
  m_MaterialParams.back().SetValueAsMat4(value);
  m_ParamIndexs.insert({ name, m_MaterialParams.size() - 1 });
}

void And::Material::AddParamWithValueAsTexture2D(const std::string& name, std::shared_ptr<Texture> value)
{
  m_MaterialParams.emplace_back(name);
  m_MaterialParams.back().SetValueAsTexture2D(value);
  m_ParamIndexs.insert({ name, m_MaterialParams.size() - 1 });
}

And::MaterialParam* And::Material::TryGetParam(const std::string& name)
{
  if (m_ParamIndexs.contains(name))
  {
    uint64 index = m_ParamIndexs[name];
    return &m_MaterialParams[index];
  }

  return nullptr;
}

const And::MaterialParam* And::Material::TryGetParam(const std::string& name) const
{
  if (m_ParamIndexs.contains(name))
  {
    uint64 index = m_ParamIndexs.at(name);
    return &m_MaterialParams[index];
  }

  return nullptr;
}
