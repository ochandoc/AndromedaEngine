#pragma once 

#include "Vertex.h"
#include "Andromeda/HAL/Types.h"

namespace And
{
  class VertexBuffer
  {
  public:
    static std::shared_ptr<VertexBuffer> CreateShare(uint64 NumVertices, const std::shared_ptr<Vertex> Vertices);
  };
}