#include "Andromeda/Graphics/RenderTarget.h"

#include "Backends/OpenGL/OpenGLRenderTarget.h"

namespace And
{
  std::shared_ptr<RenderTarget> MakeRenderTarget(const RenderTargetCreationInfo& CreationInfo)
  {
    return OpenGLRenderTarget::Make(CreationInfo);
  }
}