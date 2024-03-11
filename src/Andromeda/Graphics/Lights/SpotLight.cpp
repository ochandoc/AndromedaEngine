#include "Andromeda/Graphics/Lights/SpotLight.h"
#include "Andromeda/Graphics/Shader.h"

namespace And{



SpotLight::SpotLight() : m_raw(){

}

SpotLight::~SpotLight(){}

void* SpotLight::GetData(){

    return (void*)&m_raw;
}

SpotLight::SpotLight(const SpotLight& other){
    
    this->m_raw = other.m_raw;
    this->m_cast_shadows = other.m_cast_shadows;
}

SpotLight::SpotLight(SpotLight&& other){
    this->m_raw = other.m_raw;
    this->m_cast_shadows = other.m_cast_shadows;
}

SpotLight& SpotLight::operator=(const SpotLight& other){
    this->m_raw = other.m_raw;
    this->m_cast_shadows = other.m_cast_shadows;
    return *this;
}

float SpotLight::GetEnabled(){
    return m_raw.enabled;;
}

float* SpotLight::GetDiffuseColor() {
    return &m_raw.diffuse_color[0];
}

float SpotLight::GetSpecularStrength(){
    return m_raw.specular_strength;
}

float* SpotLight::GetSpecularColor(){
    return &m_raw.specular_color[0];
}

float SpotLight::GetSpecularShininess(){
    return m_raw.specular_shininess;
}

float* SpotLight::GetPosition(){
    return &m_raw.position[0];
}

float* SpotLight::GetDirection(){
    return &m_raw.direction[0];

}

float SpotLight::GetCuttOff(){
    return m_raw.cutt_off;
}

float SpotLight::GetOuterCuttOff(){
    return m_raw.outer_cut_off;
}

float SpotLight::GetConstantAtt(){
    return m_raw.constant_att;
}

float SpotLight::GetLinearAtt(){
    return m_raw.linear_att;
}

float SpotLight::GetQuadraticAtt(){
    return m_raw.quadratic_att;
}


void SpotLight::SetEnabled(float e){
    m_raw.enabled = e;
}

void SpotLight::SetDiffuseColor(float color[3]){
    for (int i = 0; i < 3; i++) {
        m_raw.diffuse_color[i] = color[i];
    }
}


void  SpotLight::SetSpecularStrength(float strength){
    m_raw.specular_strength = strength;
}

void  SpotLight::SetSpecularColor(float color[3]){
    for (int i = 0; i < 3; i++) {
        m_raw.specular_color[i] = color[i];
    }

}

void SpotLight::SetSpecularShininess(float shininess){
    m_raw.specular_shininess = shininess;
}

void SpotLight::SetPosition(float position[3]){
    for (int i = 0; i < 3; i++) {
        m_raw.position[i] = position[i];
    }
}

void SpotLight::SetDirection(float direction[3]){
    for (int i = 0; i < 3; i++) {
        m_raw.direction[i] = direction[i];
    }
}

void SpotLight::SetCuttOff(float cutt){
    m_raw.cutt_off = cutt;
}

void SpotLight::SetOuterCuttOff(float outer){
    m_raw.outer_cut_off = outer;
}

void SpotLight::SetConstantAtt(float constant){
    m_raw.constant_att = constant;
}

void SpotLight::SetLinearAtt(float linear){
    m_raw.linear_att = linear;
}

void  SpotLight::SetQuadraticAtt(float quadratic){
    m_raw.quadratic_att = quadratic;
}

void  SpotLight::GetEnabled(float& enabled){
    m_raw.enabled = enabled;
}

void SpotLight::GetDiffuseColor(float& r, float& g, float& b){
    r = m_raw.diffuse_color[0];
    g = m_raw.diffuse_color[1];
    b = m_raw.diffuse_color[2];
}

void  SpotLight::GetSpecularStrength(float& strenght){
    strenght = m_raw.specular_strength;
}

void SpotLight::GetSpecularColor(float& r, float& g, float& b){
    r = m_raw.specular_color[0];
    g = m_raw.specular_color[1];
    b = m_raw.specular_color[2];
}

void  SpotLight::GetSpecularShininess(float& shininess){
    shininess = m_raw.specular_shininess;
}

void SpotLight::GetPosition(float& x, float& y, float& z){
    x = m_raw.position[0];
    y = m_raw.position[1];
    z = m_raw.position[2];
}

void SpotLight::GetDirection(float& x, float& y, float& z){
    x = m_raw.direction[0];
    y = m_raw.direction[1];
    z = m_raw.direction[2];
}

void  SpotLight::GetCuttOff(float& cutt){
    cutt = m_raw.cutt_off;
}

void  SpotLight::GetOuterCuttOff(float& outer){
    outer = m_raw.outer_cut_off;
}

void  SpotLight::GetConstantAtt(float& constant){
    constant = m_raw.constant_att;
}

void  SpotLight::GetLinearAtt(float& linear){
    linear = m_raw.linear_att;
}

void  SpotLight::GetQuadraticAtt(float& quadratic){
    quadratic = m_raw.quadratic_att;
}

void  SpotLight::SetDiffuseColor(float r, float g, float b){
    m_raw.diffuse_color[0] = r;
    m_raw.diffuse_color[1] = g;
    m_raw.diffuse_color[2] = b;
}

void  SpotLight::SetSpecularColor(float r, float g, float b){
    m_raw.specular_color[0] = r;
    m_raw.specular_color[1] = g;
    m_raw.specular_color[2] = b;
}

void  SpotLight::SetPosition(float x, float y, float z){
    m_raw.position[0] = x;
    m_raw.position[1] = y;
    m_raw.position[2] = z;
}

void  SpotLight::SetDirection(float x, float y, float z){
    m_raw.direction[0] = x;
    m_raw.direction[1] = y;
    m_raw.direction[2] = z;
}



}