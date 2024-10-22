#pragma once

#include "Andromeda/Misc/CoreMiscDefines.h"
#include "Andromeda/HAL/Types.h"
#include "Andromeda/Graphics/Geometry.h"
#include "Andromeda/Graphics/RenderTarget.h"
#include "Andromeda/Graphics/FlyCamera.h"
#include "Andromeda/Graphics/Camera.h"
#include "Andromeda/ECS/Components/MeshComponent.h"
#include "Andromeda/ECS/Scene.h"
#include "Andromeda/Graphics/Lights/SpotLight.h"
#include "Andromeda/Graphics/Lights/DirectionalLight.h"
#include "Andromeda/Graphics/Lights/PointLight.h"
#include "Andromeda/Graphics/Material.h"
#include "Backends/OpenGL/OpenGLShader.h"
#include "Backends/OpenGL/opengl_skybox_texture.h"
#include "Backends/OpenGL/OpenGLRenderTarget.h"

#include "Andromeda/Graphics/Renderer.h"


namespace And{

    class RendererOpenGL : public Renderer {
    public:

        RendererOpenGL(Window& window);
        virtual ~RendererOpenGL() override;

        void new_frame() override;
        void end_frame() override;
        void set_viewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height) override;
        void set_clear_color(float* color) override;
        void set_camera(CameraBase* cam) override;

        void enable_skybox(bool value);
        virtual void set_skybox_texture(std::shared_ptr<SkyboxTexture> texture);

        void draw_forward(EntityComponentSystem& entity) override;
        void draw_deferred(EntityComponentSystem& entity) override;
        void draw_pbr(EntityComponentSystem& entity) override;

        virtual void enable_gray_scale(bool enable) { enable_postprocess = enable; }

    private:
        void draw_triangle(Triangle* t);
        void draw_obj(MeshComponent* obj, Light* l, TransformComponent* tran);
        void draw_obj_shadows(MeshComponent* obj, TransformComponent* trans, SpotLight* l);
        void draw_obj_shadows(MeshComponent* obj, TransformComponent* trans, DirectionalLight* l);
        void draw_obj_shadows(MeshComponent* obj, TransformComponent* trans, PointLight* l, float* dir);
        void draw_deep_obj(MeshComponent* obj, std::shared_ptr<Shader> s, TransformComponent* tran, float* view, float* projection);

        void RenderLight(std::shared_ptr<And::RenderTarget> shadow_buffer, Light* light);
        void RenderPBRLight(std::shared_ptr<And::RenderTarget> shadow_buffer, Light* light);

        void upload_light(Light* l);

        void draw_scene(Scene& scene, Shader* s);

        void draw_shadows(SpotLight* l, MeshComponent* obj, TransformComponent* tran);
        void draw_shadows(DirectionalLight* l, MeshComponent* obj, TransformComponent* tran);
        void draw_shadows(PointLight* l, MeshComponent* obj, TransformComponent* tran, float* dir);

        void DrawSkyBox();
        void DrawBillBoard(EntityComponentSystem& ecs, bool is_pbr = true);
        void RenderBillBoard(EntityComponentSystem& ecs);

        void CheckMaterial(OpenGLShader* s, std::shared_ptr<Material> mat);
        void CheckPBRMaterial(OpenGLShader* s, std::shared_ptr<Material> mat);
        
        void ResetTransforms(EntityComponentSystem& ecs);

        std::shared_ptr<RenderTarget> get_shadow_buffer();
        std::vector<std::shared_ptr<RenderTarget>> get_shadow_buffer_pointLight();

        bool m_bDrawOnTexture;
        bool enable_postprocess = false;

        CameraBase* m_UserCamera;
        FlyCamera m_DefaultCamera;

        std::shared_ptr<RenderTarget> m_shadows_buffer_;
        std::shared_ptr<RenderTarget> m_gBuffer_;
        std::shared_ptr<RenderTarget> m_full_quad_;
        std::shared_ptr<OpenGLRenderTarget> m_PostProcessRenderTarget;
        

        std::vector<std::shared_ptr<RenderTarget>> m_shadows_buffer_pointLight;

        std::shared_ptr<Shader> m_shadow_shader;
        std::shared_ptr<Shader> m_depth_shader;
        std::shared_ptr<OpenGLShader> m_PostProcessShader;

        std::shared_ptr<Shader> m_shader_ambient;
        std::shared_ptr<Shader> m_shader_directional;
        std::shared_ptr<Shader> m_shader_shadows_directional;
        std::shared_ptr<Shader> m_shader_point;
        std::shared_ptr<Shader> m_shader_shadows_point;
        std::shared_ptr<Shader> m_shader_spot;
        std::shared_ptr<Shader> m_shader_shadows_spot;

        std::shared_ptr<Shader> m_shader_geometry;
        std::shared_ptr<Shader> m_shader_quad_directional_shadows;
        std::shared_ptr<Shader> m_shader_quad_ambient;
        std::shared_ptr<Shader> m_shader_quad_spot_shadows;
        std::shared_ptr<Shader> m_shader_quad_point_shadows;
        
        std::shared_ptr<Shader> m_shader_quad_directional;
        std::shared_ptr<Shader> m_shader_quad_spot;
        std::shared_ptr<Shader> m_shader_quad_point;

        // PBR Shaders
        std::shared_ptr<Shader> m_shader_pbr_geometry;
        std::shared_ptr<Shader> m_shader_quad_ambient_pbr;
        std::shared_ptr<Shader> m_shader_quad_point_pbr;
        std::shared_ptr<Shader> m_shader_quad_spot_pbr;
        std::shared_ptr<Shader> m_shader_quad_directional_pbr;

        std::shared_ptr<Shader> m_shader_skybox;
        std::shared_ptr<Shader> m_shader_billboard;

        std::shared_ptr<UniformBuffer> m_buffer_matrix; // 208
        std::shared_ptr<UniformBuffer> m_buffer_matrix_pointLight; // 208 + 16 * 5
        std::shared_ptr<UniformBuffer> m_buffer_ambient_light; // 48
        std::shared_ptr<UniformBuffer> m_buffer_directional_light; // 48
        std::shared_ptr<UniformBuffer> m_buffer_point_light; // 64
        std::shared_ptr<UniformBuffer> m_buffer_spot_light; // 96
        std::shared_ptr<Direction> m_directions;

        Material m_material_default;
        Material m_material_error_default;

        float dMesh[32] = {
          -1.0f, -1.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f, // down-left
          -1.0f, +1.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f, // up-left
          +1.0f, +1.0f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f, // up-right
          +1.0f, -1.0f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f, // down-right
        };

        unsigned int dIndices[6] = {
             0, 2, 1, 2, 0, 3
        };

 
        unsigned int m_quad_vbo;
        unsigned int m_quad_vao;
        unsigned int m_quad_ebo;
 
        
        RawMesh m_skybox_mesh;

        unsigned int m_skybox_vao;
        unsigned int m_skybox_vbo;

        bool m_skybox_enabled;
        std::shared_ptr<OpenGLSkyBoxTexture> m_skybox_tex;


    protected:

        Window& m_Window;
        std::shared_ptr<RenderTarget> m_RenderTarget;

    };
}