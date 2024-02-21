#include "Backends/OpenGL/OpenGLShader.h"
#include "Backends/OpenGL/OpenGL.h"

#include <algorithm>

static const char* UniformTypesStr[] = { "float", "int", "vec2", "vec3", "vec4", "mat2", "mat3", "mat4", "sampler1D", "sampler2D", "sampler3D" };

namespace And
{
  uint32 GetShaderTypeFromString(const std::string& typeStr)
  {
    if (typeStr.compare("Vertex"))
    {
      return GL_VERTEX_SHADER;
    }

    if (typeStr.compare("Fragment"))
    {
      return GL_FRAGMENT_SHADER;
    }

    return 0;
  }

  OpenGLShader::OpenGLShader() : m_Id(0)
  {
  }

  OpenGLShader::OpenGLShader(ENoInit) : OpenGLShader()
  {
  }

  OpenGLShader::~OpenGLShader()
  {
  }

  std::shared_ptr<OpenGLShader> OpenGLShader::Make(const std::string& Path)
  {
    std::shared_ptr<OpenGLShader> shader(new OpenGLShader);

    std::string source = ReadFile(Path);
    ShaderPreProcessInfo PreProcessInfo = OpenGLShaderPreProcessor::PreProcess(source);
    
    for (auto& [type, src] : PreProcessInfo.ShaderSources)
    {
      
    }

    return shader;
  }

  void OpenGLShader::Use() const
  {
  }

  void OpenGLShader::StopUsing() const
  {
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
      UniformBuffers |= GetUniformBlockFromShaderSource(src);
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

  uint8 OpenGLShaderPreProcessor::GetUniformBlockFromShaderSource(const std::string& source)
  {
    uint8 UniformBuffers = 0;

    const char* uniformToken = "uniform";
    const uint64 uniformTokenLength = strlen(uniformToken);
    uint64 pos = source.find(uniformToken, 0);
    while (pos != std::string::npos)
    {
      uint64 endOfLine = source.find_first_of("\r\n", pos);
      uint64 begin = source.rfind("\r\n", pos);
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
      uint64 begin = source.rfind("\r\n", pos);
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
      shaderIds.push_back(shaderId);
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
      // TODO: show error
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

    return 0;
  }

  bool OpenGLShaderCompiler::CheckCompileStatus(uint32 ID)
  {
    int32 value;
    glGetShaderiv(ID, GL_COMPILE_STATUS, &value);

    if (value == GL_FALSE)
    {
      // TODO: show the compile error
      return false;
    }

    return true;
  }

}