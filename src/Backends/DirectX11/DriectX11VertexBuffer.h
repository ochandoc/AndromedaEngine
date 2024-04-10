#pragma once

#include "Andromeda/Graphics/VertexBuffer.h"

namespace And
{
  class DirectX11VertexBuffer : public VertexBuffer
  {
  public:
    static std::shared_ptr<DirectX11VertexBuffer> CreateShare(uint64 NumVertices, const std::shared_ptr<Vertex> Vertices);
  };
}