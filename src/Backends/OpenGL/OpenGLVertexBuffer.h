#pragma once

#include "Andromeda/Graphics/VertexBuffer.h"
#include "Backends/OpenGL/OpenGL.h"
#include "Andromeda/Graphics/Vertex.h"



namespace And
{
  class OpenGLVertexBuffer : public VertexBuffer
  {
  public:
    static std::shared_ptr<OpenGLVertexBuffer> CreateShare(const std::vector<Vertex>& Vertices, const std::vector<unsigned int>& Indices);

    const std::vector<Vertex>& GetBuffer() { return m_Vertex; }
    inline uint64 GetNumVertices() const { return m_NumVertices; }
    inline uint32 GetNumIndices() const { return m_NumIndices; }

    inline void BindVAO() { glBindVertexArray(m_vao); }
    inline void BindVBO() { glBindBuffer(GL_ARRAY_BUFFER, m_vbo); }
    inline void BindEBO() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo); }

  private:
    //ComPtr<ID3D11Buffer> m_Handle;
    std::vector<Vertex> m_Vertex;
    std::vector<uint32> m_Indices;

    uint64 m_NumVertices;
    uint32 m_NumIndices;

    unsigned int m_vbo;
    unsigned int m_vao;
    unsigned int m_ebo;
  };
}