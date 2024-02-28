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


//std::shared_ptr<Shader> SpotLight::GetShader(){}

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
    //m_enabled = e;
    m_raw.enabled = e;
}

void SpotLight::SetDiffuseColor(float color[3]){
    for (int i = 0; i < 3; i++) {
        //m_diffuse_color[i] = color[i];
        m_raw.diffuse_color[i] = color[i];
    }
}


void  SpotLight::SetSpecularStrength(float strength){
    //m_specular_strength = strength;
    m_raw.specular_strength = strength;
}

void  SpotLight::SetSpecularColor(float color[3]){
    for (int i = 0; i < 3; i++) {
        //m_specular_color[i] = color[i];
        m_raw.specular_color[i] = color[i];
    }

}

void SpotLight::SetSpecularShininess(float shininess){
    //m_specular_shininess = shininess;
    m_raw.specular_shininess = shininess;
}

void SpotLight::SetPosition(float position[3]){
    for (int i = 0; i < 3; i++) {
        //m_position[i] = position[i];
        m_raw.position[i] = position[i];
    }
}

void SpotLight::SetDirection(float direction[3]){
    for (int i = 0; i < 3; i++) {
        //m_direction[i] = direction[i];
        m_raw.direction[i] = direction[i];
    }
}

void SpotLight::SetCuttOff(float cutt){
    //m_cutt_off = cutt;
    m_raw.cutt_off = cutt;
}

void SpotLight::SetOuterCuttOff(float outer){
    //m_outer_cut_off = outer;
    m_raw.outer_cut_off = outer;
}

void SpotLight::SetConstantAtt(float constant){
    //m_constant_att = constant;
    m_raw.constant_att = constant;
}

void SpotLight::SetLinearAtt(float linear){
    //m_linear_att = linear;
    m_raw.linear_att = linear;
}

void  SpotLight::SetQuadraticAtt(float quadratic){
    //m_quadratic_att = quadratic;
    m_raw.quadratic_att = quadratic;
}


}