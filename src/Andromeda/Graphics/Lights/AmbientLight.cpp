#include "Andromeda/Graphics/Lights/AmbientLight.h"
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

AmbientLight::AmbientLight() : m_raw(){
    m_must_recalculate = true;
    m_matrix = std::make_shared<MatrixData>();
    m_cast_shadows = false;
}

AmbientLight::~AmbientLight(){}

void* AmbientLight::GetData(){

    return (void*)&m_raw;
}

AmbientLight::AmbientLight(const AmbientLight& other){
    
    this->m_raw = other.m_raw;
    this->m_cast_shadows = other.m_cast_shadows;
    this->m_must_recalculate = other.m_must_recalculate;
    this->m_matrix = other.m_matrix;
}

AmbientLight::AmbientLight(AmbientLight&& other){
    this->m_raw = other.m_raw;
    this->m_cast_shadows = other.m_cast_shadows;
    this->m_must_recalculate = other.m_must_recalculate;
    this->m_matrix = other.m_matrix;
}

AmbientLight& AmbientLight::operator=(const AmbientLight& other){
    this->m_raw = other.m_raw;
    this->m_cast_shadows = other.m_cast_shadows;
    this->m_must_recalculate = other.m_must_recalculate;
    this->m_matrix = other.m_matrix;
    return *this;
}

float AmbientLight::GetEnabled(){
    return m_raw.enabled;;
}

float* AmbientLight::GetDiffuseColor() {
    return &m_raw.diffuse_color[0];
}

void AmbientLight::SetEnabled(bool e){
    e ? m_raw.enabled = 1.0f : m_raw.enabled = 0.0f;
}

void AmbientLight::SetDiffuseColor(float color[3]){
    for (int i = 0; i < 3; i++) {
        m_raw.diffuse_color[i] = color[i];
    }
}

void AmbientLight::SetDiffuseColor(float x, float y, float z){

    m_raw.diffuse_color[0] = x;
    m_raw.diffuse_color[1] = y;
    m_raw.diffuse_color[2] = z;
    
}

void  AmbientLight::GetEnabled(float& enabled){
    enabled = m_raw.enabled;
}

void  AmbientLight::GetDiffuseColor(float& r, float& g, float& b){
    r = m_raw.diffuse_color[0];
    g = m_raw.diffuse_color[1];
    b = m_raw.diffuse_color[2];
}


float* AmbientLight::GetProjectViewMatrix(float aspect_ratio){
    return nullptr;
}
float* AmbientLight::GetProjectMatrix(float aspect_ratio){
    return nullptr;
}
float* AmbientLight::GetViewMatrix(float aspect_ratio){
    return nullptr;    
}

void AmbientLight::Recalculate(float aspect_ratio){}


}