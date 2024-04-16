#pragma once 

#include "Vertex.h"
#include "Andromeda/HAL/Types.h"

namespace And
{
  class VertexBuffer
  {
  public:
    static std::shared_ptr<VertexBuffer> CreateShare(const std::vector<Vertex> Vertices);
  };
}