#pragma once

#include "Andromeda/Graphics/RawMesh.h"
#include "Andromeda/Graphics/VertexBuffer.h"
#include "Andromeda/Graphics/IndexBuffer.h"

namespace And
{
  class Mesh
  {
  public:
    explicit Mesh(const RawMesh& RawMesh);

    inline IndexBuffer* GetIndexBuffer() const { return m_IndexBuffer.get(); }
    inline VertexBuffer* GetVertexBuffer() const { return m_VertexBuffer.get(); }

  private:
    std::shared_ptr<IndexBuffer> m_IndexBuffer;
    std::shared_ptr<VertexBuffer> m_VertexBuffer;
  };
}