#pragma once
#include "Light.h"
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


class SpotLight : public Light{

    public:
        SpotLight();
        ~SpotLight();
        SpotLight(const SpotLight&);
        SpotLight(SpotLight&&);
        SpotLight& operator=(const SpotLight& other);

        // Hay que crear el shader y puede fallar
        //static std::shared_ptr<SpotLight> MakeSpotLight();


    private:
    
        // Aqui está toda la informacion en bruto, solo se modifica cuando se cambia algun valor y se devuelve en void* GetData
        SpotLightRaw m_raw;
        //std::shared_ptr<Shader> m_shader;
    public:
        
        //virtual std::shared_ptr<Shader> GetShader() override;
        virtual void* GetData() override;

        float  GetEnabled();
        float* GetDiffuseColor();
        float  GetSpecularStrength();
        float* GetSpecularColor();
        float  GetSpecularShininess();

        float* GetPosition();
        float* GetDirection();
        float  GetCuttOff();
        float  GetOuterCuttOff();
        float  GetConstantAtt();
        float  GetLinearAtt();
        float  GetQuadraticAtt();
        
        void  SetEnabled(float e);
        void  SetDiffuseColor(float color[3]);
        void  SetSpecularStrength(float strength);
        void  SetSpecularColor(float color[3]);
        void  SetSpecularShininess(float shininess);

        void  SetPosition(float position[3]);
        void  SetDirection(float direction[3]);
        void  SetCuttOff(float cutt);
        void  SetOuterCuttOff(float outer);
        void  SetConstantAtt(float constant);
        void  SetLinearAtt(float linear);
        void  SetQuadraticAtt(float quadratic);
};
}