#pragma once
#include "Light.h"
#include "Andromeda/Graphics/Shader.h"

namespace And{

struct AmbientLightRaw {
  float direction[3] = {0.0f, 0.0f, 0.0f};
  float enabled = 1.0f;
  float diffuse_color[3] = {0.5f, 0.5f, 0.5f};
  float ambient_strenght = 1.0f;
  float specular_color[3] = {1.0f, 1.0f, 1.0f};
  float specular_shininess = 1.0f; // 48 bytes
};

struct MatrixData;

class AmbientLight : public Light{

    public:
        AmbientLight();
        ~AmbientLight();
        AmbientLight(const AmbientLight&);
        AmbientLight(AmbientLight&&);
        AmbientLight& operator=(const AmbientLight& other);

    private:
    
        // Aqui está toda la informacion en bruto, solo se modifica cuando se cambia algun valor y se devuelve en void* GetData
        AmbientLightRaw m_raw;
        std::shared_ptr<MatrixData> m_matrix;

        float m_cam_pos[3];

    public:

        virtual void* GetData() override;

        virtual void Recalculate(float aspect_ratio) override;
        virtual float* GetProjectViewMatrix(float aspect_ratio) override;
        virtual float* GetProjectMatrix(float aspect_ratio) override;
        virtual float* GetViewMatrix(float aspect_ratio) override;

        float* GetDiffuseColor();
        void   GetDiffuseColor(float& r, float& g, float& b);
        float  GetAmbientStrenght();

        void   SetDiffuseColor(float color[3]);
        void   SetDiffuseColor(float x, float y, float z);

        void   SetAmbientStrenght(float s);
      
};
}