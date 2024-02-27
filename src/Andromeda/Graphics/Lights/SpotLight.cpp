#include "Andromeda/Graphics/Lights/SpotLight.h"
#include "Andromeda/Graphics/Shader.h"

namespace And{

struct SpotLightRaw {
    float position[3] = { 0.5f, 0.5f, 0.5f };
    float padding3 = -1.0f;
    float direction[3] = { 0.5f, 0.5f, 0.5f };
    float padding = -1.0f;
    float diffuse_color[3] = { 0.5f, 0.5f, 0.5f };
    float padding2 = -1.0f;
    float specular_color[3] = { 0.5f, 0.5f, 0.5f };
    float enabled = 0.5f;
    float cutt_off = 0.5f;
    float outer_cut_off = 0.5f;
    float specular_strength = 0.5f;
    float specular_shininess = 0.5f;
    float constant_att = 0.5f;
    float linear_att = 0.5f;
    float quadratic_att = 0.5f; // 80 bytes
};

std::shared_ptr<SpotLight> SpotLight::MakeSpotLight(){
    std::shared_ptr<SpotLight> spot = std::make_shared<SpotLight>();
    std::shared_ptr<Shader> shader = MakeShader("lights/spot.shader");
    if (shader) {
        spot->m_shader = shader;
        return spot;
    }

    return nullptr;
}

SpotLight::SpotLight() : m_raw(){}

SpotLight::~SpotLight(){

}


void* SpotLight::GetData() {
    /*SpotLightRaw raw;
    raw.enabled = m_enabled;
    raw.cutt_off = m_cutt_off;
    raw.outer_cut_off = m_outer_cut_off;
    raw.specular_strength = m_specular_strength;
    raw.specular_shininess = m_specular_shininess;
    raw.constant_att = m_constant_att;
    raw.linear_att = m_linear_att;
    raw.quadratic_att = m_quadratic_att;

    for (int i = 0; i < 3; i++) {
        raw.position[i] = m_position[i];
        raw.direction[i] = m_direction[i];
        raw.diffuse_color[i] = m_diffuse_color[i];
        raw.specular_color[i] = m_specular_color[i];
    }*/

    return (void*)&m_raw;
}

float SpotLight::GetEnabled(){
    return m_enabled;
}

float* SpotLight::GetDiffuseColor() {
    return &m_diffuse_color[0];
}

float SpotLight::GetSpecularStrength(){
    return m_specular_strength;
}

float* SpotLight::GetSpecularColor(){
    return &m_specular_color[0];
}

float  SpotLight::GetSpecularShininess(){
    return m_specular_shininess;
}

float* SpotLight::GetPosition(){
    return &m_position[0];
}

float* SpotLight::GetDirection(){
    return &m_direction[0];

}

float SpotLight::GetCuttOff(){
    return m_cutt_off;
}

float SpotLight::GetOuterCuttOff(){
    return m_outer_cut_off;
}

float SpotLight::GetConstantAtt(){
    return m_constant_att;
}

float SpotLight::GetLinearAtt(){
    return m_linear_att;
}

float SpotLight::GetQuadraticAtt(){
    return m_quadratic_att;
}


void SpotLight::SetEnabled(float e){
    m_enabled = e;
    m_raw->enabled = e;
}

void SpotLight::SetDiffuseColor(float color[3]){
    for (int i = 0; i < 3; i++) {
        m_diffuse_color[i] = color[i];
        m_raw->diffuse_color[i] = color[i];
    }
}


void  SpotLight::SetSpecularStrength(float strength){
    m_specular_strength = strength;
    m_raw->specular_strength = strength;
}

void  SpotLight::SetSpecularColor(float color[3]){
    for (int i = 0; i < 3; i++) {
        m_specular_color[i] = color[i];
        m_raw->specular_color[i] = color[i];
    }

}

void  SpotLight::SetSpecularShininess(float shininess){
    m_specular_shininess = shininess;
    m_raw->specular_shininess = shininess;
}

void  SpotLight::SetPosition(float position[3]){
    for (int i = 0; i < 3; i++) {
        m_position[i] = position[i];
        m_raw->position[i] = position[i];
    }
}

void  SpotLight::SetDirection(float direction[3]){
    for (int i = 0; i < 3; i++) {
        m_direction[i] = direction[i];
        m_raw->direction[i] = direction[i];
    }
}

void  SpotLight::SetCuttOff(float cutt){
    m_cutt_off = cutt;
    m_raw->cutt_off = cutt;
}

void  SpotLight::SetOuterCuttOff(float outer){
    m_outer_cut_off = outer;
    m_raw->outer_cut_off = outer;
}

void  SpotLight::SetConstantAtt(float constant){
    m_constant_att = constant;
    m_raw->constant_att = constant;
}

void  SpotLight::SetLinearAtt(float linear){
    m_linear_att = linear;
    m_raw->linear_att = linear;
}

void  SpotLight::SetQuadraticAtt(float quadratic){
    m_quadratic_att = quadratic;
    m_raw->quadratic_att = quadratic;
}


}