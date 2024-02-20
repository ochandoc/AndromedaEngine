#include "Andromeda/Graphics/Light.h"
#include "Andromeda/Graphics/Shader.h"
#include "Backends/OpenGL/OpenGL.h"

namespace And {



    LightManager::LightManager() {
        m_ambient_shader = Shader::make_default("lights/ambient.shader", "UniformAmbient", LightType::Ambient);
        m_directional_shader = Shader::make_default("lights/directional.shader", "UniformDirectional", LightType::Directional);
        m_point_shader = Shader::make_default("lights/point.shader", "UniformPoint", LightType::Point);
        m_spot_shader = Shader::make_default("lights/spot.shader", "UniformSpot", LightType::Spot);
    }

    LightManager::~LightManager() {

    }


    void LightManager::add_light(std::shared_ptr<DirectionalLight> l) {

        Light default_light;

        default_light.directional = l;
        default_light.type = LightType::Directional;
        

        m_Lights.push_back(default_light);
    }

    void LightManager::add_light(std::shared_ptr<AmbientLight> l) {
        Light default_light;

        default_light.ambient = l;
        default_light.type = LightType::Ambient;

        m_Lights.push_back(default_light);
    }

    void LightManager::add_light(std::shared_ptr<PointLight> l) {

        Light default_light;

        default_light.point = l;
        default_light.type = LightType::Point;

        m_Lights.push_back(default_light);
    }

    void LightManager::add_light(std::shared_ptr<SpotLight> l) {
        Light default_light;

        default_light.spot = l;
        default_light.type = LightType::Spot;

        m_Lights.push_back(default_light);
    }


    Shader* LightManager::bind_light(Light light){
        
        switch (light.type){
        case LightType::Ambient: 
            m_ambient_shader->use();
            m_ambient_shader->set_default_light(light.ambient.get());
            m_ambient_shader->upload_default_data(light.type);
            return m_ambient_shader.get();
            break;
        case LightType::Directional:
            //glDepthFunc(GL_LEQUAL);
            //glBlendFunc(GL_ONE, GL_ONE);
            m_directional_shader->use();
            m_directional_shader->set_default_light(light.directional.get());
            m_directional_shader->upload_default_data(light.type);
            return m_directional_shader.get();
            break;
        case LightType::Point:
            m_point_shader->use();
            m_point_shader->set_default_light(light.point.get());
            m_point_shader->upload_default_data(light.type);
            return m_point_shader.get();
            break;
        case LightType::Spot:
            m_spot_shader->use();
            m_spot_shader->set_default_light(light.spot.get());
            m_spot_shader->upload_default_data(light.type);
            return m_spot_shader.get();
            break;
        
        default:
            break;
        }

        return nullptr;
    }

}