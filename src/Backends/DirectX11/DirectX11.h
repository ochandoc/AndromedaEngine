#pragma once

#include <dxgi.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#include <wrl/client.h>

template <typename T>
using ComPtr = Microsoft::WRL::ComPtr<T>;

namespace DirectX11Tools
{
  std::string BlobToString(ID3DBlob* blob);
  std::string BlobToString(ComPtr<ID3DBlob> blob);
}