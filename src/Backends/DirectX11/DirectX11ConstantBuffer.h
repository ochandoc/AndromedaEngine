#pragma once

#include "Andromeda/HAL/Types.h"
#include "Backends/DirectX11/DirectX11.h"

namespace And
{
  class DirectX11ConstantBuffer
  {
  private:
    DirectX11ConstantBuffer();

  public:
    static std::shared_ptr<DirectX11ConstantBuffer> CreateShared(uint64 size);

    inline ID3D11Buffer* GetBuffer() const { return m_Handle.Get(); }

  private:
    ComPtr<ID3D11Buffer> m_Handle;
  };
}