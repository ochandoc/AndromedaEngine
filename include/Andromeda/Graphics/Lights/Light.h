#pragma once
#include "Andromeda/Graphics/Shader.h"

namespace And{

class SpotLight;
class Light{

    public:
        ~Light();
        Light(const Light&) = delete;
        Light(Light&&) = default;



        virtual std::shared_ptr<Shader> GetShader() = 0;

        // Returns all light data in void*
        virtual void* GetData() = 0;

        void SetCastShadows(bool enabled) { m_cast_shadows = enabled; }
        bool GetCastShadows(){return m_cast_shadows;}

        friend class SpotLight;

    protected:
        Light();

        float m_enabled = -1.0f;
        float m_diffuse_color[3] = {0.5f, 0.5f, 0.5f};
        float m_specular_strength = 0.5f;
        float m_specular_color[3] = {0.5f, 0.5f, 0.5f};
        float m_specular_shininess = 0.5f;

        bool m_cast_shadows = true;

    private:

};
}