#pragma once
#include "Light.h"
#include "Andromeda/Graphics/Shader.h"

namespace And{

struct PointLightRaw {
    float position[3] = {0.5f, 0.5f, 0.5f};
    float specular_strength = 0.5f;
    float diffuse_color[3] = {0.5f, 0.5f, 0.5f};
    float specular_shininess = 0.5f;
    float specular_color[3] = {0.5f, 0.5f, 0.5f};
    float constant_att = 0.5f;
    float linear_att = 0.5f;
    float quadratic_att = 0.5f;
    float enabled = 0.5f;
    float attenuation = 1.0f; // 64 bytes
};


class PointLight : public Light{

    public:
        PointLight();
        ~PointLight();
        PointLight(const PointLight&);
        PointLight(PointLight&&);
        PointLight& operator=(const PointLight& other);

    private:
    
        // Aqui está toda la informacion en bruto, solo se modifica cuando se cambia algun valor y se devuelve en void* GetData
        PointLightRaw m_raw;
        
    public:
        
        virtual void* GetData() override;

        float  GetEnabled();
        float* GetDiffuseColor();
        float  GetSpecularStrength();
        float* GetSpecularColor();
        float  GetSpecularShininess();
        float* GetPosition();
        float  GetConstantAtt();
        float  GetLinearAtt();
        float  GetQuadraticAtt();
        
        void  SetEnabled(float e);
        void  SetDiffuseColor(float color[3]);
        void  SetSpecularStrength(float strength);
        void  SetSpecularColor(float color[3]);
        void  SetSpecularShininess(float shininess);
        void  SetPosition(float direction[3]);
        void  SetConstantAtt(float constant);
        void  SetLinearAtt(float linear);
        void  SetQuadraticAtt(float quadratic);
};
}