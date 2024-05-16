#include "Andromeda\Graphics\VertexBuffer.h"
#include "Andromeda/HAL/Window.h"

#include "Backends/DirectX11/DirectX11VertexBuffer.h"
#include "Backends/OpenGL/OpenGLVertexBuffer.h"

namespace And
{
  std::shared_ptr<VertexBuffer> VertexBuffer::CreateShare(const std::vector<Vertex> Vertices, const std::vector<unsigned int> Indices)
  {
    switch (GetGraphicApiType())
    {
    case EGraphicsApiType::OpenGL:
      return OpenGLVertexBuffer::CreateShare(Vertices, Indices);
      break;

    case EGraphicsApiType::DirectX11:
      return DirectX11VertexBuffer::CreateShare(Vertices);
      break;
    }
      return std::shared_ptr<VertexBuffer>();
  }
}