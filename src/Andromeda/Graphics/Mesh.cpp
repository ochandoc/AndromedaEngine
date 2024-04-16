#include "Andromeda/Graphics/Mesh.h"

namespace And
{
  Mesh::Mesh(const RawMesh& RawMesh)
  {
    m_IndexBuffer = IndexBuffer::CreateShared(RawMesh.GetIndices());
    m_VertexBuffer = VertexBuffer::CreateShare(RawMesh.GetVertices());
  }
}