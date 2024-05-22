#include "Andromeda/Graphics/Lights/DirectionalLight.h"
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

DirectionalLight::DirectionalLight() : m_raw(){
    m_must_recalculate = true;
    m_matrix = std::make_shared<MatrixData>();
}

DirectionalLight::~DirectionalLight(){}

void* DirectionalLight::GetData(){

    return (void*)&m_raw;
}

DirectionalLight::DirectionalLight(const DirectionalLight& other){
    
    this->m_raw = other.m_raw;
    this->m_cast_shadows = other.m_cast_shadows;
    this->m_must_recalculate = other.m_must_recalculate;
    this->m_matrix = other.m_matrix;
}

DirectionalLight::DirectionalLight(DirectionalLight&& other){
    this->m_raw = other.m_raw;
    this->m_cast_shadows = other.m_cast_shadows;
    this->m_must_recalculate = other.m_must_recalculate;
    this->m_matrix = other.m_matrix;
}

DirectionalLight& DirectionalLight::operator=(const DirectionalLight& other){
    this->m_raw = other.m_raw;
    this->m_cast_shadows = other.m_cast_shadows;
    this->m_must_recalculate = other.m_must_recalculate;
    this->m_matrix = other.m_matrix;
    return *this;
}

void DirectionalLight::SetCameraPosition(float x, float y, float z){
    m_cam_pos[0] = x;
    m_cam_pos[1] = y;
    m_cam_pos[2] = z;
    m_must_recalculate = true;
}

void DirectionalLight::SetCameraPosition(const float* pos){
    for(int i = 0; i < 3; i++){
        m_cam_pos[i] = pos[i];
    }
    m_must_recalculate = true;
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

void DirectionalLight::SetIntensity(const float intensity) {
    m_intensity = intensity;
    m_raw.diffuse_color[0] = m_base_color[0] * m_intensity;
    m_raw.diffuse_color[1] = m_base_color[1] * m_intensity;
    m_raw.diffuse_color[2] = m_base_color[2] * m_intensity;

}

void DirectionalLight::SetDiffuseColor(float color[3]) {
    for (int i = 0; i < 3; i++) {
        m_base_color[i] = color[i];
        m_raw.diffuse_color[i] = color[i] * m_intensity;
    }
}

void DirectionalLight::SetDiffuseColor(float r, float g, float b) {
    m_base_color[0] = r;
    m_base_color[1] = g;
    m_base_color[2] = b;

    m_raw.diffuse_color[0] = r * m_intensity;
    m_raw.diffuse_color[1] = g * m_intensity;
    m_raw.diffuse_color[2] = b * m_intensity;
}

void  DirectionalLight::SetSpecularStrength(float strength){
    m_raw.specular_strength = strength;
}

void  DirectionalLight::SetSpecularColor(float color[3]){
    for (int i = 0; i < 3; i++) {
        m_raw.specular_color[i] = color[i];
    }

}

void  DirectionalLight::SetSpecularColor(float x, float y, float z){
   
    m_raw.specular_color[0] = x;
    m_raw.specular_color[1] = y;
    m_raw.specular_color[2] = z;
}

void DirectionalLight::SetSpecularShininess(float shininess){
    m_raw.specular_shininess = shininess;
}

void DirectionalLight::SetDirection(float direction[3]){
    for (int i = 0; i < 3; i++) {
        m_raw.direction[i] = direction[i];
    }
    m_must_recalculate = true;
}

void DirectionalLight::SetDirection(float x, float y, float z){

    m_raw.direction[0] = x;
    m_raw.direction[1] = y;
    m_raw.direction[2] = z;
    m_must_recalculate = true;
    
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

void DirectionalLight::Recalculate(float aspect_ratio){
    
    /*if(m_must_recalculate){

        glm::vec3 cam_pos = glm::make_vec3(m_cam_pos);
        glm::vec3 light_dir = glm::make_vec3(m_raw.direction);

        float x = cam_pos.x + ( (-1.0f * light_dir.x) * 50.0f);
        float z = cam_pos.z + ( (-1.0f * light_dir.z) * 50.0f);
        
        glm::vec3 pos = glm::vec3(x, cam_pos.y, z);
        
        glm::vec3 up(0.0f, 1.0f, 0.0f);
        glm::vec3 right = glm::normalize(glm::cross(up, light_dir));
        up = glm::cross(light_dir, right);
        glm::mat4 viewLight = glm::lookAt(pos, pos + glm::normalize(light_dir), up);

        glm::mat4 orto = glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, 1.0f, 300.0f);
        glm::mat4 viewLight_tmp = glm::lookAt(20.0f * cam_pos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        m_matrix->projection_matrix = orto;
        m_matrix->view_matrix = viewLight_tmp;
        m_matrix->projection_view_matrix = orto * viewLight_tmp;
        m_must_recalculate = false;
    }*/


}

float* DirectionalLight::GetProjectViewMatrix(float aspect_ratio){
    Recalculate(aspect_ratio);
    return glm::value_ptr(m_matrix->projection_view_matrix);
}

float* DirectionalLight::GetProjectMatrix(float aspect_ratio){
    Recalculate(aspect_ratio);
    return glm::value_ptr(m_matrix->projection_matrix);
}

float* DirectionalLight::GetViewMatrix(float aspect_ratio){
    Recalculate(aspect_ratio);
    return glm::value_ptr(m_matrix->view_matrix);
}


}