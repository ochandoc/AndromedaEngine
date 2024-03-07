#include "Andromeda/Graphics/Lights/DirectionalLight.h"
#include "Andromeda/Graphics/Shader.h"

namespace And{

DirectionalLight::DirectionalLight() : m_raw(){

}

DirectionalLight::~DirectionalLight(){}

void* DirectionalLight::GetData(){

    return (void*)&m_raw;
}

DirectionalLight::DirectionalLight(const DirectionalLight& other){
    
    this->m_raw = other.m_raw;
    this->m_cast_shadows = other.m_cast_shadows;
}

DirectionalLight::DirectionalLight(DirectionalLight&& other){
    this->m_raw = other.m_raw;
    this->m_cast_shadows = other.m_cast_shadows;
}

DirectionalLight& DirectionalLight::operator=(const DirectionalLight& other){
    // Guardar tambien cast shadows
    this->m_raw = other.m_raw;
    this->m_cast_shadows = other.m_cast_shadows;
    return *this;
}

float DirectionalLight::GetEnabled(){
    return m_raw.enabled;;
}

float* DirectionalLight::GetDiffuseColor() {
    return &m_raw.diffuse_color[0];
}

float DirectionalLight::GetSpecularStrength(){
    return m_raw.specular_strength;
}

float* DirectionalLight::GetSpecularColor(){
    return &m_raw.specular_color[0];
}

float DirectionalLight::GetSpecularShininess(){
    return m_raw.specular_shininess;
}

float* DirectionalLight::GetDirection(){
    return &m_raw.direction[0];
}

void DirectionalLight::SetEnabled(float e){
    //m_enabled = e;
    m_raw.enabled = e;
}

void DirectionalLight::SetDiffuseColor(float color[3]){
    for (int i = 0; i < 3; i++) {
        //m_diffuse_color[i] = color[i];
        m_raw.diffuse_color[i] = color[i];
    }
}

void DirectionalLight::SetDiffuseColor(float x, float y, float z){

    m_raw.diffuse_color[0] = x;
    m_raw.diffuse_color[1] = y;
    m_raw.diffuse_color[2] = z;
    
}

void  DirectionalLight::SetSpecularStrength(float strength){
    //m_specular_strength = strength;
    m_raw.specular_strength = strength;
}

void  DirectionalLight::SetSpecularColor(float color[3]){
    for (int i = 0; i < 3; i++) {
        //m_specular_color[i] = color[i];
        m_raw.specular_color[i] = color[i];
    }

}

void  DirectionalLight::SetSpecularColor(float x, float y, float z){
   
    m_raw.specular_color[0] = x;
    m_raw.specular_color[1] = y;
    m_raw.specular_color[2] = z;
}

void DirectionalLight::SetSpecularShininess(float shininess){
    //m_specular_shininess = shininess;
    m_raw.specular_shininess = shininess;
}

void DirectionalLight::SetDirection(float direction[3]){
    for (int i = 0; i < 3; i++) {
        //m_direction[i] = direction[i];
        m_raw.direction[i] = direction[i];
    }
}

void DirectionalLight::SetDirection(float x, float y, float z){

    m_raw.direction[0] = x;
    m_raw.direction[1] = y;
    m_raw.direction[2] = z;
    
}

void  DirectionalLight::GetEnabled(float& enabled){
    enabled = m_raw.enabled;
}

void  DirectionalLight::GetDiffuseColor(float& r, float& g, float& b){
    r = m_raw.diffuse_color[0];
    g = m_raw.diffuse_color[1];
    b = m_raw.diffuse_color[2];
}

void  DirectionalLight::GetSpecularStrength(float& strenght){
    strenght = m_raw.specular_strength;
}

void  DirectionalLight::GetSpecularColor(float& r, float& g, float& b){
    r = m_raw.specular_color[0];
    g = m_raw.specular_color[1];
    b = m_raw.specular_color[2];
}

void  DirectionalLight::GetSpecularShininess(float& shininess){
    shininess = m_raw.specular_shininess;
}

void  DirectionalLight::GetDirection(float& x, float& y, float& z){
    x = m_raw.direction[0];
    y = m_raw.direction[1];
    z = m_raw.direction[2];
}


}