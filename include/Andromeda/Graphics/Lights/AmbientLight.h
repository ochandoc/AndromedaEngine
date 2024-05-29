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

        /**
         * @brief Retrieves the data.
         *
         * @return A pointer to the data.
         */
        virtual void* GetData() override;

        /**
         * @brief Recalculates values based on the provided aspect ratio.
         *
         * @param aspect_ratio The aspect ratio used for the calculation.
         */
        virtual void Recalculate(float aspect_ratio) override;

        /**
         * @brief Retrieves the projection-view matrix based on the provided aspect ratio.
         *
         * @param aspect_ratio The aspect ratio used for the calculation.
         * @return A pointer to the projection-view matrix.
         */
        virtual float* GetProjectViewMatrix(float aspect_ratio) override;

        /**
         * @brief Retrieves the projection matrix based on the provided aspect ratio.
         *
         * @param aspect_ratio The aspect ratio used for the calculation.
         * @return A pointer to the projection matrix.
         */
        virtual float* GetProjectMatrix(float aspect_ratio) override;

        /**
         * @brief Retrieves the view matrix based on the provided aspect ratio.
         *
         * @param aspect_ratio The aspect ratio used for the calculation.
         * @return A pointer to the view matrix.
         */
        virtual float* GetViewMatrix(float aspect_ratio) override;

        /**
         * @brief Retrieves the diffuse color.
         *
         * @return A pointer to the diffuse color.
         */
        float* GetDiffuseColor();

        /**
         * @brief Retrieves the diffuse color and stores it in the provided variables.
         *
         * @param r Reference to store the red component value.
         * @param g Reference to store the green component value.
         * @param b Reference to store the blue component value.
         */
        void GetDiffuseColor(float& r, float& g, float& b);

        /**
         * @brief Retrieves the ambient strength.
         *
         * @return The ambient strength.
         */
        float GetAmbientStrenght();

        /**
         * @brief Sets the diffuse color.
         *
         * @param color An array of 3 elements representing the diffuse color (r, g, b).
         */
        void SetDiffuseColor(float color[3]);

        /**
         * @brief Sets the diffuse color.
         *
         * @param x The red component value.
         * @param y The green component value.
         * @param z The blue component value.
         */
        void SetDiffuseColor(float x, float y, float z);

        /**
         * @brief Sets the ambient strength.
         *
         * @param s The new ambient strength.
         */
        void SetAmbientStrenght(float s);
      
};
}