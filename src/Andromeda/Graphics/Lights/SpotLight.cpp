#include "Andromeda/Graphics/Lights/SpotLight.h"
#include "Andromeda/Graphics/Shader.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace And{

struct MatrixData{
    glm::mat4 projection_view_matrix;
    glm::mat4 projection_matrix;
    glm::mat4 view_matrix;
};

SpotLight::SpotLight() : m_raw(){
    m_must_recalculate = true;
    m_matrix = std::make_shared<MatrixData>();

}

SpotLight::~SpotLight(){}

void* SpotLight::GetData(){

    return (void*)&m_raw;
}

SpotLight::SpotLight(const SpotLight& other){
    
    this->m_raw = other.m_raw;
    this->m_cast_shadows = other.m_cast_shadows;
    this->m_must_recalculate = other.m_must_recalculate;
    this->m_matrix = other.m_matrix;
}

SpotLight::SpotLight(SpotLight&& other){
    this->m_raw = other.m_raw;
    this->m_cast_shadows = other.m_cast_shadows;
    this->m_must_recalculate = other.m_must_recalculate;
    this->m_matrix = other.m_matrix;
}

SpotLight& SpotLight::operator=(const SpotLight& other){
    this->m_raw = other.m_raw;
    this->m_cast_shadows = other.m_cast_shadows;
    this->m_must_recalculate = other.m_must_recalculate;
    this->m_matrix = other.m_matrix;
    return *this;
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

void SpotLight::SetDiffuseColor(float color[3]){
    for (int i = 0; i < 3; i++) {
        m_raw.diffuse_color[i] = color[i];
    }
}


void  SpotLight::SetSpecularStrength(float strength){
    m_raw.specular_strength = strength;
    //m_must_recalculate = true;
}

void  SpotLight::SetSpecularColor(float color[3]){
    for (int i = 0; i < 3; i++) {
        m_raw.specular_color[i] = color[i];
    }
    //m_must_recalculate = true;

}

void SpotLight::SetSpecularShininess(float shininess){
    m_raw.specular_shininess = shininess;
    //m_must_recalculate = true;
}

void SpotLight::SetPosition(float position[3]){
    for (int i = 0; i < 3; i++) {
        m_raw.position[i] = position[i];
    }
    m_must_recalculate = true;
}

void SpotLight::SetDirection(float direction[3]){
    for (int i = 0; i < 3; i++) {
        m_raw.direction[i] = direction[i];
    }
    m_must_recalculate = true;
}

void SpotLight::SetCuttOff(float cutt){
    m_raw.cutt_off = cutt;
    m_must_recalculate = true;
}

void SpotLight::SetOuterCuttOff(float outer){
    m_raw.outer_cut_off = outer;
    m_must_recalculate = true;
}

void SpotLight::SetConstantAtt(float constant){
    m_raw.constant_att = constant;
    //m_must_recalculate = true;
}

void SpotLight::SetLinearAtt(float linear){
    m_raw.linear_att = linear;
    //m_must_recalculate = true;
}

void  SpotLight::SetQuadraticAtt(float quadratic){
    m_raw.quadratic_att = quadratic;
    //m_must_recalculate = true;
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
    //m_must_recalculate = true;
}

void  SpotLight::SetSpecularColor(float r, float g, float b){
    m_raw.specular_color[0] = r;
    m_raw.specular_color[1] = g;
    m_raw.specular_color[2] = b;
    //m_must_recalculate = true;
}

void  SpotLight::SetPosition(float x, float y, float z){
    m_raw.position[0] = x;
    m_raw.position[1] = y;
    m_raw.position[2] = z;
    m_must_recalculate = true;
}

void  SpotLight::SetDirection(float x, float y, float z){
    m_raw.direction[0] = x;
    m_raw.direction[1] = y;
    m_raw.direction[2] = z;
    m_must_recalculate = true;
}

void SpotLight::Recalculate(float aspect_ratio){
    if (m_must_recalculate) {

        glm::vec3 pos = glm::make_vec3(&m_raw.position[0]);
        glm::vec3 dir = glm::make_vec3(&m_raw.direction[0]);

        glm::vec3 up(0.0f, 1.0f, 0.0f);
        glm::vec3 right = glm::normalize(glm::cross(up, dir));
        up = glm::cross(dir, right);
        glm::mat4 viewLight = glm::lookAt(pos, pos + glm::normalize(dir), up);

        float fov_radians = glm::radians(m_raw.outer_cut_off) * 1.5f;
        float near = 10.0f;
        float far = 310.0f;
        glm::mat4 projLight = glm::perspective(fov_radians, aspect_ratio, near, far);
        glm::mat4 projViewLight = projLight * viewLight;

        m_matrix->projection_matrix = projLight;
        m_matrix->view_matrix = viewLight;
        m_matrix->projection_view_matrix = projViewLight;

        m_must_recalculate = false;
    }
}

float* SpotLight::GetProjectViewMatrix(float aspect_ratio){

    
    Recalculate(aspect_ratio);
    return glm::value_ptr(m_matrix->projection_view_matrix);
}

float* SpotLight::GetProjectMatrix(float aspect_ratio){
    Recalculate(aspect_ratio);
    return glm::value_ptr(m_matrix->projection_matrix);
}

float* SpotLight::GetViewMatrix(float aspect_ratio){
    Recalculate(aspect_ratio);
    return glm::value_ptr(m_matrix->view_matrix);
}




}