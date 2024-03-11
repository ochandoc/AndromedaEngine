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

        virtual void Recalculate() override;
        
    public:
        
        virtual void* GetData() override;

        float  GetEnabled();
        void   GetEnabled(float& enabled);
        float* GetDiffuseColor();
        void   GetDiffuseColor(float &r, float& g, float& b);
        float  GetSpecularStrength();
        void   GetSpecularStrength(float& strength);
        float* GetSpecularColor();
        void   GetSpecularColor(float &r, float& g, float& b);
        float  GetSpecularShininess();
        void   GetSpecularShininess(float& shininess);
        float* GetPosition();
        void   GetPosition(float& x, float& y, float& z);
        float  GetConstantAtt();
        void   GetConstantAtt(float& constant);
        float  GetLinearAtt();
        void   GetLinearAtt(float& linear);
        float  GetQuadraticAtt();
        void   GetQuadraticAtt(float& quadratic);
        
        void  SetEnabled(float e);
        void  SetDiffuseColor(float color[3]);
        void  SetSpecularStrength(float strength);
        void  SetSpecularColor(float color[3]);
        void  SetSpecularColor(float r, float g, float b);
        void  SetSpecularShininess(float shininess);
        void  SetPosition(float direction[3]);
        void  SetPosition(float x, float y, float z);
        void  SetConstantAtt(float constant);
        void  SetLinearAtt(float linear);
        void  SetQuadraticAtt(float quadratic);
};
}