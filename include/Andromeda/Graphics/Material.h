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
    void SetNormalTexture(std::shared_ptr<Texture> tex);
    void SetSpecularTexture(std::shared_ptr<Texture> tex);
    void SetMetallicTexture(std::shared_ptr<Texture> tex);
    void SetRoughnessTexture(std::shared_ptr<Texture> tex);
    void SetAmbientOclusionTexture(std::shared_ptr<Texture> tex);

    float* GetColor() { return m_Color; }
    const float* GetColor() const { return m_Color; }
    
    std::shared_ptr<Texture> GetColorTexture() { return m_ColorTexture; }
    const std::shared_ptr<Texture> GetColortexture() const { return m_ColorTexture; }
    
    std::shared_ptr<Texture> GetNormalTexture() { return m_NormalTexture; }
    const std::shared_ptr<Texture> GetNormaltexture() const { return m_NormalTexture; }
    
    std::shared_ptr<Texture> GetSpecularTexture() { return m_SpecularTexture; }
    const std::shared_ptr<Texture> GetSpeculartexture() const { return m_SpecularTexture; }
    
    std::shared_ptr<Texture> GetMetallicTexture() { return m_MetallicTexture; }
    const std::shared_ptr<Texture> GetMetallictexture() const { return m_MetallicTexture; }

    std::shared_ptr<Texture> GetRoughnessTexture() { return m_RoughnessTexture; }
    const std::shared_ptr<Texture> GetRoughnesstexture() const { return m_RoughnessTexture; }

    std::shared_ptr<Texture> GetAmbientOclusionTexture() { return m_AmbienOclusionTexture; }
    const std::shared_ptr<Texture> GetAmbienOclusiontexture() const { return m_AmbienOclusionTexture; }

  private:
    float m_Color[4];
    std::shared_ptr<Texture> m_ColorTexture;
    std::shared_ptr<Texture> m_NormalTexture;
    std::shared_ptr<Texture> m_SpecularTexture;
    std::shared_ptr<Texture> m_MetallicTexture;
    std::shared_ptr<Texture> m_RoughnessTexture;
    std::shared_ptr<Texture> m_AmbienOclusionTexture;
  };
}