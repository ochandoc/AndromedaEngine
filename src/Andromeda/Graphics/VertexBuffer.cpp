#include "Andromeda\Graphics\VertexBuffer.h"
#include "Andromeda/HAL/Window.h"

#include "Backends/DirectX11/DriectX11VertexBuffer.h"

namespace And
{
  std::shared_ptr<VertexBuffer> VertexBuffer::CreateShare(uint64 NumVertices, const std::shared_ptr<Vertex> Vertices)
  {
    switch (GetGraphicApiType())
    {
    case EGraphicsApiType::OpenGL:
      return nullptr;
      break;

    case EGraphicsApiType::DirectX11:
      return DirectX11VertexBuffer::CreateShare(NumVertices, Vertices);
      break;
    }
      return std::shared_ptr<VertexBuffer>();
  }
}