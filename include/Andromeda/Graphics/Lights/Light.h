#pragma once
#include "Andromeda/Graphics/Shader.h"
#include "Andromeda/ECS/ComponentBase.h"

namespace And{

class SpotLight;
class DirectionalLight;
class Light : public ComponentBase{

    public:
        Light(){
            m_cast_shadows = true;
        }
        ~Light(){}
        Light(const Light&) = delete;
        Light(Light&&) = default;

        // Returns all light data in void*
        virtual void* GetData() = 0;

        void SetCastShadows(bool enabled) { m_cast_shadows = enabled; }
        bool GetCastShadows(){return m_cast_shadows;}

        friend class SpotLight;
        friend class DirectionalLight;

    protected:

        virtual void Recalculate() = 0;
        bool m_cast_shadows;
        bool m_must_recalculate;

    private:

};
}