#pragma once

#include "Andromeda/Misc/CoreMiscDefines.h"
#include "Andromeda/HAL/Types.h"
#include "Andromeda/Graphics/Texture.h"

namespace And
{
  class Material final
  {
  public:
    Material();
    Material(const Material& other);
    Material(Material&& other);

    ~Material();

    Material& operator=(const Material& other);
    Material& operator=(Material&& other);

    void SetColor(float r, float g, float b, float a);
    void SetColor(float* rgba);
    void SetColorTexture(std::shared_ptr<Texture> tex);

    float* GetColor() { return m_Color; }
    const float* GetColor() const { return m_Color; }
    std::shared_ptr<Texture> GetColorTexture() { return m_ColorTexture; }
    const std::shared_ptr<Texture> GetColortexture() const { return m_ColorTexture; }

  private:
    float m_Color[4];
    std::shared_ptr<Texture> m_ColorTexture;
  };
}