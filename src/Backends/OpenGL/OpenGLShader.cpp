#include "Backends/OpenGL/OpenGLShader.h"
#include "Backends/OpenGL/OpenGL.h"

#include <algorithm>
#include <assert.h>

static const char* UniformTypesStr[] = { "float", "int", "vec2", "vec3", "vec4", "mat2", "mat3", "mat4", "sampler1D", "sampler2D", "sampler3D" };

namespace And
{
  uint32 GetShaderTypeFromString(const std::string& typeStr)
  {
    if (typeStr.find("Vertex") != std::string::npos)
    {
      return GL_VERTEX_SHADER;
    }

    if (typeStr.find("Fragment") != std::string::npos)
    {
      return GL_FRAGMENT_SHADER;
    }

    return 0;
  }

  OpenGLShader::OpenGLShader() 
    : m_Id(0), m_NumTextures(0), m_UniformBlocks(0)
  {
  }

  OpenGLShader::OpenGLShader(ENoInit) : OpenGLShader()
  {
  }

  OpenGLShader::~OpenGLShader()
  {
    glDeleteProgram(m_Id);
  }

  std::shared_ptr<OpenGLShader> OpenGLShader::Make(const std::string& Path)
  {
    std::shared_ptr<OpenGLShader> shader(new OpenGLShader);

    std::string source = ReadFile(Path);
    ShaderPreProcessInfo PreProcessInfo = OpenGLShaderPreProcessor::PreProcess(source);
    
    uint32 programId = OpenGLShaderCompiler::CompileShaders(PreProcessInfo.ShaderSources);

    if (programId != 0)
    {
      shader->m_Id = programId;
      shader->m_UniformBlocks = PreProcessInfo.UniformBuffers;

      for (auto& [name, type] : PreProcessInfo.SimpleUniforms)
      {
        int32 location = glGetUniformLocation(programId, name.c_str());
        shader->m_Uniforms[name] = { type, location };

        EUniformType enumType = EUniformType(type);
        if (enumType == EUniformType::Sampler1D || enumType == EUniformType::Sampler2D || enumType == EUniformType::Sampler3D)
          shader->m_NumTextures++;
      }

      for (auto& [type, name] : PreProcessInfo.UniformBlocks)
      {
        uint32 blockIndex = glGetUniformBlockIndex(programId, name.c_str());
        int32 size = 0;
        glGetActiveUniformBlockiv(programId, blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &size);
        shader->m_UniformBlocksSizes.insert({ EUniformBlockType(type), size});
        uint32 desiredIndex = (uint32)log2f(type);
        glUniformBlockBinding(programId, blockIndex, desiredIndex);
      }
    }
    else
    {
      shader.reset();
    }


    return shader;
  }

  void OpenGLShader::Use() const
  {
    glUseProgram(m_Id);
  }

  void OpenGLShader::StopUsing() const
  {
    glUseProgram(0);
  }

  int32 OpenGLShader::GetUniformBlockSize(EUniformBlockType type) const
  {
    EUniformBlockType enumType = EUniformBlockType(type);
    if (!m_UniformBlocksSizes.contains(enumType))
      return -1;

    return m_UniformBlocksSizes.at(enumType);
  }

  void OpenGLShader::SetFloat(const std::string& Name, float value)
  {
    assert(m_Uniforms.contains(Name));
    assert(m_Uniforms[Name].Type == EUniformType::Float);

    int32 location = m_Uniforms[Name].Location;
    if (location != -1)
    {
      glUniform1f(location, value);
    }
  }

  void OpenGLShader::SetInt(const std::string& Name, int32 value)
  {
    assert(m_Uniforms.contains(Name));
    assert(m_Uniforms[Name].Type == EUniformType::Int);

    int32 location = m_Uniforms[Name].Location;
    if (location != -1)
    {
      glUniform1i(location, value);
    }
  }

  void OpenGLShader::SetVec2(const std::string& Name, const glm::vec2& vec)
  {
    assert(m_Uniforms.contains(Name));
    assert(m_Uniforms[Name].Type == EUniformType::Vec2);

    int32 location = m_Uniforms[Name].Location;
    if (location != -1)
    {
      glUniform2f(location, vec.x, vec.y);
    }
  }

