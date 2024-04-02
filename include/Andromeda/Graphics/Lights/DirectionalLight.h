#pragma once
#include "Light.h"
#include "Andromeda/Graphics/Shader.h"

namespace And{

struct DirectionalLightRaw {
  float direction[3] = {0.5f, 0.5f, 0.5f};
  float enabled = 0.5f;
  float diffuse_color[3] = {0.5f, 0.5f, 0.5f};
  float specular_strength = 0.5f;
  float specular_color[3] = {0.5f, 0.5f, 0.5f};
  float specular_shininess = 0.5f; // 48 bytes
};

struct MatrixData;

class DirectionalLight : public Light{

    public:
        DirectionalLight();
        ~DirectionalLight();
        DirectionalLight(const DirectionalLight&);
        DirectionalLight(DirectionalLight&&);
        DirectionalLight& operator=(const DirectionalLight& other);

    private:
    
        // Aqui está toda la informacion en bruto, solo se modifica cuando se cambia algun valor y se devuelve en void* GetData
        DirectionalLightRaw m_raw;
        std::shared_ptr<MatrixData> m_matrix;

        float m_cam_pos[3];

    public:
        
        virtual void* GetData() override;
        virtual void Recalculate(float aspect_ratio) override;
        virtual float* GetProjectViewMatrix(float aspect_ratio) override;
        virtual float* GetProjectMatrix(float aspect_ratio) override;
        virtual float* GetViewMatrix(float aspect_ratio) override;

        void SetCameraPosition(float x, float y, float z);
        void SetCameraPosition(const float* pos);

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
        float* GetDirection();
        void   GetDirection(float& x, float& y, float& z);
        
        void   SetEnabled(float e);
        void   SetDiffuseColor(float color[3]);
        void   SetDiffuseColor(float x, float y, float z);
        void   SetSpecularStrength(float strength);
        void   SetSpecularColor(float color[3]);
        void   SetSpecularColor(float x, float y, float z);
        void   SetSpecularShininess(float shininess);
        void   SetDirection(float direction[3]);
        void   SetDirection(float x, float y, float z);
};
}