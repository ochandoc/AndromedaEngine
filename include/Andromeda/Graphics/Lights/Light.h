#pragma once
#include "Andromeda/Graphics/Shader.h"
#include "Andromeda/ECS/ComponentBase.h"
#include "Andromeda/Graphics/Camera.h"

namespace And{

class SpotLight;
class DirectionalLight;


class Light : public ComponentBase{

    public:
        Light(){
            m_cast_shadows = true;
            m_must_recalculate = true;
        }
        ~Light(){}
        Light(const Light&) = delete;
        Light(Light&&) = default;

        /**
         * @brief Get all light data in raw format void*
         * 
         * @return void* 
         */
        virtual void* GetData() = 0;

        /**
         * @brief Recalcule all matrix (its called automatically)
         * 
         * @param aspect_ratio 
         */
        virtual void Recalculate(float aspect_ratio) = 0;

        /**
         * @brief Returns Projection * View Matrix
         * 
         * @param aspect_ratio 
         * @return float* 
         */
        virtual float* GetProjectViewMatrix(float aspect_ratio) = 0;

        /**
         * @brief Returns Projections Matrix
         * 
         * @param aspect_ratio 
         * @return float* 
         */
        virtual float* GetProjectMatrix(float aspect_ratio) = 0;

        /**
         * @brief Returns View Matrix
         * 
         * @param aspect_ratio 
         * @return float* 
         */
        virtual float* GetViewMatrix(float aspect_ratio) = 0;

        /**
         * @brief Set Cast Shadow bool
         * 
         */
        void SetCastShadows(bool enabled) { m_cast_shadows = enabled; }

        /**
         * @brief Get the Cast Shadows bool
         * 
         * @return true 
         * @return false 
         */
        bool GetCastShadows(){return m_cast_shadows;}

        void SetEnabled(bool e) { m_enabled = e; }
        bool GetEnabled() { return m_enabled; }

        friend class SpotLight;
        friend class DirectionalLight;

    protected:

        bool m_cast_shadows;
        bool m_must_recalculate;
        bool m_enabled = true;

    private:

};
}