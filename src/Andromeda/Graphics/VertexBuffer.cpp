#include "Andromeda\Graphics\VertexBuffer.h"
#include "Andromeda/HAL/Window.h"

#include "Backends/DirectX11/DirectX11VertexBuffer.h"

namespace And
{
  std::shared_ptr<VertexBuffer> VertexBuffer::CreateShare(const std::vector<Vertex> Vertices)
  {
    switch (GetGraphicApiType())
    {
    case EGraphicsApiType::OpenGL:
      return nullptr;
      break;

    case EGraphicsApiType::DirectX11:
      return DirectX11VertexBuffer::CreateShare(Vertices);
      break;
    }
      return std::shared_ptr<VertexBuffer>();
  }
}