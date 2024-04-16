#pragma once

#include "Andromeda/Misc/CoreMiscDefines.h"
#include "Backends/DirectX11/DirectX11Shader.h"

namespace And
{
  class DirectX11ShaderLibrary
  {
    NON_COPYABLE_CLASS(DirectX11ShaderLibrary)
    NON_MOVABLE_CLASS(DirectX11ShaderLibrary)
  public:
    DirectX11ShaderLibrary();
    ~DirectX11ShaderLibrary() = default;

    inline std::shared_ptr<DirectX11Shader> GetForwardShader(const std::string& name) { return m_ForwardShaders.at(name); }
    inline std::shared_ptr<DirectX11Shader> GetDeferredShader(const std::string& name) { return m_DefferedShaders.at(name); }

  private:
    std::unordered_map<std::string, std::shared_ptr<DirectX11Shader>> m_ForwardShaders;
    std::unordered_map<std::string, std::shared_ptr<DirectX11Shader>> m_DefferedShaders;
  };
}