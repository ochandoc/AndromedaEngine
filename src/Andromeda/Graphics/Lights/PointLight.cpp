#include "Andromeda/Graphics/Lights/PointLight.h"
#include "Andromeda/Graphics/Shader.h"

namespace And{



PointLight::PointLight() : m_raw(){

}

PointLight::~PointLight(){}

void* PointLight::GetData(){

    return (void*)&m_raw;
}

PointLight::PointLight(const PointLight& other){
    
    this->m_raw = other.m_raw;
    this->m_cast_shadows = other.m_cast_shadows;
}

PointLight::PointLight(PointLight&& other){
    this->m_raw = other.m_raw;
    this->m_cast_shadows = other.m_cast_shadows;
}

PointLight& PointLight::operator=(const PointLight& other){
    this->m_raw = other.m_raw;
    this->m_cast_shadows = other.m_cast_shadows;
    return *this;
}

float PointLight::GetEnabled(){
    return m_raw.enabled;;
}

float* PointLight::GetDiffuseColor() {
    return &m_raw.diffuse_color[0];
}

float PointLight::GetSpecularStrength(){
    return m_raw.specular_strength;
}

float* PointLight::GetSpecularColor(){
    return &m_raw.specular_color[0];
}

float PointLight::GetSpecularShininess(){
    return m_raw.specular_shininess;
}

float* PointLight::GetPosition(){
    return &m_raw.position[0];
}

float PointLight::GetConstantAtt(){
    return m_raw.constant_att;
}

float PointLight::GetLinearAtt(){
    return m_raw.linear_att;
}

float PointLight::GetQuadraticAtt(){
    return m_raw.quadratic_att;
}

void PointLight::SetPosition(float p[3]){
    for(int i = 0; i < 3; i++){
        m_raw.position[i] = p[i];
    }
}

void PointLight::SetEnabled(float e){
    m_raw.enabled = e;
}

void PointLight::SetDiffuseColor(float color[3]){
    for (int i = 0; i < 3; i++) {
        m_raw.diffuse_color[i] = color[i];
    }
}

void  PointLight::SetSpecularStrength(float strength){
    m_raw.specular_strength = strength;
}

void  PointLight::SetSpecularColor(float color[3]){
    for (int i = 0; i < 3; i++) {
        m_raw.specular_color[i] = color[i];
    }
}

void PointLight::SetSpecularShininess(float shininess){
    m_raw.specular_shininess = shininess;
}

void PointLight::SetConstantAtt(float constant){
    m_raw.constant_att = constant;
}

void PointLight::SetLinearAtt(float linear){
    m_raw.linear_att = linear;
}

void PointLight::SetQuadraticAtt(float quadratic){
    m_raw.quadratic_att = quadratic;
}

}