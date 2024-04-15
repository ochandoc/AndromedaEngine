#include "Andromeda/Graphics/Texture.h"

#include "Andromeda/HAL/Window.h"

#include "Backends/OpenGL/OpenGLTexture2D.h"
#include "Backends/DirectX11/DirectX11SkyboxTexture.h"
#include "Backends/DirectX11/DirectX11Texture2D.h"

namespace And
{
  std::shared_ptr<Texture> MakeTexture(const TextureCreationInfo& CreationInfo)
  {
    switch (GetGraphicApiType())
    {
    case EGraphicsApiType::OpenGL:
      return OpenGLTexture2D::Make(CreationInfo);
      break;
    case EGraphicsApiType::DirectX11:
      return std::shared_ptr<Texture>();
      break;
    }
    return std::shared_ptr<Texture>();
  }

  std::shared_ptr<Texture> MakeTexture(const std::string& Path)
  {
    switch (GetGraphicApiType())
    {
    case EGraphicsApiType::OpenGL:
        return OpenGLTexture2D::Make(Path);
      break;
    case EGraphicsApiType::DirectX11:
      return DirectX11Texture2D::CreateShared(Path);
      break;
    }
    return std::shared_ptr<Texture>();
  }

  std::shared_ptr<SkyboxTexture> MakeSkyboxTexture(const std::vector<std::string>& paths)
  {
    switch (GetGraphicApiType())
    {
    case EGraphicsApiType::OpenGL:
      return std::shared_ptr<SkyboxTexture>();
      break;
    case EGraphicsApiType::DirectX11:
      return DirectX11SkyboxTexture::CreateShared(paths);
      break;
    }

    return std::shared_ptr<SkyboxTexture>();
  }
}

