//#include "Andromeda/Graphics/LightOld.h"
//#include "Andromeda/Graphics/Shader.h"
//#include "Backends/OpenGL/OpenGL.h"
//#include "Backends/OpenGL/opengl_uniform_buffer.h"

namespace And {



    /*LightManager::LightManager() {
        m_ambient_OldShader = OldShader::make_default("lights/ambient.shader", "UniformAmbient", LightType::Ambient);
        m_directional_OldShader = OldShader::make_default("lights/directional.shader", "UniformDirectional", LightType::Directional);
        m_point_OldShader = OldShader::make_default("lights/point.shader", "UniformPoint", LightType::Point);
        m_spot_OldShader = OldShader::make_default("lights/spot.shader", "UniformSpot", LightType::Spot);


        m_spot_Shader = MakeShader("lights/spot.shader");
        int size = m_spot_Shader->GetUniformBlockSize(EUniformBlockType::UniformBuffer0);
        m_buffer_matrix = std::make_shared<UniformBuffer>(0,(unsigned int)size);
        
        int size_light = m_spot_Shader->GetUniformBlockSize(EUniformBlockType::UniformBuffer5);
        m_buffer_spot = std::make_shared<UniformBuffer>(5,(unsigned int)size_light);


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


    OldShader* LightManager::bind_light(Light light){
        
        switch (light.type){
        case LightType::Ambient: 
            m_ambient_OldShader->use();
            m_ambient_OldShader->set_default_light(light.ambient.get());
            m_ambient_OldShader->upload_default_data(light.type);
            return m_ambient_OldShader.get();
            break;
        case LightType::Directional:
            //glDepthFunc(GL_LEQUAL);
            //glBlendFunc(GL_ONE, GL_ONE);
            m_directional_OldShader->use();
            m_directional_OldShader->set_default_light(light.directional.get());
            m_directional_OldShader->upload_default_data(light.type);
            return m_directional_OldShader.get();
            break;
        case LightType::Point:
            m_point_OldShader->use();
            m_point_OldShader->set_default_light(light.point.get());
            m_point_OldShader->upload_default_data(light.type);
            return m_point_OldShader.get();
            break;
        case LightType::Spot:
            OpenGLShader* tmp = static_cast<OpenGLShader*>(m_spot_Shader.get());
            //tmp->Use();
            //m_buffer_spot->
            //m_spot_OldShader->use();
            //m_spot_OldShader->set_default_light(light.spot.get());
            //m_spot_OldShader->upload_default_data(light.type);
            return m_spot_OldShader.get();
            break;
        
        default:
            break;
        }

        return nullptr;
    }*/

}