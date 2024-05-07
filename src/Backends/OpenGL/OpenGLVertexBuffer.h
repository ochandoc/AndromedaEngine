#pragma once

#include "Andromeda/Graphics/IndexBuffer.h"
#include "Backends/OpenGL/OpenGL.h"



namespace And
{
  class OpenGLIndexBuffer : public IndexBuffer
  {
  public:
    static std::shared_ptr<OpenGLIndexBuffer> CreateShared(const std::vector<uint32>& Indices);

    //inline ID3D11Buffer* GetBuffer() const { return m_Handle.Get(); }
    inline uint64 GetNumIndices() const { return m_NumIndices; }

  private:
    //ComPtr<ID3D11Buffer> m_Handle;
    uint64 m_NumIndices;
  };
}