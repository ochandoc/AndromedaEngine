#include "Andromeda/Graphics/IndexBuffer.h"
#include "Andromeda/HAL/Window.h"

#include "Backends/DirectX11/DirectX11IndexBuffer.h"

namespace And
{
  std::shared_ptr<IndexBuffer> IndexBuffer::CreateShared(uint64 NumIndices, const std::vector<uint32>& Indices)
  {
    switch (GetGraphicApiType())
    {
    case EGraphicsApiType::OpenGL:
      return nullptr;
      break;
    case EGraphicsApiType::DirectX11:
      return DirectX11IndexBuffer::CreateShared(NumIndices, Indices);
      break;
    }
  }
}