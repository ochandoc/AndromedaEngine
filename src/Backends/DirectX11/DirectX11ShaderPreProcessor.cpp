#include "DirectX11ShaderPreProcessor.h"

#include "Backends/DirectX11/DirectX11.h"

namespace And
{
  namespace DirectX11ShaderPreProcessor
  {
    std::string LoadSourceFromFile(const std::string& Path)
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

    std::unordered_map<EDirectX11ShaderType, std::string> SplitSource(const std::string& source)
    {
      std::unordered_map<EDirectX11ShaderType, std::string> sources;

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

    EDirectX11ShaderType GetShaderTypeFromString(const std::string& typeStr)
    {
      if (typeStr.find("Vertex") != std::string::npos)
      {
        return EDirectX11ShaderType::Vertex;
      }

      if (typeStr.find("Pixel") != std::string::npos)
      {
        return EDirectX11ShaderType::Pixel;
      }

      return EDirectX11ShaderType::None;
    }
  }
}

