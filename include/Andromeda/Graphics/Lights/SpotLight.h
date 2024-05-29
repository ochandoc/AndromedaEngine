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
         * @brief Retrieves the position.
         *
         * @return A pointer to the position.
         */
        float* GetPosition();

        /**
         * @brief Retrieves the position and stores it in the provided variables.
         *
         * @param x Reference to store the x-coordinate of the position.
         * @param y Reference to store the y-coordinate of the position.
         * @param z Reference to store the z-coordinate of the position.
         */
        void GetPosition(float& x, float& y, float& z);

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
         * @brief Retrieves the cutoff angle.
         *
         * @return The cutoff angle.
         */
        float GetCuttOff();

        /**
         * @brief Retrieves the cutoff angle and stores it in the provided variable.
         *
         * @param cutt Reference to store the cutoff angle.
         */
        void GetCuttOff(float& cutt);

        /**
         * @brief Retrieves the outer cutoff angle.
         *
         * @return The outer cutoff angle.
         */
        float GetOuterCuttOff();

        /**
         * @brief Retrieves the outer cutoff angle and stores it in the provided variable.
         *
         * @param outer Reference to store the outer cutoff angle.
         */
        void GetOuterCuttOff(float& outer);

        /**
         * @brief Retrieves the constant attenuation.
         *
         * @return The constant attenuation.
         */
        float GetConstantAtt();

        /**
         * @brief Retrieves the constant attenuation and stores it in the provided variable.
         *
         * @param constant Reference to store the constant attenuation.
         */
        void GetConstantAtt(float& constant);

        /**
         * @brief Retrieves the linear attenuation.
         *
         * @return The linear attenuation.
         */
        float GetLinearAtt();

        /**
         * @brief Retrieves the linear attenuation and stores it in the provided variable.
         *
         * @param linear Reference to store the linear attenuation.
         */
        void GetLinearAtt(float& linear);

        /**
         * @brief Retrieves the quadratic attenuation.
         *
         * @return The quadratic attenuation.
         */
        float GetQuadraticAtt();

        /**
         * @brief Retrieves the quadratic attenuation and stores it in the provided variable.
         *
         * @param quadratic Reference to store the quadratic attenuation.
         */
        void GetQuadraticAtt(float& quadratic);

        /**
         * @brief Sets the intensity.
         *
         * @param intensity The new intensity value.
         */
        void SetIntensity(const float intensity);

        /**
         * @brief Sets the diffuse color.
         *
         * @param color An array of 3 elements representing the diffuse color (r, g, b).
         */
        void SetDiffuseColor(float color[3]);

        /**
         * @brief Sets the diffuse color.
         *
         * @param r The red component value.
         * @param g The green component value.
         * @param b The blue component value.
         */
        void SetDiffuseColor(float r, float g, float b);

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
         * @param r The red component value.
         * @param g The green component value.
         * @param b The blue component value.
         */
        void SetSpecularColor(float r, float g, float b);

        /**
         * @brief Sets the specular shininess.
         *
         * @param shininess The new specular shininess.
         */
        void SetSpecularShininess(float shininess);

        /**
         * @brief Sets the position.
         *
         * @param position An array of 3 elements representing the position (x, y, z).
         */
        void SetPosition(float position[3]);

        /**
         * @brief Sets the position.
         *
         * @param x The x-coordinate of the position.
         * @param y The y-coordinate of the position.
         * @param z The z-coordinate of the position.
         */
        void SetPosition(float x, float y, float z);

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

        /**
         * @brief Sets the cutoff angle.
         *
         * @param cutt The new cutoff angle.
         */
        void SetCuttOff(float cutt);

        /**
         * @brief Sets the outer cutoff angle.
         *
         * @param outer The new outer cutoff angle.
         */
        void SetOuterCuttOff(float outer);

        /**
         * @brief Sets the constant attenuation.
         *
         * @param constant The new constant attenuation value.
         */
        void SetConstantAtt(float constant);

        /**
         * @brief Sets the linear attenuation.
         *
         * @param linear The new linear attenuation value.
         */
        void SetLinearAtt(float linear);

        /**
         * @brief Sets the quadratic attenuation.
         *
         * @param quadratic The new quadratic attenuation value.
         */
        void SetQuadraticAtt(float quadratic);

    private:
        float m_intensity = 1.0f;
};
}