  void OpenGLShader::SetVec3(const std::string& Name, const glm::vec3& vec)
  {
    assert(m_Uniforms.contains(Name));
    assert(m_Uniforms[Name].Type == EUniformType::Vec3);

    int32 location = m_Uniforms[Name].Location;
    if (location != -1)
    {
      glUniform3f(location, vec.x, vec.y, vec.z);
    }
  }

  void OpenGLShader::SetVec4(const std::string& Name, const glm::vec4& vec)
  {
    assert(m_Uniforms.contains(Name));
    assert(m_Uniforms[Name].Type == EUniformType::Vec4);

    int32 location = m_Uniforms[Name].Location;
    if (location != -1)
    {
      glUniform4f(location, vec.x, vec.y, vec.z, vec.w);
    }
  }

  void OpenGLShader::SetMat2(const std::string& Name, const glm::mat2& mat)
  {
    assert(m_Uniforms.contains(Name));
    assert(m_Uniforms[Name].Type == EUniformType::Mat2);

    int32 location = m_Uniforms[Name].Location;
    if (location != -1)
    {
      glUniformMatrix2fv(location, 1, GL_FALSE, glm::value_ptr(mat));
    }
  }

  void OpenGLShader::SetMat3(const std::string& Name, const glm::mat3& mat)
  {
    assert(m_Uniforms.contains(Name));
    assert(m_Uniforms[Name].Type == EUniformType::Mat3);

    int32 location = m_Uniforms[Name].Location;
    if (location != -1)
    {
      glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(mat));
    }
  }

  void OpenGLShader::SetMat4(const std::string& Name, const glm::mat4& mat)
  {
    assert(m_Uniforms.contains(Name));
    assert(m_Uniforms[Name].Type == EUniformType::Mat4);

    int32 location = m_Uniforms[Name].Location;
    if (location != -1)
    {
      glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
    }
  }

  void OpenGLShader::SetTexture(const std::string& Name, int8 Slot)
  {
    assert(m_Uniforms.contains(Name));
    assert(m_Uniforms[Name].Type == EUniformType::Sampler1D || 
           m_Uniforms[Name].Type == EUniformType::Sampler2D || 
           m_Uniforms[Name].Type == EUniformType::Sampler3D);

    int32 location = m_Uniforms[Name].Location;
    if (location != -1)
    {
      glUniform1i(location, Slot);
    }
  }

  void OpenGLShader::SetTextureInArray(const std::string& Name, uint32 index, uint8 Slot)
  {
    std::string nameIndexed = Name + "[" + std::to_string(index) + "]";
    int32 location = glGetUniformLocation(m_Id, nameIndexed.c_str());
    if (location != -1)
    {
      glUniform1i(location, Slot);
    }
  }

  std::string OpenGLShader::ReadFile(const std::string& Path)
  {
    std::string source;

    std::ifstream inFile(Path, std::ios::in | std::ios::binary);
    if (inFile)
    {
      inFile.seekg(0, std::ios::end);
      uint64 size = inFile.tellg();
      if (size != -1)
      {
        source.resize(size);
        inFile.seekg(0, std::ios::beg);
        inFile.read(&source[0], size);
      }
    }

    return source;
  }

  ShaderPreProcessInfo OpenGLShaderPreProcessor::PreProcess(const std::string& source)
  {
    ShaderPreProcessInfo PreProcessInfo;
    PreProcessInfo.ShaderSources = SplitSource(source);

    uint8 UniformBuffers = 0;
    for (auto& [type, src] : PreProcessInfo.ShaderSources)
    {
      UniformBuffers |= GetUniformBlockFromShaderSource(src, PreProcessInfo.UniformBlocks);
      GetSingleUniforms(src, PreProcessInfo.SimpleUniforms);
    }
    PreProcessInfo.UniformBuffers = UniformBuffers;

    return PreProcessInfo;
  }

  std::unordered_map<uint32, std::string> OpenGLShaderPreProcessor::SplitSource(const std::string& source)
  {
    std::unordered_map<uint32, std::string> sources;

    const char* typeToken = "#type";
    const uint64 typeTokenLength = strlen(typeToken);
    uint64 pos = source.find(typeToken, 0);
    while (pos != std::string::npos)
    {
      uint64 endOfLine = source.find_first_of("\r\n", pos);
      uint64 begin = pos + typeTokenLength + 1;
      std::string type = source.substr(begin, endOfLine - begin);

      uint64 nextLinePos = source.find_first_not_of("\r\n", endOfLine);
      pos = source.find(typeToken, nextLinePos);

      if (pos == std::string::npos)
      {
        sources[GetShaderTypeFromString(type)] = source.substr(nextLinePos);
      }
      else
      {
        sources[GetShaderTypeFromString(type)] = source.substr(nextLinePos, pos - nextLinePos);
      }
    }
    return sources;
  }

  uint8 OpenGLShaderPreProcessor::GetUniformBlockFromShaderSource(const std::string& source, std::unordered_map<uint8, std::string>& UniformBlocks)
  {
    uint8 UniformBuffers = 0;

    const char* uniformToken = "uniform";
    const uint64 uniformTokenLength = strlen(uniformToken);
    uint64 pos = source.find(uniformToken, 0);
    while (pos != std::string::npos)
    {
      uint64 endOfLine = source.find_first_of("\r\n", pos);
      uint64 begin = source.rfind("\n", pos);
      uint64 uniformTokenPos = pos;


      std::string UnformStr = source.substr(begin, (endOfLine - begin));
      if (!(UnformStr.find("/") <= uniformTokenPos))
      {
        EUniformType UniformType = GetUniformTypeFromString(UnformStr);
        if (UniformType == EUniformType::UniformBlock)
        {
          int32 BindingPoint = GetUniformLayoutSpecificationAsInt(UnformStr, ELayoutSpecification::Binding);

          if (BindingPoint != -1)
          {
            UniformBuffers |= 1 << BindingPoint;
            uint64 nameBegin = UnformStr.find(uniformToken) + uniformTokenLength + 1;
            uint64 semiColon = UnformStr.find_first_of(" \r\n{", nameBegin);
            UnformStr = UnformStr.substr(nameBegin, (semiColon - nameBegin));
            UniformBlocks[1 << BindingPoint] = UnformStr;
          }
        }
      }

      uint64 nextLinePos = source.find_first_not_of("\r\n", endOfLine);
      pos = source.find(uniformToken, nextLinePos);
    }
    

    return UniformBuffers;
  }

  void OpenGLShaderPreProcessor::GetSingleUniforms(const std::string& source, std::unordered_map<std::string, EUniformType>& Uniforms)
  {
    const char* uniformToken = "uniform";
    const uint64 uniformTokenLength = strlen(uniformToken);
    uint64 pos = source.find(uniformToken, 0);
    while (pos != std::string::npos)
    {
      uint64 endOfLine = source.find_first_of("\r\n", pos);
      uint64 begin = source.rfind("\n", pos);
      uint64 uniformTokenPos = pos;

      std::string UnformStr = source.substr(begin, (endOfLine - begin));
      if (!(UnformStr.find("/") <= uniformTokenPos))
      {
        EUniformType UniformType = GetUniformTypeFromString(UnformStr);
        if (UniformType != EUniformType::UniformBlock)
        {
          uint32 typeAsInt = (int)UniformType;
          uint64 typePos = UnformStr.find(UniformTypesStr[typeAsInt]) + strlen(UniformTypesStr[typeAsInt]) + 1;
          uint64 semiColon = UnformStr.find(";");
          UnformStr = UnformStr.substr(typePos, (semiColon - typePos));

          Uniforms[UnformStr] = UniformType;
        }
      }

      uint64 nextLinePos = source.find_first_not_of("\r\n", endOfLine);
      pos = source.find(uniformToken, nextLinePos);
    }
  }

  EUniformType OpenGLShaderPreProcessor::GetUniformTypeFromString(const std::string& uniform)
  {
    // TODO (och): check the name if the uniform blocks contains a name of a type
    const uint32 NumUniformTypesStr = 11;

    for (int i = 0; i < NumUniformTypesStr; ++i)
    {
      if (uniform.find(UniformTypesStr[i]) != std::string::npos)
      {
        return EUniformType(i);
      }
    }

    return EUniformType::UniformBlock;
  }

  int32 OpenGLShaderPreProcessor::GetUniformLayoutSpecificationAsInt(const std::string& UniformSrc, ELayoutSpecification specification)
  {
    std::string layoutSpecifications = UniformSrc;
    layoutSpecifications.erase(std::remove_if(layoutSpecifications.begin(), layoutSpecifications.end(), isspace), layoutSpecifications.end());

    const char* layoutToken = "layout";
    const uint64 layoutTokenLength = strlen(layoutToken);
    uint64 layoutBegin = layoutSpecifications.find("layout", 0);
    uint64 layoutEnd = layoutSpecifications.find(")", layoutBegin);

    uint64 specificationsBegin = layoutBegin + layoutTokenLength + 1;
    layoutSpecifications = layoutSpecifications.substr(specificationsBegin, layoutEnd - specificationsBegin);

    switch (specification)
    {
    case And::ELayoutSpecification::Binding:
      {
        const char* bindingToken = "binding";
        const uint64 bindingTokenLength = strlen(bindingToken);
        uint64 bindingBegin = layoutSpecifications.find("binding");
        uint64 bindingEnd = layoutSpecifications.find_first_of(",\r\n", bindingBegin);
        bindingBegin += bindingTokenLength + 1;
        layoutSpecifications = layoutSpecifications.substr(bindingBegin, bindingEnd - bindingBegin);
        return std::stoi(layoutSpecifications);
      }
      break;
    case And::ELayoutSpecification::Location:
      {
        const char* locationToken = "location";
        const uint64 locationTokenLength = strlen(locationToken);
        uint64 locationBegin = layoutSpecifications.find("location");
        uint64 locationEnd = layoutSpecifications.find_first_of(",\r\n", locationBegin);
        locationBegin += locationTokenLength + 1;
        layoutSpecifications = layoutSpecifications.substr(locationBegin, locationEnd - locationBegin);
        return std::stoi(layoutSpecifications);
      }
      break;
    }
    return -1;
  }

  uint32 OpenGLShaderCompiler::CompileShaders(const std::unordered_map<uint32, std::string>& Sources)
  {
    uint32 programId = glCreateProgram();

    std::vector<uint32> shaderIds;
    for (auto& [type, src] : Sources)
    {
      uint32 shaderId = CompileShader(type, src);
      if (shaderId != 0)
      {
        shaderIds.push_back(shaderId);
      }
    }
    
    for (uint32 shaderId : shaderIds)
    {
      glAttachShader(programId, shaderId);
    }

    glLinkProgram(programId);
    glValidateProgram(programId);

    int32 value;
    glGetProgramiv(programId, GL_VALIDATE_STATUS, &value);
    if (value == GL_FALSE)
    {
      int ErrorLength;
      std::string Error;
      glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &ErrorLength);
      Error.reserve(ErrorLength);
      glGetProgramInfoLog(programId, ErrorLength, &ErrorLength, &Error[0]);
      printf("---------------------------------------------------------------------------------\n");
      printf("\x1b[41mProgram Error\x1b[0m\n");
      printf("%s\n", Error.c_str());
      printf("---------------------------------------------------------------------------------\n");
      glDeleteProgram(programId);
      programId = 0;
    }

    for (uint32 shaderId : shaderIds)
    {
      glDeleteShader(shaderId);
    }

    return programId;
  }

  uint32 OpenGLShaderCompiler::CompileShader(uint32 type, const std::string& Source)
  {
    uint32 shaderID = glCreateShader(type);
    const char* shaderSource = Source.c_str();

    glShaderSource(shaderID, 1, &shaderSource, nullptr);

    glCompileShader(shaderID);
    if (CheckCompileStatus(shaderID))
    {
      return shaderID;
    }
    
    glDeleteShader(shaderID);
    return 0;
  }

  bool OpenGLShaderCompiler::CheckCompileStatus(uint32 ID)
  {
    int32 value;
    glGetShaderiv(ID, GL_COMPILE_STATUS, &value);

    if (value == GL_FALSE)
    {
      std::string Error;
      int ErrorLeght;
      glGetShaderiv(ID, GL_INFO_LOG_LENGTH, &ErrorLeght);

      Error.reserve(ErrorLeght);

      glGetShaderInfoLog(ID, ErrorLeght, &ErrorLeght, &Error[0]);
      printf("---------------------------------------------------------------------------------\n");
      printf("\x1b[41mShader Compile Error\x1b[0m\n");
      printf("%s\n", Error.c_str());
      printf("---------------------------------------------------------------------------------\n");
      return false;
    }

    return true;
  }

}