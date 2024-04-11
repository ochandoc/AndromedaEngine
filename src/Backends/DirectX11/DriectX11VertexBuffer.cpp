#include "Backends/DirectX11/DriectX11VertexBuffer.h"

#include "Backends/DirectX11/DirectX11Renderer.h"

namespace And
{
  std::shared_ptr<DirectX11VertexBuffer> DirectX11VertexBuffer::CreateShare(const std::vector<Vertex>& Vertices)
  {
    ID3D11Device* device = DirectX11Renderer::GetDevice();

    if (!device) return nullptr;

    HRESULT result = S_OK;

    D3D11_BUFFER_DESC BufferDes = {
      .ByteWidth = (uint32)(Vertices.size() * sizeof(Vertex)),
      .Usage = D3D11_USAGE_DEFAULT,
      .BindFlags = D3D11_BIND_VERTEX_BUFFER,
      .CPUAccessFlags = 0,
      .MiscFlags = 0,
      .StructureByteStride = 0
    };

    D3D11_SUBRESOURCE_DATA Data = {
      .pSysMem = (void*)Vertices.data(),
      .SysMemPitch = 0,
      .SysMemSlicePitch = 0
    };

    ComPtr<ID3D11Buffer> buff;

    result = device->CreateBuffer(&BufferDes, &Data, buff.GetAddressOf());

    if (FAILED(result)) return nullptr;

    std::shared_ptr<DirectX11VertexBuffer> buffer(new DirectX11VertexBuffer);
    buffer->m_Handle = buff;

    return buffer;
  }
}