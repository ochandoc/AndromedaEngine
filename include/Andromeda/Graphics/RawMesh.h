#pragma once

#include "Andromeda/HAL/Types.h"
#include "Andromeda/Graphics/Vertex.h"

namespace And
{
  class RawMesh
  {
  public:
    RawMesh();
    RawMesh(const RawMesh& other);
    RawMesh(RawMesh&& other);

    ~RawMesh();

    RawMesh& operator =(const RawMesh& other);
    RawMesh& operator =(RawMesh&& other);

    inline uint64 GetNumTriangles() const { return m_Indices.size() / 3; }
    inline uint64 GetNumVertices() const { return m_Vertices.size(); }
    inline uint64 GetNumIndices() const { return m_Indices.size(); }

    inline std::vector<Vertex>& GetVertices() { return m_Vertices; }
    inline const std::vector<Vertex>& GetVertices() const { return m_Vertices; }

    inline std::vector<uint32>& GetIndices() { return m_Indices; }
    inline const std::vector<uint32>& GetIndices() const { return m_Indices; }

    static RawMesh CreateCube();
    static RawMesh CreateSkybox();

  private:
    std::vector<Vertex> m_Vertices;
    std::vector<uint32> m_Indices;
  };
}