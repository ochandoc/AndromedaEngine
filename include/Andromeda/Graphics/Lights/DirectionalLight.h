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
         * @brief Sets the camera position.
         *
         * @param x The x-coordinate of the camera position.
         * @param y The y-coordinate of the camera position.
         * @param z The z-coordinate of the camera position.
         */
        void SetCameraPosition(float x, float y, float z);

        /**
         * @brief Sets the camera position.
         *
         * @param pos A pointer to an array containing the camera position (x, y, z).
         */
        void SetCameraPosition(const float* pos);

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
         * @brief Retrieves the specular strength.
         *
         * @return The specular strength.
         */
        float GetSpecularStrength();

        /**
         * @brief Retrieves the specular strength and stores it in the provided variable.
         *
         * @param strength Reference to store the specular strength.
         */
        void GetSpecularStrength(float& strength);

        /**
         * @brief Retrieves the specular color.
         *
         * @return A pointer to the specular color.
         */
        float* GetSpecularColor();

        /**
         * @brief Retrieves the specular color and stores it in the provided variables.
         *
         * @param r Reference to store the red component value.
         * @param g Reference to store the green component value.
         * @param b Reference to store the blue component value.
         */
        void GetSpecularColor(float& r, float& g, float& b);

        /**
         * @brief Retrieves the specular shininess.
         *
         * @return The specular shininess.
         */
        float GetSpecularShininess();

        /**
         * @brief Retrieves the specular shininess and stores it in the provided variable.
         *
         * @param shininess Reference to store the specular shininess.
         */
        void GetSpecularShininess(float& shininess);

        /**
         * @brief Retrieves the direction.
         *
         * @return A pointer to the direction.
         */
        float* GetDirection();

        /**
         * @brief Retrieves the direction and stores it in the provided variables.
         *
         * @param x Reference to store the x-coordinate of the direction.
         * @param y Reference to store the y-coordinate of the direction.
         * @param z Reference to store the z-coordinate of the direction.
         */
        void GetDirection(float& x, float& y, float& z);

        /**
         * @brief Sets the intensity.
         *
         * @param value The new intensity value.
         */
        void SetIntensity(float value);

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
         * @brief Sets the specular strength.
         *
         * @param strength The new specular strength.
         */
        void SetSpecularStrength(float strength);

        /**
         * @brief Sets the specular color.
         *
         * @param color An array of 3 elements representing the specular color (r, g, b).
         */
        void SetSpecularColor(float color[3]);

        /**
         * @brief Sets the specular color.
         *
         * @param x The red component value.
         * @param y The green component value.
         * @param z The blue component value.
         */
        void SetSpecularColor(float x, float y, float z);

        /**
         * @brief Sets the specular shininess.
         *
         * @param shininess The new specular shininess.
         */
        void SetSpecularShininess(float shininess);

        /**
         * @brief Sets the direction.
         *
         * @param direction An array of 3 elements representing the direction (x, y, z).
         */
        void SetDirection(float direction[3]);

        /**
         * @brief Sets the direction.
         *
         * @param x The x-coordinate of the direction.
         * @param y The y-coordinate of the direction.
         * @param z The z-coordinate of the direction.
         */
        void SetDirection(float x, float y, float z);

    private:
        float m_intensity = 1.0f;
};
}