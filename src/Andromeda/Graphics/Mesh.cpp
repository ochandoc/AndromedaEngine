#include "Andromeda/Graphics/Mesh.h"

namespace And
{
  Mesh::Mesh(const RawMesh& RawMesh)
  {
    m_VertexBuffer = VertexBuffer::CreateShare(RawMesh.GetVertices(), RawMesh.GetIndices());
    m_IndexBuffer = IndexBuffer::CreateShared(RawMesh.GetIndices());
  }
}