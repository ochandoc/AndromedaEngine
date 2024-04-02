#include "Andromeda/Graphics/Lights/PointLight.h"
#include "Andromeda/Graphics/Shader.h"

namespace And{



PointLight::PointLight() : m_raw(){
    m_must_recalculate = true;
}

PointLight::~PointLight(){}

void* PointLight::GetData(){

    return (void*)&m_raw;
}

PointLight::PointLight(const PointLight& other){
    
    this->m_raw = other.m_raw;
    this->m_cast_shadows = other.m_cast_shadows;
    this->m_must_recalculate = other.m_must_recalculate;
}

PointLight::PointLight(PointLight&& other){
    this->m_raw = other.m_raw;
    this->m_cast_shadows = other.m_cast_shadows;
    this->m_must_recalculate = other.m_must_recalculate;
}

PointLight& PointLight::operator=(const PointLight& other){
    this->m_raw = other.m_raw;
    this->m_cast_shadows = other.m_cast_shadows;
    this->m_must_recalculate = other.m_must_recalculate;
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

void PointLight::SetDiffuseColor(float r, float g, float b){
    m_raw.diffuse_color[0] = r;
    m_raw.diffuse_color[1] = g;
    m_raw.diffuse_color[2] = b;
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

void PointLight::GetDiffuseColor(float &r, float& g, float& b){
    r = m_raw.diffuse_color[0];
    g = m_raw.diffuse_color[1];
    b = m_raw.diffuse_color[2];
}

void PointLight::GetSpecularColor(float &r, float& g, float& b){
    r = m_raw.specular_color[0];
    g = m_raw.specular_color[1];
    b = m_raw.specular_color[2];
}

void PointLight::GetPosition(float& x, float& y, float& z){
    x = m_raw.position[0];
    y = m_raw.position[1];
    z = m_raw.position[2];
}

void PointLight::GetEnabled(float& e){

    e = m_raw.enabled;
}

void PointLight::GetSpecularStrength(float& e){
    e = m_raw.specular_strength;
}

void PointLight::GetSpecularShininess(float& e){
    e = m_raw.specular_shininess;
}

void PointLight::GetConstantAtt(float& e){
    e = m_raw.constant_att;
}

void PointLight::GetLinearAtt(float& e){
    e = m_raw.linear_att;
}

void PointLight::GetQuadraticAtt(float& e){
    e = m_raw.quadratic_att;
}

void PointLight::SetSpecularColor(float r, float g, float b){
    m_raw.specular_color[0] = r;
    m_raw.specular_color[1] = g;
    m_raw.specular_color[2] = b;
}

void PointLight::SetPosition(float x, float y, float z){
    m_raw.position[0] = x;
    m_raw.position[1] = y;
    m_raw.position[2] = z;
}   

void PointLight::Recalculate(float aspect_ratio){

}

float* PointLight::GetProjectViewMatrix(float aspect_ratio){

    return nullptr;
}

float* PointLight::GetProjectMatrix(float aspect_ratio){

    return nullptr;
}

float* PointLight::GetViewMatrix(float aspect_ratio){
    return nullptr;
}





}