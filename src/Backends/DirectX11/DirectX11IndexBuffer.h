#pragma once

#include "Andromeda/Graphics/IndexBuffer.h"

namespace And
{
  class DirectX11IndexBuffer : public IndexBuffer
  {
  public:
    static std::shared_ptr<DirectX11IndexBuffer> CreateShared(uint64 NumIndices, const std::vector<uint32>& Indices);
  };
}