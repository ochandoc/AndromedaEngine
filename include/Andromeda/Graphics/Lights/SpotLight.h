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

struct MatrixData;

class SpotLight : public Light{

    public:
        SpotLight();
        ~SpotLight();
        SpotLight(const SpotLight&);
        SpotLight(SpotLight&&);
        SpotLight& operator=(const SpotLight& other);

    private:
    
        // Aqui está toda la informacion en bruto, solo se modifica cuando se cambia algun valor y se devuelve en void* GetData
        SpotLightRaw m_raw;

        std::shared_ptr<MatrixData> m_matrix;
        
    public:
        
        virtual void* GetData() override;
        virtual void Recalculate(float aspect_ratio) override;
        virtual float* GetProjectViewMatrix(float aspect_ratio) override;
        virtual float* GetProjectMatrix(float aspect_ratio) override;
        virtual float* GetViewMatrix(float aspect_ratio) override;

        float  GetEnabled();
        void   GetEnabled(float& enabled);
        float* GetDiffuseColor();
        void   GetDiffuseColor(float& r, float& g, float& b);
        float  GetSpecularStrength();
        void   GetSpecularStrength(float& strenght);
        float* GetSpecularColor();
        void   GetSpecularColor(float& r, float& g, float& b);
        float  GetSpecularShininess();
        void   GetSpecularShininess(float& shininess);

        float* GetPosition();
        void   GetPosition(float& x, float& y, float& z);
        float* GetDirection();
        void   GetDirection(float& x, float& y, float& z);
        float  GetCuttOff();
        void   GetCuttOff(float& cutt);
        float  GetOuterCuttOff();
        void   GetOuterCuttOff(float& outer);
        float  GetConstantAtt();
        void   GetConstantAtt(float& constant);
        float  GetLinearAtt();
        void   GetLinearAtt(float& linear);
        float  GetQuadraticAtt();
        void   GetQuadraticAtt(float& quadratic);
        
        void  SetEnabled(bool e);
        void  SetDiffuseColor(float color[3]);
        void  SetDiffuseColor(float r, float g, float b);
        void  SetSpecularStrength(float strength);
        void  SetSpecularColor(float color[3]);
        void  SetSpecularColor(float r, float g, float b);
        void  SetSpecularShininess(float shininess);

        void  SetPosition(float position[3]);
        void  SetPosition(float x, float y, float z);
        void  SetDirection(float direction[3]);
        void  SetDirection(float x, float y, float z);
        void  SetCuttOff(float cutt);
        void  SetOuterCuttOff(float outer);
        void  SetConstantAtt(float constant);
        void  SetLinearAtt(float linear);
        void  SetQuadraticAtt(float quadratic);
};
}