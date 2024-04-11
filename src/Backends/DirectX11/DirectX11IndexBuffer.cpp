#include "Backends/DirectX11/DirectX11IndexBuffer.h"

#include "Backends/DirectX11/DirectX11Renderer.h"

namespace And
{
  std::shared_ptr<DirectX11IndexBuffer> DirectX11IndexBuffer::CreateShared(const std::vector<uint32>& Indices)
  {
    ID3D11Device* device = DirectX11Renderer::GetDevice();

    if (!device) return nullptr;

    HRESULT result = S_OK;

    D3D11_BUFFER_DESC BufferDes = {
      .ByteWidth = (uint32)(Indices.size() * sizeof(uint32)),
      .Usage = D3D11_USAGE_DEFAULT,
      .BindFlags = D3D11_BIND_INDEX_BUFFER,
      .CPUAccessFlags = 0,
      .MiscFlags = 0,
      .StructureByteStride = 0
    };

    D3D11_SUBRESOURCE_DATA Data = {
      .pSysMem = (void*)Indices.data(),
      .SysMemPitch = 0,
      .SysMemSlicePitch = 0
    };

    ComPtr<ID3D11Buffer> buff;

    result = device->CreateBuffer(&BufferDes, &Data, buff.GetAddressOf());

    if (FAILED(result)) return nullptr;

    std::shared_ptr<DirectX11IndexBuffer> buffer(new DirectX11IndexBuffer);
    buffer->m_Handle = buff;
    buffer->m_NumIndices = Indices.size();

    return buffer;
  }
}