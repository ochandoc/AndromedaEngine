#pragma once

#include "Andromeda/Graphics/VertexBuffer.h"
#include "Backends/DirectX11/DirectX11.h"

namespace And
{
  class DirectX11VertexBuffer : public VertexBuffer
  {
  public:
    static std::shared_ptr<DirectX11VertexBuffer> CreateShare(const std::vector<Vertex>& Vertices);

    inline ID3D11Buffer* GetBuffer() const { return m_Handle.Get(); }

  private:
    ComPtr<ID3D11Buffer> m_Handle;
  };
}