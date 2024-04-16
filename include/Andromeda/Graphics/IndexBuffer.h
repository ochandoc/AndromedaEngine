#pragma once

#include "Andromeda/HAL/Types.h"

namespace And
{
  class IndexBuffer
  {
  public:
    static std::shared_ptr<IndexBuffer> CreateShared(const std::vector<uint32>& Indices);
  };

  
}