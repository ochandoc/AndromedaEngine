#include "Andromeda/Graphics/RawMesh.h"

namespace And
{
  RawMesh::RawMesh() {}

  RawMesh::RawMesh(const RawMesh& other) 
  {
    m_Vertices = other.m_Vertices;
    m_Indices = other.m_Indices;
  }

  RawMesh::RawMesh(RawMesh&& other) 
  {
    m_Vertices.swap(other.m_Vertices);
    m_Indices.swap(other.m_Indices);
  }

  RawMesh::~RawMesh() {}

  RawMesh& RawMesh::operator=(const RawMesh& other)
  {
    if (this != &other)
    {
      m_Vertices = other.m_Vertices;
      m_Indices = other.m_Indices;
    }
    return *this;
  }

  RawMesh& RawMesh::operator=(RawMesh&& other)
  {
    if (this != &other)
    {
      m_Vertices.swap(other.m_Vertices);
      m_Indices.swap(other.m_Indices);
    }
    return *this;
  }

  RawMesh RawMesh::CreateCube()
  {
    RawMesh rm;
    rm.m_Vertices = {
      {
        -0.5, 0.5, -0.5,           // v top left front
        0.0f, 0.0f, -1.0f,         // vn
        0.0f, 1.0f,                // vt
      },
      {
        -0.5, 0.5, -0.5,           // v top left front
        -1.0f, 0.0f, 0.0f,         // vn
        1.0f, 1.0f,                // vt
      },
      {
        -0.5, 0.5, -0.5,           // v top left front
        0.0f, 1.0f, 0.0f,             // vn
        1.0f, 1.0f,                   // vt
      },
      {
        0.5, 0.5, -0.5,            // v top right front
        0.0f, 0.0f, -1.0f,         // vn
        1.0f, 1.0f,                // vt
      },
      {
        0.5, 0.5, -0.5,            // v top right front
        1.0f, 0.0f, 0.0f,          // vn
        0.0f, 1.0f,                // vt
      },
      {
        0.5, 0.5, -0.5,            // v top right front
        0.0f, 1.0f, 0.0f,          // vn
        0.0f, 1.0f,                // vt
      },
      {
        -0.5, -0.5, -0.5,          // v down left front
        0.0f, 0.0f, -1.0f,         // vn
        0.0f, 0.0f,                // vt
      },
      {
        -0.5, -0.5, -0.5,          // v down left front
        -1.0f, 0.0f, 0.0f,         // vn
        1.0f, 0.0f,                // vt
      },
      {
        -0.5, -0.5, -0.5,          // v down left front
        0.0f, -1.0f, 0.0f,         // vn
        1.0f, 0.0f,                // vt
      },
      {
        0.5, -0.5, -0.5,           // v down right front
        0.0f, 0.0f, -1.0f,         // vn
        1.0f, 0.0f,                // vt
      },
      {
        0.5, -0.5, -0.5,           // v down right front
        1.0f, 0.0f, 0.0f,          // vn
        0.0f, 0.0f,                // vt
      },
      {
        0.5, -0.5, -0.5,           // v down right front
        0.0f, -1.0f, 0.0f,         // vn
        0.0f, 0.0f,                // vt
      },
      {
        -0.5, 0.5, 0.5,            // v top left back
        -1.0f, 0.0f, 0.0f,         // vn
        0.0f, 1.0f,                // vt
      },
      {
        -0.5, 0.5, 0.5,            // v top left back
        0.0f, 1.0f, 0.0f,          // vn
        1.0f, 0.0f,                // vt
      },
      {
        -0.5, 0.5, 0.5,            // v top left back
        0.0f, 0.0f, 1.0f,          // vn
        1.0f, 1.0f,                // vt
      },
      {
        0.5, 0.5, 0.5,             // v top right back
        1.0f, 0.0f, 0.0f,          // vn
        1.0f, 1.0f,                // vt
      },
      {
        0.5, 0.5, 0.5,             // v top right back
        0.0f, 1.0f, 0.0f,          // vn
        0.0f, 0.0f,                // vt
      },
      {
        0.5, 0.5, 0.5,             // v top right back
        0.0f, 0.0f, 1.0f,          // vn
        0.0f, 1.0f,                // vt
      },
      {
        -0.5, -0.5, 0.5,           // v down left back
        -1.0f, 0.0f, 0.0f,         // vn
        0.0f, 0.0f,                // vt
      },
      {
        -0.5, -0.5, 0.5,           // v down left back
        0.0f, -1.0f, 0.0f,         // vn
        1.0f, 1.0f,                // vt
      },
      {
        -0.5, -0.5, 0.5,           // v down left back
        0.0f, 0.0f, 1.0f,          // vn
        1.0f, 0.0f,                // vt
      },
      {
        0.5, -0.5, 0.5,            // v down right back
        1.0f, 0.0f, 0.0f,          // vn
        1.0f, 0.0f,                // vt
      },
      {
        0.5, -0.5, 0.5,            // v down right back
        0.0f, -1.0f, 0.0f,         // vn
        0.0f, 1.0f,                // vt
      },
      {
        0.5, -0.5, 0.5,            // v down right back
        0.0f, 0.0f, 1.0f,          // vn
        0.0f, 0.0f                 // vt
      },
    };

    rm.m_Indices = {
      0, 3, 6,                    // f front
      6, 3, 9,

      3 + 1, 15, 9 + 1,          // f right
      9 + 1, 15, 21,

      12, 0 + 1, 18,              // f left
      18, 0 + 1, 6 + 1,

      12 + 1, 15 + 1, 0 + 2,      // f top
      0 + 2, 15 + 1, 3 + 2,

      6 + 2, 21 + 1, 18 + 1,      // f down 
      9 + 2, 21 + 1, 6 + 2,

      15 + 2, 12 + 2, 21 + 2,     // f back
      21 + 2, 12 + 2, 18 + 2
    };

    return rm;
  }
}