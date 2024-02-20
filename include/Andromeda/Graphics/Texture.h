#pragma once

#include "Andromeda/Misc/CoreMiscDefines.h"
#include "Andromeda/HAL/Types.h"

namespace And
{
  enum class ETextureFormat : uint8
  {
    /**  Common color format */
    RGB8,
    RGBA8,


    /**  Common format for position or normals */
    RGB16F,
    RGBA16F,


    /**  Extended format of 32 bits floats */
    RGB32F,
    RGBA32F,


    /** Format for depth texture */
    Depth,
  };

  struct TextureCreationInfo
  {
    uint32 Width;
    uint32 Height;
    ETextureFormat Format;
    bool Mipmaps;
  };

  class Texture
  {
    NON_COPYABLE_CLASS(Texture)
    NON_MOVABLE_CLASS(Texture)
  public:
    Texture() = default;
    virtual ~Texture() = default;

    virtual void UploadData(void* Data, uint64 Size) = 0;
    virtual bool IsLoadedFromFile() const = 0;
    virtual const char* GetPath() const = 0;

    inline uint32 GetWidth() const { return m_CreationInfo.Width; }
    inline uint32 GetHeight() const { return m_CreationInfo.Height; }
    inline ETextureFormat GetFormat() const { return m_CreationInfo.Format; }
    inline const TextureCreationInfo& GetCreationInfo() const { return m_CreationInfo; }
  protected:
    TextureCreationInfo m_CreationInfo;
  };

  std::shared_ptr<Texture> MakeTexture(const TextureCreationInfo& CreationInfo);
  std::shared_ptr<Texture> MakeTexture(const std::string& Path);
}