#include "Andromeda/Graphics/Mateial.h"

And::Material::Material()
{
  m_Color[0] = 1.0f;
  m_Color[1] = 1.0f;
  m_Color[2] = 1.0f;
  m_Color[3] = 1.0f;
}

And::Material::Material(const Material& other)
{
  m_Color[0] = other.m_Color[0];
  m_Color[1] = other.m_Color[1];
  m_Color[2] = other.m_Color[2];
  m_Color[3] = other.m_Color[3];
  m_ColorTexture = other.m_ColorTexture;
}

And::Material::Material(Material&& other)
{
  m_Color[0] = other.m_Color[0];
  m_Color[1] = other.m_Color[1];
  m_Color[2] = other.m_Color[2];
  m_Color[3] = other.m_Color[3];
  m_ColorTexture.swap(other.m_ColorTexture);
}

And::Material::~Material() {}

And::Material& And::Material::operator=(const Material& other)
{
  if (this != &other)
  {
    m_Color[0] = other.m_Color[0];
    m_Color[1] = other.m_Color[1];
    m_Color[2] = other.m_Color[2];
    m_Color[3] = other.m_Color[3];
    m_ColorTexture = other.m_ColorTexture;
  }
  return *this;
}

And::Material& And::Material::operator=(Material&& other)
{
  if (this != &other)
  {
    m_Color[0] = other.m_Color[0];
    m_Color[1] = other.m_Color[1];
    m_Color[2] = other.m_Color[2];
    m_Color[3] = other.m_Color[3];
    m_ColorTexture.swap(other.m_ColorTexture);
  }
  return *this;
}

void And::Material::SetColor(float r, float g, float b, float a)
{
  m_Color[0] = r;
  m_Color[1] = g;
  m_Color[2] = b;
  m_Color[3] = a;
}

void And::Material::SetColor(float* rgba)
{
  m_Color[0] = rgba[0];
  m_Color[1] = rgba[1];
  m_Color[2] = rgba[2];
  m_Color[3] = rgba[3];
}

void And::Material::SetColorTexture(std::shared_ptr<Texture> tex)
{
  m_ColorTexture = tex;
}
