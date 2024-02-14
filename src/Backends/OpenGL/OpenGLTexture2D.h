#pragma once

#include "Andromeda/Graphics/Texture.h"

namespace And
{
  class OpenGLTexture2D : public Texture
  {
  private:
    OpenGLTexture2D();

  public:
    OpenGLTexture2D(ENoInit);

    virtual ~OpenGLTexture2D();

    static std::shared_ptr<OpenGLTexture2D> Make(const TextureCreationInfo& CreationInfo);
    static std::shared_ptr<OpenGLTexture2D> Make(const std::string& Path);
    
    void Activate(uint32 Slot) const;
    inline uint32 GetId() const { return m_Id; }

    virtual void UploadData(void* Data, uint64 Size) override;
    virtual bool IsLoadedFromFile() const override;
    virtual const char* GetPath() const override;

  private:
    uint32 m_Id;
    bool m_LoadedFromFile;
    std::string m_Path;
  };
}


