#include "DirectX11ConstantBuffer.h"

#include "Backends/DirectX11/DirectX11Renderer.h"

namespace And
{
  DirectX11ConstantBuffer::DirectX11ConstantBuffer() {}

  std::shared_ptr<DirectX11ConstantBuffer> DirectX11ConstantBuffer::CreateShared(uint64 size)
  {
    ID3D11Device* device = DirectX11Renderer::GetDevice();

    if (!device) return nullptr;
    if (size == 0) return nullptr;

    D3D11_BUFFER_DESC BuffDesc = {
      .ByteWidth = (uint32)size,
      .Usage = D3D11_USAGE_DYNAMIC,
      .BindFlags = D3D11_BIND_CONSTANT_BUFFER,
      .CPUAccessFlags = D3D11_CPU_ACCESS_WRITE,
      .MiscFlags = 0,
      .StructureByteStride = 0
    };

    ComPtr<ID3D11Buffer> buffer;

    HRESULT result = device->CreateBuffer(&BuffDesc, NULL, buffer.GetAddressOf());

    if (FAILED(result)) return nullptr;

    std::shared_ptr<DirectX11ConstantBuffer> ConstantBuffer(new DirectX11ConstantBuffer);
    ConstantBuffer->m_Handle = buffer;

    return ConstantBuffer;
  }
}

