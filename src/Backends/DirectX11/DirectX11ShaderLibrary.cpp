#include "DirectX11ShaderLibrary.h"

And::DirectX11ShaderLibrary::DirectX11ShaderLibrary()
{
  m_ForwardShaders["Skybox"] = DirectX11Shader::CreateShared("shaders/skybox.hlsl");
  m_ForwardShaders["Color"] = DirectX11Shader::CreateShared("shaders/color.hlsl");
  m_ForwardShaders["Light.Directional"] = DirectX11Shader::CreateShared("shaders/directional.hlsl");
  //m_ForwardShaders["Colored.Spot"];
  //m_ForwardShaders["Colored.Point"];
}
