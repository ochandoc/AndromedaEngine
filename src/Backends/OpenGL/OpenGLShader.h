#pragma once

#include "Andromeda/Graphics/Shader.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace And
{
  enum class EUniformType : uint8
  {
    Float,
    Int,
    Vec2,
    Vec3,
    Vec4,

    Mat2,
    Mat3,
    Mat4,

    Sampler1D,
    Sampler2D,
    Sampler3D,

    UniformBlock,
  };

  enum class EUniformBlockType : uint8
  {
    UniformBuffer0 = 1 << 0, /**  Uniform block for the matrices */
    UniformBuffer1 = 1 << 1, /**  Uniform block for the point light cube map */
    UniformBuffer2 = 1 << 2, /**  Uniform buffer of the ambient light */
    UniformBuffer3 = 1 << 3, /**  Uniform buffer of the directional light */
    UniformBuffer4 = 1 << 4, /**  Uniform buffer of the point light */
    UniformBuffer5 = 1 << 5, /**  Uniform buffer of the spot light */
  };

  struct ShaderPreProcessInfo
  {
    std::unordered_map<uint32, std::string> ShaderSources;
    std::unordered_map<std::string, EUniformType> SimpleUniforms;
    std::unordered_map<uint8, std::string> UniformBlocks;
    uint8 UniformBuffers;
  };

  struct UniformInfo
  {
    EUniformType Type;
    int32 Location;
  };

  class OpenGLShader : public Shader
  {
  private:
    OpenGLShader();

  public:
    explicit OpenGLShader(ENoInit);

    virtual ~OpenGLShader();

    static std::shared_ptr<OpenGLShader> Make(const std::string& Path);

    virtual void Use() const override;
    virtual void StopUsing() const override;

    inline uint8 GetUniformBlocks() const { return m_UniformBlocks; }
    inline uint8 GetNumTextures() const { return m_NumTextures; }

    int32 GetUniformBlockSize(EUniformBlockType type) const;

    void SetFloat(const std::string& Name, float value);
    void SetInt(const std::string& Name, int32 value);
    void SetVec2(const std::string& Name, const glm::vec2& vec);
    void SetVec3(const std::string& Name, const glm::vec3& vec);
    void SetVec4(const std::string& Name, const glm::vec4& vec);
    void SetMat2(const std::string& Name, const glm::mat2& mat);
    void SetMat3(const std::string& Name, const glm::mat3& mat);
    void SetMat4(const std::string& Name, const glm::mat4& mat);
    void SetTexture(const std::string& Name, int8 Slot);

  private:
    static std::string ReadFile(const std::string& Path);

  private:
    uint32 m_Id;
    uint8 m_UniformBlocks;
    uint8 m_NumTextures;
    std::unordered_map<std::string, UniformInfo> m_Uniforms;
    std::unordered_map<EUniformBlockType, int32> m_UniformBlocksSizes;
  };

  enum class ELayoutSpecification : uint8
  {
    Binding,
    Location,
  };

  class OpenGLShaderPreProcessor
  {
  public:
    static ShaderPreProcessInfo PreProcess(const std::string& Source);

  private:
    static std::unordered_map<uint32, std::string> SplitSource(const std::string& source);
    static uint8 GetUniformBlockFromShaderSource(const std::string& source, std::unordered_map<uint8, std::string>& UniformBlocks);
    static void GetSingleUniforms(const std::string& source, std::unordered_map<std::string, EUniformType>& Uniforms);
    static EUniformType GetUniformTypeFromString(const std::string& uniform);
    static int32 GetUniformLayoutSpecificationAsInt(const std::string& UniformSrc, ELayoutSpecification specification);
  };

  class OpenGLShaderCompiler
  {
  public:
    static uint32 CompileShaders(const std::unordered_map<uint32, std::string>& Sources);

  private:
    static uint32 CompileShader(uint32 type, const std::string& Source);
    static bool CheckCompileStatus(uint32 ID);
  };
}