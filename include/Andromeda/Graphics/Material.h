#pragma once

#include "Andromeda/Misc/CoreMiscDefines.h"
#include "Andromeda/HAL/Types.h"
#include "Andromeda/Graphics/Texture.h"

namespace And
{
  enum class EMaterialParamType
  {
    None,
    Int,
    UInt,
    Float,
    Vec2,
    Vec3,
    Vec4,
    Mat3,
    Mat4,
    Texture2D,
  };

  class MaterialParam
  {
  public:
    explicit MaterialParam(const std::string& Name);
    MaterialParam(const MaterialParam& other);
    MaterialParam(MaterialParam&& other);

    ~MaterialParam();

    MaterialParam& operator=(const MaterialParam& other);
    MaterialParam& operator=(MaterialParam&& other);

    void SetValueAsInt(int value);
    void SetValueAsUInt(uint32 value);
    void SetValueAsFloat(float value);
    void SetValueAsVec2(float x, float y);
    void SetValueAsVec3(float x, float y, float z);
    void SetValueAsVec4(float x, float y, float z, float w);
    void SetValueAsMat3(const float* value);
    void SetValueAsMat4(const float* value);
    void SetValueAsTexture2D(std::shared_ptr<Texture> value);
    
    inline int GetValueAsInt() const { return intValue; }
    inline uint32 GetValueAsUInt() const { return uintValue; }
    inline float GetValueAsFloat() const { return floatValue; }
    inline const float* GetValueAsVec2() const { return vec2Value; }
    inline const float* GetValueAsVec3() const { return vec3Value; }
    inline const float* GetValueAsVec4() const { return vec4Value; }
    inline const float* GetValueAsMat3() const { return mat3Value; }
    inline const float* GetValueAsMat4() const { return mat4Value; }
    inline std::shared_ptr<Texture> GetValueAsTexture2D() const { return texture2DValue; }

    inline EMaterialParamType GetType() const { return m_Type; }
    inline const std::string& GetName() const { return m_Name; }

  private:
    std::string m_Name;
    EMaterialParamType m_Type;
    union
    {
      int intValue;
      uint32 uintValue;
      float floatValue;
      float vec2Value[2];
      float vec3Value[3];
      float vec4Value[4];
      float mat3Value[3 * 3];
      float mat4Value[4 * 4];
      std::shared_ptr<Texture> texture2DValue;
    };
  };

  class Material
  {
  public:
    explicit Material(const std::string& name);

    ~Material();

    void AddParam(const std::string& name);
    void AddParamWithValueAsInt(const std::string& name, int value);
    void AddParamWithValueAsUInt(const std::string& name, uint32 value);
    void AddParamWithValueAsFloat(const std::string& name, float value);
    void AddParamWithValueAsVec2(const std::string& name, float x, float y);
    void AddParamWithValueAsVec3(const std::string& name, float x, float y, float z);
    void AddParamWithValueAsVec4(const std::string& name, float x, float y, float z, float w);
    void AddParamWithValueAsMat3(const std::string& name, float* value);
    void AddParamWithValueAsMat4(const std::string& name, float* value);
    void AddParamWithValueAsTexture2D(const std::string& name, std::shared_ptr<Texture> value);

    MaterialParam* TryGetParam(const std::string& name);
    const MaterialParam* TryGetParam(const std::string& name) const;
    
    inline uint64 GetNumParams() const { return m_MaterialParams.size(); }
    inline const std::string& GetName() const { return m_Name; }
    inline std::vector<MaterialParam>& GetParams() { return m_MaterialParams; }
    inline const std::vector<MaterialParam>& GetParams() const { return m_MaterialParams; }

  private:
    std::string m_Name;
    std::vector<MaterialParam> m_MaterialParams;
    std::unordered_map<std::string, uint64> m_ParamIndexs;
  };
}