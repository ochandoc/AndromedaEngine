#include "Andromeda/Graphics/Material.h"

namespace And
{
  Material::Material() 
  {
    m_Color[0] = 1.0f;
    m_Color[1] = 1.0f;
    m_Color[2] = 1.0f;
    m_Color[3] = 1.0f;
  }

  Material::Material(const Material& other)
  {
    m_Color[0] = other.m_Color[0];
    m_Color[1] = other.m_Color[1];
    m_Color[2] = other.m_Color[2];
    m_Color[3] = other.m_Color[3];

    m_ColorTexture = other.m_ColorTexture;
    m_NormalTexture = other.m_NormalTexture;
    m_SpecularTexture = other.m_SpecularTexture;
    m_MetallicTexture = other.m_MetallicTexture;
    m_RoughnessTexture = other.m_RoughnessTexture;
    m_AmbienOclusionTexture = other.m_AmbienOclusionTexture;
  }

  Material::Material(Material&& other)
  {
    m_Color[0] = other.m_Color[0];
    m_Color[1] = other.m_Color[1];
    m_Color[2] = other.m_Color[2];
    m_Color[3] = other.m_Color[3];

    m_ColorTexture.swap(other.m_ColorTexture);
    m_NormalTexture.swap(other.m_NormalTexture);
    m_SpecularTexture.swap(other.m_SpecularTexture);
    m_MetallicTexture.swap(other.m_MetallicTexture);
    m_RoughnessTexture.swap(other.m_RoughnessTexture);
    m_AmbienOclusionTexture.swap(other.m_AmbienOclusionTexture);
  }

  Material::~Material()
  {
  }

  Material& Material::operator=(const Material& other)
  {
    if (this != &other)
    {
      m_Color[0] = other.m_Color[0];
      m_Color[1] = other.m_Color[1];
      m_Color[2] = other.m_Color[2];
      m_Color[3] = other.m_Color[3];

      m_ColorTexture = other.m_ColorTexture;
      m_NormalTexture = other.m_NormalTexture;
      m_SpecularTexture = other.m_SpecularTexture;
      m_MetallicTexture = other.m_MetallicTexture;
      m_RoughnessTexture = other.m_RoughnessTexture;
      m_AmbienOclusionTexture = other.m_AmbienOclusionTexture;
    }
    return *this;
  }

  Material& Material::operator=(Material&& other)
  {
    if (this != &other)
    {
      m_Color[0] = other.m_Color[0];
      m_Color[1] = other.m_Color[1];
      m_Color[2] = other.m_Color[2];
      m_Color[3] = other.m_Color[3];

      m_ColorTexture.swap(other.m_ColorTexture);
      m_NormalTexture.swap(other.m_NormalTexture);
      m_SpecularTexture.swap(other.m_SpecularTexture);
      m_MetallicTexture.swap(other.m_MetallicTexture);
      m_RoughnessTexture.swap(other.m_RoughnessTexture);
      m_AmbienOclusionTexture.swap(other.m_AmbienOclusionTexture);
    }
    return *this;
  }

  void Material::SetColor(float r, float g, float b, float a)
  {
    m_Color[0] = r;
    m_Color[1] = g;
    m_Color[2] = b;
    m_Color[3] = a;
  }

  void Material::SetColor(float* rgba)
  {
    m_Color[0] = rgba[0];
    m_Color[1] = rgba[1];
    m_Color[2] = rgba[2];
    m_Color[3] = rgba[3];
  }

  void Material::SetColorTexture(std::shared_ptr<Texture> tex)
  {
    m_ColorTexture = tex;
  }

  void Material::SetNormalTexture(std::shared_ptr<Texture> tex)
  {
    m_NormalTexture = tex;
  }
  
  void Material::SetSpecularTexture(std::shared_ptr<Texture> tex){
    m_SpecularTexture = tex;
  }

  void Material::SetMetallicTexture(std::shared_ptr<Texture> tex){
      
      m_MetallicTexture = tex;
  }

  void Material::SetRoughnessTexture(std::shared_ptr<Texture> tex){

      m_RoughnessTexture = tex;
  }

  void Material::SetAmbientOclusionTexture(std::shared_ptr<Texture> tex){

      m_AmbienOclusionTexture = tex;
  }

  bool Material::GetIsPBR(){
      return m_NormalTexture && m_RoughnessTexture && m_AmbienOclusionTexture && m_MetallicTexture;
  }


}