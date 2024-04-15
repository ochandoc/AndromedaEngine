#include "DirectX11ShaderLibrary.h"

And::DirectX11ShaderLibrary::DirectX11ShaderLibrary()
{
  m_ForwardShaders["Skybox"] = DirectX11Shader::CreateShared("skybox/cubemap.hlsl");
  m_ForwardShaders["Color"] = DirectX11Shader::CreateShared("shaders/color.hlsl");
}
