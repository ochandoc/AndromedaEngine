#pragma once
#include "Light.h"
#include "Andromeda/Graphics/Shader.h"

namespace And{

struct SpotLightRaw;

class SpotLight : public Light{

    public:
        SpotLight();
        ~SpotLight();
        SpotLight(const SpotLight&) = delete;
        SpotLight(SpotLight&&) = default;

        // Hay que crear el shader y puede fallar
        //static std::shared_ptr<SpotLight> MakeSpotLight();


    private:
    
        float m_position[3] = {0.5f, 0.5f, 0.5f};
        float m_direction[3] = {0.5f, 0.5f, 0.5f};
        float m_cutt_off = 0.5f;
        float m_outer_cut_off = 0.5f;
        float m_constant_att = 0.5f;
        float m_linear_att = 0.5f;
        float m_quadratic_att = 0.5f;

        // Aqui está toda la informacion en bruto, solo se modifica cuando se cambia algun valor y se devuelve en void* GetData
        std::shared_ptr<SpotLightRaw> m_raw;
        std::shared_ptr<Shader> m_shader;
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