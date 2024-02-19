#include "Andromeda/Graphics/Texture.h"

#include "Backends/OpenGL/OpenGLTexture2D.h"

namespace And
{
  std::shared_ptr<Texture> MakeTexture(const TextureCreationInfo& CreationInfo)
  {
    return OpenGLTexture2D::Make(CreationInfo);
  }

  std::shared_ptr<Texture> MakeTexture(const std::string& Path)
  {
    return OpenGLTexture2D::Make(Path);
  }
}

