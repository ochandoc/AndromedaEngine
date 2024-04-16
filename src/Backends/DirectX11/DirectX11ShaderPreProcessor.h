#pragma once

#include "Andromeda/HAL/Types.h"

namespace And
{
  enum class EDirectX11ShaderType : uint8
  {
    None  = 0,
    Vertex  = 1,
    Pixel   = 2,
  };

  namespace DirectX11ShaderPreProcessor
  {
    std::string LoadSourceFromFile(const std::string& Path);

    std::unordered_map<EDirectX11ShaderType, std::string> SplitSource(const std::string& Source);
    EDirectX11ShaderType GetShaderTypeFromString(const std::string& typeStr);
  }
}