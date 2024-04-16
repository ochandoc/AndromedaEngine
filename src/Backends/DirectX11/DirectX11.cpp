#include "DirectX11.h"

std::string DirectX11Tools::BlobToString(ID3DBlob* blob)
{
  if (!blob) return std::string();

  return std::string((char*)blob->GetBufferPointer(), blob->GetBufferSize());
}

std::string DirectX11Tools::BlobToString(ComPtr<ID3DBlob> blob)
{
  if (!blob.Get()) return std::string();

  return std::string((char*)blob->GetBufferPointer(), blob->GetBufferSize());
}
