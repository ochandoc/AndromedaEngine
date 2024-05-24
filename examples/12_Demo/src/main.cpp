#include <assert.h>

#include <stdio.h>
#include <iostream>
#include <fstream>

#include <functional>
#include <algorithm>
#include <utility>

#include <optional>
#include <memory>

#include <string>
#include <vector>
#include <queue>

#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <chrono>
#include <iostream>
#include <random>
#include <cmath>
#include <cstdlib>
#include "Andromeda.h"

const float PI = 3.14159265f;

const float Global_Scale = 20.0f;

static And::Entity* point_tv;

static And::Entity* spot_estanteria0;
static And::Entity* spot_estanteria1;
static And::Entity* spot_estanteria2;

static And::Entity* spheres_disco[3];

static And::Entity* spot_cuadro[3];

static And::Entity* points_habitaculo[4];

static And::Entity* point_hexagonos;

void CreateHabitaculo(And::EntityComponentSystem& ecs, And::Entity* parent) {

    const float scale_suelo = 75.0f;
    
    And::MaterialComponent mat_com;
    std::shared_ptr<And::Material> mat = std::make_shared<And::Material>();
    std::shared_ptr<And::Texture> tex = And::MakeTexture("demo/textures/suelo_3/albedo.png");
    std::shared_ptr<And::Texture> normals = And::MakeTexture("demo/textures/suelo_3/normals.png");
    std::shared_ptr<And::Texture> ao = And::MakeTexture("demo/textures/suelo_3/ao.png");
    std::shared_ptr<And::Texture> metallic = And::MakeTexture("demo/textures/suelo_3/metallic.png");
    std::shared_ptr<And::Texture> rou = And::MakeTexture("demo/textures/suelo_3/roughness.png");
    mat->SetColorTexture(tex);
    mat->SetNormalTexture(normals);
    mat->SetAmbientOclusionTexture(ao);
    mat->SetMetallicTexture(metallic);
    mat->SetRoughnessTexture(rou);
    mat_com.SetMaterial(mat);

    And::MeshComponent MC;
    MC.MeshOBJ = And::Geometry::load("cube.obj");
    And::RawMesh raw_mesh_vela_tmp(MC.MeshOBJ->get_vertices(), MC.MeshOBJ->get_indices());
    std::shared_ptr<And::Mesh> mesh_vela_tmp = std::make_shared<And::Mesh>(raw_mesh_vela_tmp);
    MC.SetMesh(mesh_vela_tmp);

    And::TransformComponent tr;
    tr.SetPosition(0.0f, 0.0f, 0.0f);
    tr.SetRotation(0.0f, 0.0f, 0.0f);
    tr.SetScale(scale_suelo, 1.0f, scale_suelo * 1.5f);
    tr.HasRigidBody(false);
    //tr.SetParent(parent->get_component<And::TransformComponent>());
    ecs.new_entity(mat_com, MC, tr);


    And::MaterialComponent mat_com_pared;
    std::shared_ptr<And::Material> mat_pared = std::make_shared<And::Material>();
    std::shared_ptr<And::Texture> tex_pared = And::MakeTexture("demo/textures/suelo_7/albedo.png");
    std::shared_ptr<And::Texture> normals_pared = And::MakeTexture("demo/textures/suelo_7/normals.png");
    std::shared_ptr<And::Texture> ao_pared = And::MakeTexture("demo/textures/suelo_7/ao.png");
    std::shared_ptr<And::Texture> metallic_pared = And::MakeTexture("demo/textures/suelo_7/metallic.png");
    std::shared_ptr<And::Texture> rou_pared = And::MakeTexture("demo/textures/suelo_7/roughness.png");
    mat_pared->SetColorTexture(tex_pared);
    mat_pared->SetNormalTexture(normals_pared);
    mat_pared->SetAmbientOclusionTexture(ao_pared);
    mat_pared->SetMetallicTexture(metallic_pared);
    mat_pared->SetRoughnessTexture(rou_pared);
    mat_com_pared.SetMaterial(mat_pared);

    
    tr.SetScale(1.0f, 30.0f, scale_suelo * 1.5f);
    tr.SetPosition(scale_suelo * -0.5f, 15.0f, 0.0f);
    //tr.SetParent(parent->get_component<And::TransformComponent>());
    ecs.new_entity(mat_com_pared, MC, tr);
    
    tr.SetScale(1.0f, 30.0f, scale_suelo * 1.5f);
    tr.SetPosition(scale_suelo * 0.5f, 15.0f, 0.0f);
    //tr.SetParent(parent->get_component<And::TransformComponent>());
    ecs.new_entity(mat_com_pared, MC, tr);
    
    tr.SetScale(scale_suelo, 30.0f, 1.0f);
    tr.SetPosition(0.0f, 15.0f, scale_suelo * 0.75f);
    //tr.SetParent(parent->get_component<And::TransformComponent>());
    ecs.new_entity(mat_com_pared, MC, tr);



}

void CreateLighting(And::EntityComponentSystem& ecs) {

    And::AmbientLight ambient;
    ambient.SetDiffuseColor(1.0f, 1.0f, 1.0f);
    ambient.SetAmbientStrenght(0.001f);
    //ecs.new_entity(ambient);

    const float esquina_x = 30.0f;
    const float esquina_z = 49.0f;
    
    // Point esquina
    {
        const float intensity = 100.0f;
        And::PointLight point;
        point.SetPosition(esquina_x, 26.0f, esquina_z);
        point.SetSpecularColor(1.0f, 1.0f, 1.0f);
        point.SetSpecularShininess(32.0f);
        point.SetSpecularStrength(0.003f);
        point.SetConstantAtt(1.0f);
        point.SetLinearAtt(0.09f);
        point.SetQuadraticAtt(0.032f);
        point.SetEnabled(true);
        point.SetCastShadows(true);
        point.SetIntensity(intensity);
        point.SetDiffuseColor(1.0f, 1.0f, 1.0f);
        points_habitaculo[0] = ecs.new_entity(point);
    }
    
    // Point esquina2
    {
        const float intensity = 100.0f;
        And::PointLight point;
        point.SetPosition(-esquina_x, 26.0f, -esquina_z);
        point.SetSpecularColor(1.0f, 1.0f, 1.0f);
        point.SetSpecularShininess(32.0f);
        point.SetSpecularStrength(0.003f);
        point.SetConstantAtt(1.0f);
        point.SetLinearAtt(0.09f);
        point.SetQuadraticAtt(0.032f);
        point.SetEnabled(true);
        point.SetCastShadows(true);
        point.SetIntensity(intensity);
        point.SetDiffuseColor(1.0f, 1.0f, 1.0f);
        points_habitaculo[1] = ecs.new_entity(point);
    }
    
    // Point esquina3
    {
        const float intensity = 100.0f;
        And::PointLight point;
        point.SetPosition(esquina_x, 26.0f, -esquina_z);
        point.SetSpecularColor(1.0f, 1.0f, 1.0f);
        point.SetSpecularShininess(32.0f);
        point.SetSpecularStrength(0.003f);
        point.SetConstantAtt(1.0f);
        point.SetLinearAtt(0.09f);
        point.SetQuadraticAtt(0.032f);
        point.SetEnabled(true);
        point.SetCastShadows(true);
        point.SetIntensity(intensity);
        point.SetDiffuseColor(1.0f, 1.0f, 1.0f);
        points_habitaculo[2] = ecs.new_entity(point);
    }
    
    // Point esquina4
    {
        const float intensity = 100.0f;
        And::PointLight point;
        point.SetPosition(-esquina_x, 26.0f, esquina_z);
        point.SetSpecularColor(1.0f, 1.0f, 1.0f);
        point.SetSpecularShininess(32.0f);
        point.SetSpecularStrength(0.003f);
        point.SetConstantAtt(1.0f);
        point.SetLinearAtt(0.09f);
        point.SetQuadraticAtt(0.032f);
        point.SetEnabled(true);
        point.SetCastShadows(true);
        point.SetIntensity(intensity);
        point.SetDiffuseColor(1.0f, 1.0f, 1.0f);
        //ecs.new_entity(point);
    }
    
    // Point medio
    {
        const float intensity = 100.0f;
        And::PointLight point;
        point.SetPosition(10.0f, 26.0f, -15.0f);
        point.SetSpecularColor(1.0f, 1.0f, 1.0f);
        point.SetSpecularShininess(32.0f);
        point.SetSpecularStrength(0.003f);
        point.SetConstantAtt(1.0f);
        point.SetLinearAtt(0.09f);
        point.SetQuadraticAtt(0.032f);
        point.SetEnabled(true);
        point.SetCastShadows(true);
        point.SetIntensity(intensity);
        point.SetDiffuseColor(1.0f, 1.0f, 1.0f);
        points_habitaculo[3] = ecs.new_entity(point);
    }

    // Point detras tele
    {
        const float intensity = 200.0f;
        And::PointLight point;
        point.SetPosition(34.5f,9.0f, -15.0f);
        point.SetSpecularColor(0.0f, 0.0f, 0.0f);
        point.SetSpecularShininess(2.0f);
        point.SetSpecularStrength(0.0f);
        point.SetConstantAtt(1.0f);
        point.SetLinearAtt(0.0f);
        point.SetQuadraticAtt(0.0f);
        point.SetEnabled(true);
        point.SetCastShadows(true);
        point.SetIntensity(intensity);
        point.SetDiffuseColor(1.0f, 0.01f, 0.01f);
        point_tv = ecs.new_entity(point);
    }

    // Directional
    {
        And::DirectionalLight directional;
        directional.SetDiffuseColor(1.0f, 1.0f, 1.0f);
        directional.SetDirection(0.5f, -0.5f, 0.5f);
        directional.SetSpecularColor(1.0f, 1.0f, 1.0f);
        directional.SetSpecularShininess(32.0f);
        directional.SetSpecularStrength(0.003f);
        directional.SetEnabled(true);
        directional.SetCastShadows(true);
        directional.SetIntensity(1.0f);
        // ecs.new_entity(directional);
    }
    
    const float distance_cuadros = 20.0f;
    // Spot cuadro 1
    {
        And::SpotLight spot{};
        //spot.SetPosition(33.0f, 20.0f, 0.0f);
        spot.SetPosition(-35.5f, 32.0f, distance_cuadros);
        spot.SetDirection(0.0f, -1.0f, 0.0f);
        //spot.SetDiffuseColor(0.976f, 0.518f, 0.012f);
        spot.SetDiffuseColor(1.0f, 1.0f, 1.0f);
        spot.SetSpecularColor(1.0f, 1.0f, 1.0f);
        spot.SetSpecularStrength(0.003f);
        spot.SetSpecularShininess(32.0f);
        spot.SetConstantAtt(1.0f);
        spot.SetLinearAtt(0.7f);
        spot.SetQuadraticAtt(1.8f);
        spot.SetCuttOff(2.5f);
        spot.SetOuterCuttOff(17.0f);
        spot.SetCastShadows(true);
        spot.SetEnabled(true);
        spot.SetIntensity(300.0f);
        spot_cuadro[0] = ecs.new_entity(spot);
    }
    
    // Spot cuadro 2
    {
        And::SpotLight spot{};
        //spot.SetPosition(33.0f, 20.0f, 0.0f);
        spot.SetPosition(-35.5f, 32.0f, 0.0f);
        spot.SetDirection(0.0f, -1.0f, 0.0f);
        //spot.SetDiffuseColor(0.976f, 0.518f, 0.012f);
        spot.SetDiffuseColor(1.0f, 1.0f, 1.0f);
        spot.SetSpecularColor(1.0f, 1.0f, 1.0f);
        spot.SetSpecularStrength(0.003f);
        spot.SetSpecularShininess(32.0f);
        spot.SetConstantAtt(1.0f);
        spot.SetLinearAtt(0.7f);
        spot.SetQuadraticAtt(1.8f);
        spot.SetCuttOff(2.5f);
        spot.SetOuterCuttOff(17.0f);
        spot.SetCastShadows(true);
        spot.SetEnabled(true);
        spot.SetIntensity(300.0f);
        spot_cuadro[1] = ecs.new_entity(spot);
    }
    
    // Spot cuadro 3
    {
        And::SpotLight spot{};
        //spot.SetPosition(33.0f, 20.0f, 0.0f);
        spot.SetPosition(-35.5f, 32.0f, -distance_cuadros);
        spot.SetDirection(0.0f, -1.0f, 0.0f);
        //spot.SetDiffuseColor(0.976f, 0.518f, 0.012f);
        spot.SetDiffuseColor(1.0f, 1.0f, 1.0f);
        spot.SetSpecularColor(1.0f, 1.0f, 1.0f);
        spot.SetSpecularStrength(0.003f);
        spot.SetSpecularShininess(32.0f);
        spot.SetConstantAtt(1.0f);
        spot.SetLinearAtt(0.7f);
        spot.SetQuadraticAtt(1.8f);
        spot.SetCuttOff(2.5f);
        spot.SetOuterCuttOff(17.0f);
        spot.SetCastShadows(true);
        spot.SetEnabled(true);
        spot.SetIntensity(300.0f);
        spot_cuadro[2] = ecs.new_entity(spot);
    }
    
    // Spot lamp
    {
        And::SpotLight spot{};
        //spot.SetPosition(33.0f, 20.0f, 0.0f);
        spot.SetPosition(-9.5f, 12.5f, 52.0f);
        spot.SetDirection(-1.0f, -0.5f, 0.0f);
        spot.SetDiffuseColor(0.976f, 0.518f, 0.3f);
        spot.SetSpecularColor(1.0f, 1.0f, 1.0f);
        spot.SetSpecularStrength(0.003f);
        spot.SetSpecularShininess(32.0f);
        spot.SetConstantAtt(1.0f);
        spot.SetLinearAtt(0.7f);
        spot.SetQuadraticAtt(1.8f);
        spot.SetCuttOff(2.5f);
        spot.SetOuterCuttOff(30.0f);
        spot.SetCastShadows(true);
        spot.SetEnabled(true);
        spot.SetIntensity(300.0f);
        ecs.new_entity(spot);
    }
    
    // Spot estanteria
    {
        And::SpotLight spot{};
        //spot.SetPosition(33.0f, 20.0f, 0.0f);
        spot.SetPosition(35.0f, 31.0f, 43.0f);
        spot.SetDirection(0.0f, -1.0f, 0.0f);
        spot.SetDiffuseColor(0.976f, 0.518f, 0.3f);
        spot.SetSpecularColor(1.0f, 1.0f, 1.0f);
        spot.SetSpecularStrength(0.003f);
        spot.SetSpecularShininess(32.0f);
        spot.SetConstantAtt(1.0f);
        spot.SetLinearAtt(0.7f);
        spot.SetQuadraticAtt(1.8f);
        spot.SetCuttOff(2.5f);
        spot.SetOuterCuttOff(30.0f);
        spot.SetCastShadows(true);
        spot.SetEnabled(true);
        spot.SetIntensity(300.0f);
        spot_estanteria0 = ecs.new_entity(spot);
    }

    // Spot estanteria 2
    {
        And::SpotLight spot{};
        //spot.SetPosition(33.0f, 20.0f, 0.0f);
        //43.0f - (scale_z * 1.5f)
        spot.SetPosition(35.0f, 31.0f,43.0f - (8.0f * 1.5f));
        spot.SetDirection(0.0f, -1.0f, 0.0f);
        spot.SetDiffuseColor(0.976f, 0.518f, 0.3f);
        spot.SetSpecularColor(1.0f, 1.0f, 1.0f);
        spot.SetSpecularStrength(0.003f);
        spot.SetSpecularShininess(32.0f);
        spot.SetConstantAtt(1.0f);
        spot.SetLinearAtt(0.7f);
        spot.SetQuadraticAtt(1.8f);
        spot.SetCuttOff(2.5f);
        spot.SetOuterCuttOff(30.0f);
        spot.SetCastShadows(true);
        spot.SetEnabled(true);
        spot.SetIntensity(300.0f);
        spot_estanteria1 = ecs.new_entity(spot);
    }

    // Spot estanteria 3
    {
        And::SpotLight spot{};
        //spot.SetPosition(33.0f, 20.0f, 0.0f);
        //43.0f - (scale_z * 1.5f)
        spot.SetPosition(35.0f, 31.0f, 43.0f - (8.0f * 3.0f));
        spot.SetDirection(0.0f, -1.0f, 0.0f);
        spot.SetDiffuseColor(0.976f, 0.518f, 0.3f);
        spot.SetSpecularColor(1.0f, 1.0f, 1.0f);
        spot.SetSpecularStrength(0.003f);
        spot.SetSpecularShininess(32.0f);
        spot.SetConstantAtt(1.0f);
        spot.SetLinearAtt(0.7f);
        spot.SetQuadraticAtt(1.8f);
        spot.SetCuttOff(2.5f);
        spot.SetOuterCuttOff(30.0f);
        spot.SetCastShadows(true);
        spot.SetEnabled(true);
        spot.SetIntensity(300.0f);
        spot_estanteria2 = ecs.new_entity(spot);
    }
    
    // Spot hexagon tiles
    {
        And::SpotLight spot{};
        //spot.SetPosition(33.0f, 20.0f, 0.0f);
        //43.0f - (scale_z * 1.5f)
        spot.SetPosition(35.0f, 31.0f, -39.0f );
        spot.SetDirection(0.0f, -1.0f, 0.0f);
        spot.SetDiffuseColor(1.0f, 1.0f, 1.0f);
        spot.SetSpecularColor(1.0f, 1.0f, 1.0f);
        spot.SetSpecularStrength(0.003f);
        spot.SetSpecularShininess(32.0f);
        spot.SetConstantAtt(1.0f);
        spot.SetLinearAtt(0.7f);
        spot.SetQuadraticAtt(1.8f);
        spot.SetCuttOff(2.5f);
        spot.SetOuterCuttOff(30.0f);
        spot.SetCastShadows(true);
        spot.SetEnabled(true);
        spot.SetIntensity(300.0f);
        //spot_hexagonos = ecs.new_entity(spot);
    }


    // Point hexagonos
    {
        const float intensity = 200.0f;
        And::PointLight point;
        point.SetPosition(25.0f, 17.0f, -39.0f);
        point.SetSpecularColor(1.0f, 1.0f, 1.0f);
        point.SetSpecularShininess(8.0f);
        point.SetSpecularStrength(0.003f);
        point.SetConstantAtt(1.0f);
        point.SetLinearAtt(0.0f);
        point.SetQuadraticAtt(0.0f);
        point.SetEnabled(true);
        point.SetCastShadows(true);
        point.SetDiffuseColor(1.0f, 1.0f, 1.0f);
        point.SetIntensity(intensity);
        point_hexagonos = ecs.new_entity(point);
    }

}

void CreateFurnitures(And::EntityComponentSystem& ecs, And::Entity* parent){
    
    // Mesa 
    {
        And::MaterialComponent mat_com;
        std::shared_ptr<And::Material> mat = std::make_shared<And::Material>();
        std::shared_ptr<And::Texture> tex = And::MakeTexture("demo/textures/bambu/albedo.png");
        std::shared_ptr<And::Texture> normals = And::MakeTexture("demo/textures/bambu/normals.png");
        std::shared_ptr<And::Texture> ao = And::MakeTexture("demo/textures/bambu/ao.png");
        std::shared_ptr<And::Texture> metallic = And::MakeTexture("demo/textures/bambu/metallic.png");
        std::shared_ptr<And::Texture> rou = And::MakeTexture("demo/textures/bambu/roughness.png");
        mat->SetColorTexture(tex);
        mat->SetNormalTexture(normals);
        mat->SetAmbientOclusionTexture(ao);
        mat->SetMetallicTexture(metallic);
        mat->SetRoughnessTexture(rou);
        mat_com.SetMaterial(mat);

        And::MeshComponent MC;
        MC.MeshOBJ = And::Geometry::load("demo/obj/office_desk.obj");
        And::RawMesh raw_mesh_mesa(MC.MeshOBJ->get_vertices(), MC.MeshOBJ->get_indices());
        std::shared_ptr<And::Mesh> mesh = std::make_shared<And::Mesh>(raw_mesh_mesa);
        MC.SetMesh(mesh);

        And::TransformComponent tr;
        tr.SetPosition(33.0f, 1.0f, -15.0f);
        tr.SetRotation(0.0f,PI / 2.0f, 0.0f);
        tr.SetScale(15.0f, 5.0f, 10.0f);
        tr.HasRigidBody(false);
        //tr.SetParent(parent->get_component<And::TransformComponent>());
        ecs.new_entity(mat_com, MC, tr);
    }

    // Guitarra 
    {
        And::MaterialComponent mat_com;
        std::shared_ptr<And::Material> mat = std::make_shared<And::Material>();
        std::shared_ptr<And::Texture> tex = And::MakeTexture("demo/textures/suelo_5/albedo.png");
        std::shared_ptr<And::Texture> normals = And::MakeTexture("demo/textures/suelo_5/normals.png");
        std::shared_ptr<And::Texture> ao = And::MakeTexture("demo/textures/suelo_5/ao.png");
        std::shared_ptr<And::Texture> metallic = And::MakeTexture("demo/textures/suelo_5/metallic.png");
        std::shared_ptr<And::Texture> rou = And::MakeTexture("demo/textures/suelo_5/roughness.png");
        mat->SetColorTexture(tex);
        mat->SetNormalTexture(normals);
        mat->SetAmbientOclusionTexture(ao);
        mat->SetMetallicTexture(metallic);
        mat->SetRoughnessTexture(rou);
        mat_com.SetMaterial(mat);

        And::MeshComponent MC;
        MC.MeshOBJ = And::Geometry::load("demo/obj/guitar.obj");
        And::RawMesh raw_mesh_mesa(MC.MeshOBJ->get_vertices(), MC.MeshOBJ->get_indices());
        std::shared_ptr<And::Mesh> mesh = std::make_shared<And::Mesh>(raw_mesh_mesa);
        MC.SetMesh(mesh);

        And::TransformComponent tr;
        tr.SetPosition(-34.0f, 1.0f, 50.5f);
        tr.SetRotation(PI / -8.0f, PI, 0.0f);
        tr.SetScale(12.5f, 12.5f, 12.5f);
        tr.HasRigidBody(false);
        //tr.SetParent(parent->get_component<And::TransformComponent>());
        ecs.new_entity(mat_com, MC, tr);
        //ecs.new_entity(mat_com, MC, tr)->get_component<And::TransformComponent>()->SetParent(parent->get_component<And::TransformComponent>());
    }
    
    const float distance_cuadros = 20.0f;
    // Poster 1
    {
        And::MaterialComponent mat_com;
        std::shared_ptr<And::Material> mat = std::make_shared<And::Material>();
        std::shared_ptr<And::Texture> tex = And::MakeTexture("demo/textures/poster/albedo_esat.png");
        std::shared_ptr<And::Texture> normals = And::MakeTexture("demo/textures/poster/normals_raw.png");
        std::shared_ptr<And::Texture> ao = And::MakeTexture("demo/textures/poster/ao_raw.png");
        std::shared_ptr<And::Texture> metallic = And::MakeTexture("demo/textures/poster/metallic.png");
        std::shared_ptr<And::Texture> rou = And::MakeTexture("demo/textures/poster/roughness.png");
        mat->SetColorTexture(tex);
        mat->SetNormalTexture(normals);
        mat->SetAmbientOclusionTexture(ao);
        mat->SetMetallicTexture(metallic);
        mat->SetRoughnessTexture(rou);
        mat_com.SetMaterial(mat);

        And::MeshComponent MC;
        MC.MeshOBJ = And::Geometry::load("cube.obj");
        And::RawMesh raw_mesh_mesa(MC.MeshOBJ->get_vertices(), MC.MeshOBJ->get_indices());
        std::shared_ptr<And::Mesh> mesh = std::make_shared<And::Mesh>(raw_mesh_mesa);
        MC.SetMesh(mesh);

        And::TransformComponent tr;
        tr.SetPosition(-36.5f, 15.0f, distance_cuadros);
        tr.SetRotation(0.0f, 0.0f, 0.0f);
        tr.SetScale(0.3f, 10.0f, 10.0f);
        tr.HasRigidBody(false);
        //tr.SetParent(parent->get_component<And::TransformComponent>());
        ecs.new_entity(mat_com, MC, tr);
        //ecs.new_entity(mat_com, MC, tr)->get_component<And::TransformComponent>()->SetParent(parent->get_component<And::TransformComponent>());
    }
    
    // Poster 2
    {
        And::MaterialComponent mat_com;
        std::shared_ptr<And::Material> mat = std::make_shared<And::Material>();
        std::shared_ptr<And::Texture> tex = And::MakeTexture("demo/textures/poster/albedo_ochy_raw.png");
        std::shared_ptr<And::Texture> normals = And::MakeTexture("demo/textures/poster/normals_raw.png");
        std::shared_ptr<And::Texture> ao = And::MakeTexture("demo/textures/poster/ao_raw.png");
        std::shared_ptr<And::Texture> metallic = And::MakeTexture("demo/textures/poster/metallic.png");
        std::shared_ptr<And::Texture> rou = And::MakeTexture("demo/textures/poster/roughness.png");
        mat->SetColorTexture(tex);
        mat->SetNormalTexture(normals);
        mat->SetAmbientOclusionTexture(ao);
        mat->SetMetallicTexture(metallic);
        mat->SetRoughnessTexture(rou);
        mat_com.SetMaterial(mat);

        And::MeshComponent MC;
        MC.MeshOBJ = And::Geometry::load("cube.obj");
        And::RawMesh raw_mesh_mesa(MC.MeshOBJ->get_vertices(), MC.MeshOBJ->get_indices());
        std::shared_ptr<And::Mesh> mesh = std::make_shared<And::Mesh>(raw_mesh_mesa);
        MC.SetMesh(mesh);

        And::TransformComponent tr;
        tr.SetPosition(-36.5f, 15.0f, 0.0f);
        tr.SetRotation(0.0f, 0.0f, 0.0f);
        tr.SetScale(0.3f, 10.0f, 10.0f);
        tr.HasRigidBody(false);
        //tr.SetParent(parent->get_component<And::TransformComponent>());
        ecs.new_entity(mat_com, MC, tr);
        //ecs.new_entity(mat_com, MC, tr)->get_component<And::TransformComponent>()->SetParent(parent->get_component<And::TransformComponent>());
    }
    
    // Poster 3
    {
        And::MaterialComponent mat_com;
        std::shared_ptr<And::Material> mat = std::make_shared<And::Material>();
        std::shared_ptr<And::Texture> tex = And::MakeTexture("demo/textures/poster/albedo_xema_raw.png");
        std::shared_ptr<And::Texture> normals = And::MakeTexture("demo/textures/poster/normals_raw.png");
        std::shared_ptr<And::Texture> ao = And::MakeTexture("demo/textures/poster/ao_raw.png");
        std::shared_ptr<And::Texture> metallic = And::MakeTexture("demo/textures/poster/metallic.png");
        std::shared_ptr<And::Texture> rou = And::MakeTexture("demo/textures/poster/roughness.png");
        mat->SetColorTexture(tex);
        mat->SetNormalTexture(normals);
        mat->SetAmbientOclusionTexture(ao);
        mat->SetMetallicTexture(metallic);
        mat->SetRoughnessTexture(rou);
        mat_com.SetMaterial(mat);

        And::MeshComponent MC;
        MC.MeshOBJ = And::Geometry::load("cube.obj");
        And::RawMesh raw_mesh_mesa(MC.MeshOBJ->get_vertices(), MC.MeshOBJ->get_indices());
        std::shared_ptr<And::Mesh> mesh = std::make_shared<And::Mesh>(raw_mesh_mesa);
        MC.SetMesh(mesh);

        And::TransformComponent tr;
        tr.SetPosition(-36.5f, 15.0f, -distance_cuadros);
        tr.SetRotation(0.0f, 0.0f, 0.0f);
        tr.SetScale(0.3f, 10.0f, 10.0f);
        tr.HasRigidBody(false);
        //tr.SetParent(parent->get_component<And::TransformComponent>());
        ecs.new_entity(mat_com, MC, tr);
        //ecs.new_entity(mat_com, MC, tr)->get_component<And::TransformComponent>()->SetParent(parent->get_component<And::TransformComponent>());
    }

    // TV
    {
        And::MaterialComponent mat_com;
        std::shared_ptr<And::Material> mat = std::make_shared<And::Material>();
        std::shared_ptr<And::Texture> tex = And::MakeTexture("demo/textures/titanio/albedo.png");
        std::shared_ptr<And::Texture> normals = And::MakeTexture("demo/textures/titanio/normals.png");
        std::shared_ptr<And::Texture> ao = And::MakeTexture("demo/textures/titanio/ao.png");
        std::shared_ptr<And::Texture> metallic = And::MakeTexture("demo/textures/titanio/metallic.png");
        std::shared_ptr<And::Texture> rou = And::MakeTexture("demo/textures/titanio/roughness.png");
        mat->SetColorTexture(tex);
        mat->SetNormalTexture(normals);
        mat->SetAmbientOclusionTexture(ao);
        mat->SetMetallicTexture(metallic);
        mat->SetRoughnessTexture(rou);
        mat_com.SetMaterial(mat);

        And::MeshComponent MC;
        MC.MeshOBJ = And::Geometry::load("demo/obj/tv.obj");
        And::RawMesh raw_mesh_mesa(MC.MeshOBJ->get_vertices(), MC.MeshOBJ->get_indices());
        std::shared_ptr<And::Mesh> mesh = std::make_shared<And::Mesh>(raw_mesh_mesa);
        MC.SetMesh(mesh);

        And::TransformComponent tr;
        tr.SetPosition(33.0f, 4.75f, -15.0f);
        tr.SetRotation(0.0f, PI * -0.5f, 0.0f);
        tr.SetScale(25.0f, 20.0f, 20.0f);
        tr.HasRigidBody(false);
        //tr.SetParent(parent->get_component<And::TransformComponent>());
        ecs.new_entity(mat_com, MC, tr);
        //ecs.new_entity(mat_com, MC, tr)->get_component<And::TransformComponent>()->SetParent(parent->get_component<And::TransformComponent>());
    }

    // Sofa 1
    {
        And::MaterialComponent mat_com;
        std::shared_ptr<And::Material> mat = std::make_shared<And::Material>();
        std::shared_ptr<And::Texture> tex = And::MakeTexture("demo/textures/leather/albedo.png");
        std::shared_ptr<And::Texture> normals = And::MakeTexture("demo/textures/leather/normals.png");
        std::shared_ptr<And::Texture> ao = And::MakeTexture("demo/textures/leather/ao.png");
        std::shared_ptr<And::Texture> metallic = And::MakeTexture("demo/textures/leather/metallic.png");
        std::shared_ptr<And::Texture> rou = And::MakeTexture("demo/textures/leather/roughness.png");
        mat->SetColorTexture(tex);
        mat->SetNormalTexture(normals);
        mat->SetAmbientOclusionTexture(ao);
        mat->SetMetallicTexture(metallic);
        mat->SetRoughnessTexture(rou);
        mat_com.SetMaterial(mat);

        And::MeshComponent MC;
        MC.MeshOBJ = And::Geometry::load("demo/obj/furniture/sofa.obj");
        And::RawMesh raw_mesh_mesa(MC.MeshOBJ->get_vertices(), MC.MeshOBJ->get_indices());
        std::shared_ptr<And::Mesh> mesh = std::make_shared<And::Mesh>(raw_mesh_mesa);
        MC.SetMesh(mesh);

        And::TransformComponent tr;
        tr.SetPosition(-10.0f, 0.0f, -15.0f);
        tr.SetRotation(0.0f,PI * -0.5f, 0.0f);
        tr.SetScale(0.8f, 0.8f, 0.8f);
        tr.HasRigidBody(false);
        //tr.SetParent(parent->get_component<And::TransformComponent>());
        ecs.new_entity(mat_com, MC, tr);
        //ecs.new_entity(mat_com, MC, tr)->get_component<And::TransformComponent>()->SetParent(parent->get_component<And::TransformComponent>());
    }
    
    // Display table
    {
        And::MaterialComponent mat_com;
        std::shared_ptr<And::Material> mat = std::make_shared<And::Material>();
        std::shared_ptr<And::Texture> tex = And::MakeTexture("demo/textures/bambu/albedo.png");
        std::shared_ptr<And::Texture> normals = And::MakeTexture("demo/textures/bambu/normals.png");
        std::shared_ptr<And::Texture> ao = And::MakeTexture("demo/textures/bambu/ao.png");
        std::shared_ptr<And::Texture> metallic = And::MakeTexture("demo/textures/bambu/metallic.png");
        std::shared_ptr<And::Texture> rou = And::MakeTexture("demo/textures/bambu/roughness.png");
        mat->SetColorTexture(tex);
        mat->SetNormalTexture(normals);
        mat->SetAmbientOclusionTexture(ao);
        mat->SetMetallicTexture(metallic);
        mat->SetRoughnessTexture(rou);
        mat_com.SetMaterial(mat);

        And::MeshComponent MC;
        MC.MeshOBJ = And::Geometry::load("demo/obj/furniture/display_table.obj");
        And::RawMesh raw_mesh_mesa(MC.MeshOBJ->get_vertices(), MC.MeshOBJ->get_indices());
        std::shared_ptr<And::Mesh> mesh = std::make_shared<And::Mesh>(raw_mesh_mesa);
        MC.SetMesh(mesh);

        And::TransformComponent tr;
        tr.SetPosition(10.0f, 0.0f, -15.0f);
        tr.SetRotation(0.0f,PI * -0.5f, 0.0f);
        tr.SetScale(12.0f, 6.0f, 10.0f);
        tr.HasRigidBody(false);
        //tr.SetParent(parent->get_component<And::TransformComponent>());
        ecs.new_entity(mat_com, MC, tr);
        //ecs.new_entity(mat_com, MC, tr)->get_component<And::TransformComponent>()->SetParent(parent->get_component<And::TransformComponent>());
    }

    // office desk
    {
        And::MaterialComponent mat_com;
        std::shared_ptr<And::Material> mat = std::make_shared<And::Material>();
        std::shared_ptr<And::Texture> tex = And::MakeTexture("demo/textures/wood/albedo.png");
        std::shared_ptr<And::Texture> normals = And::MakeTexture("demo/textures/wood/normals.png");
        std::shared_ptr<And::Texture> ao = And::MakeTexture("demo/textures/wood/ao.png");
        std::shared_ptr<And::Texture> metallic = And::MakeTexture("demo/textures/wood/metallic.png");
        std::shared_ptr<And::Texture> rou = And::MakeTexture("demo/textures/wood/roughness.png");
        mat->SetColorTexture(tex);
        mat->SetNormalTexture(normals);
        mat->SetAmbientOclusionTexture(ao);
        mat->SetMetallicTexture(metallic);
        mat->SetRoughnessTexture(rou);
        mat_com.SetMaterial(mat);

        And::MeshComponent MC;
        MC.MeshOBJ = And::Geometry::load("demo/obj/office/desk.obj");
        And::RawMesh raw_mesh_mesa(MC.MeshOBJ->get_vertices(), MC.MeshOBJ->get_indices());
        std::shared_ptr<And::Mesh> mesh = std::make_shared<And::Mesh>(raw_mesh_mesa);
        MC.SetMesh(mesh);

        And::TransformComponent tr;
        tr.SetPosition(-17.0f, 0.0f, 50.5f);
        tr.SetRotation(0.0f, 0.0f, 0.0f);
        tr.SetScale(10.0f, 10.0f, 10.0f);
        tr.HasRigidBody(false);
        //tr.SetParent(parent->get_component<And::TransformComponent>());
        ecs.new_entity(mat_com, MC, tr);
        //ecs.new_entity(mat_com, MC, tr)->get_component<And::TransformComponent>()->SetParent(parent->get_component<And::TransformComponent>());
    }

    // office chair
    {
        And::MaterialComponent mat_com;
        std::shared_ptr<And::Material> mat = std::make_shared<And::Material>();
        std::shared_ptr<And::Texture> tex = And::MakeTexture("demo/textures/grip/albedo.png");
        std::shared_ptr<And::Texture> normals = And::MakeTexture("demo/textures/grip/normals.png");
        std::shared_ptr<And::Texture> ao = And::MakeTexture("demo/textures/grip/ao.png");
        std::shared_ptr<And::Texture> metallic = And::MakeTexture("demo/textures/grip/metallic.png");
        std::shared_ptr<And::Texture> rou = And::MakeTexture("demo/textures/grip/roughness.png");
        mat->SetColorTexture(tex);
        mat->SetNormalTexture(normals);
        mat->SetAmbientOclusionTexture(ao);
        mat->SetMetallicTexture(metallic);
        mat->SetRoughnessTexture(rou);
        mat_com.SetMaterial(mat);

        And::MeshComponent MC;
        MC.MeshOBJ = And::Geometry::load("demo/obj/office/chair.obj");
        And::RawMesh raw_mesh_mesa(MC.MeshOBJ->get_vertices(), MC.MeshOBJ->get_indices());
        std::shared_ptr<And::Mesh> mesh = std::make_shared<And::Mesh>(raw_mesh_mesa);
        MC.SetMesh(mesh);

        And::TransformComponent tr;
        tr.SetPosition(-17.0f, 0.0f, 45.0f);
        tr.SetRotation(0.0f, 0.0f, 0.0f);
        tr.SetScale(12.0f, 12.0f, 12.0f);
        tr.HasRigidBody(false);
        //tr.SetParent(parent->get_component<And::TransformComponent>());
        ecs.new_entity(mat_com, MC, tr);
        //ecs.new_entity(mat_com, MC, tr)->get_component<And::TransformComponent>()->SetParent(parent->get_component<And::TransformComponent>());
    }

    // office lamp
    {
        And::MaterialComponent mat_com;
        std::shared_ptr<And::Material> mat = std::make_shared<And::Material>();
        std::shared_ptr<And::Texture> tex = And::MakeTexture("demo/textures/aluminio/albedo.png");
        std::shared_ptr<And::Texture> normals = And::MakeTexture("demo/textures/aluminio/normals.png");
        std::shared_ptr<And::Texture> ao = And::MakeTexture("demo/textures/aluminio/ao.png");
        std::shared_ptr<And::Texture> metallic = And::MakeTexture("demo/textures/aluminio/metallic.png");
        std::shared_ptr<And::Texture> rou = And::MakeTexture("demo/textures/aluminio/roughness.png");
        mat->SetColorTexture(tex);
        mat->SetNormalTexture(normals);
        mat->SetAmbientOclusionTexture(ao);
        mat->SetMetallicTexture(metallic);
        mat->SetRoughnessTexture(rou);
        mat_com.SetMaterial(mat);

        And::MeshComponent MC;
        MC.MeshOBJ = And::Geometry::load("demo/obj/office/lamp.obj");
        And::RawMesh raw_mesh_mesa(MC.MeshOBJ->get_vertices(), MC.MeshOBJ->get_indices());
        std::shared_ptr<And::Mesh> mesh = std::make_shared<And::Mesh>(raw_mesh_mesa);
        MC.SetMesh(mesh);

        And::TransformComponent tr;
        tr.SetPosition(-9.5f, 9.1f, 52.0f);
        tr.SetRotation(0.0f, PI * -0.5f, 0.0f);
        tr.SetScale(12.0f, 12.0f, 12.0f);
        tr.HasRigidBody(false);
        //tr.SetParent(parent->get_component<And::TransformComponent>());
        ecs.new_entity(mat_com, MC, tr);
        //ecs.new_entity(mat_com, MC, tr)->get_component<And::TransformComponent>()->SetParent(parent->get_component<And::TransformComponent>());
    }

    // office calendar
    {
        And::MaterialComponent mat_com;
        std::shared_ptr<And::Material> mat = std::make_shared<And::Material>();
        std::shared_ptr<And::Texture> tex = And::MakeTexture("demo/textures/paper/albedo.png");
        std::shared_ptr<And::Texture> normals = And::MakeTexture("demo/textures/paper/normals.png");
        std::shared_ptr<And::Texture> ao = And::MakeTexture("demo/textures/paper/ao.png");
        std::shared_ptr<And::Texture> metallic = And::MakeTexture("demo/textures/paper/metallic.png");
        std::shared_ptr<And::Texture> rou = And::MakeTexture("demo/textures/paper/roughness.png");
        mat->SetColorTexture(tex);
        mat->SetNormalTexture(normals);
        mat->SetAmbientOclusionTexture(ao);
        mat->SetMetallicTexture(metallic);
        mat->SetRoughnessTexture(rou);
        mat_com.SetMaterial(mat);

        And::MeshComponent MC;
        MC.MeshOBJ = And::Geometry::load("demo/obj/office/calendar.obj");
        And::RawMesh raw_mesh_mesa(MC.MeshOBJ->get_vertices(), MC.MeshOBJ->get_indices());
        std::shared_ptr<And::Mesh> mesh = std::make_shared<And::Mesh>(raw_mesh_mesa);
        MC.SetMesh(mesh);

        And::TransformComponent tr;
        tr.SetPosition(-25.0f, 9.1f, 52.0f);
        tr.SetRotation(0.0f, PI * 0.75f, 0.0f);
        tr.SetScale(12.0f, 12.0f, 12.0f);
        tr.HasRigidBody(false);
        //tr.SetParent(parent->get_component<And::TransformComponent>());
        ecs.new_entity(mat_com, MC, tr);
        //ecs.new_entity(mat_com, MC, tr)->get_component<And::TransformComponent>()->SetParent(parent->get_component<And::TransformComponent>());
    }

    // office tubos
    {
        And::MaterialComponent mat_com;
        std::shared_ptr<And::Material> mat = std::make_shared<And::Material>();
        std::shared_ptr<And::Texture> tex = And::MakeTexture("demo/textures/metal_dented/albedo.png");
        std::shared_ptr<And::Texture> normals = And::MakeTexture("demo/textures/metal_dented/normals.png");
        std::shared_ptr<And::Texture> ao = And::MakeTexture("demo/textures/metal_dented/ao.png");
        std::shared_ptr<And::Texture> metallic = And::MakeTexture("demo/textures/metal_dented/metallic.png");
        std::shared_ptr<And::Texture> rou = And::MakeTexture("demo/textures/metal_dented/roughness.png");
        mat->SetColorTexture(tex);
        mat->SetNormalTexture(normals);
        mat->SetAmbientOclusionTexture(ao);
        mat->SetMetallicTexture(metallic);
        mat->SetRoughnessTexture(rou);
        mat_com.SetMaterial(mat);

        And::MeshComponent MC;
        MC.MeshOBJ = And::Geometry::load("demo/obj/office/tubos.obj");
        And::RawMesh raw_mesh_mesa(MC.MeshOBJ->get_vertices(), MC.MeshOBJ->get_indices());
        std::shared_ptr<And::Mesh> mesh = std::make_shared<And::Mesh>(raw_mesh_mesa);
        MC.SetMesh(mesh);

        And::TransformComponent tr;
        tr.SetPosition(-21.0f, 9.1f, 51.0f);
        tr.SetRotation(0.0f, PI * 0.75f, 0.0f);
        tr.SetScale(14.0f, 14.0f, 14.0f);
        tr.HasRigidBody(false);
        //tr.SetParent(parent->get_component<And::TransformComponent>());
        ecs.new_entity(mat_com, MC, tr);
        //ecs.new_entity(mat_com, MC, tr)->get_component<And::TransformComponent>()->SetParent(parent->get_component<And::TransformComponent>());
    }

    // Living room keyboard
    {
        And::MaterialComponent mat_com;
        std::shared_ptr<And::Material> mat = std::make_shared<And::Material>();
        std::shared_ptr<And::Texture> tex = And::MakeTexture("demo/textures/worn_metal/albedo.png");
        std::shared_ptr<And::Texture> normals = And::MakeTexture("demo/textures/worn_metal/normals.png");
        std::shared_ptr<And::Texture> ao = And::MakeTexture("demo/textures/worn_metal/ao.png");
        std::shared_ptr<And::Texture> metallic = And::MakeTexture("demo/textures/worn_metal/metallic.png");
        std::shared_ptr<And::Texture> rou = And::MakeTexture("demo/textures/worn_metal/roughness.png");
        mat->SetColorTexture(tex);
        mat->SetNormalTexture(normals);
        mat->SetAmbientOclusionTexture(ao);
        mat->SetMetallicTexture(metallic);
        mat->SetRoughnessTexture(rou);
        mat_com.SetMaterial(mat);

        And::MeshComponent MC;
        MC.MeshOBJ = And::Geometry::load("demo/obj/keyboard.obj");
        And::RawMesh raw_mesh_mesa(MC.MeshOBJ->get_vertices(), MC.MeshOBJ->get_indices());
        std::shared_ptr<And::Mesh> mesh = std::make_shared<And::Mesh>(raw_mesh_mesa);
        MC.SetMesh(mesh);

        And::TransformComponent tr;
        tr.SetPosition(9.0f, 5.0f, -16.0f);
        tr.SetRotation(0.0f, PI * 1.70f, 0.0f);
        tr.SetScale(16.0f, 16.0f, 16.0f);
        tr.HasRigidBody(false);
        //tr.SetParent(parent->get_component<And::TransformComponent>());
        ecs.new_entity(mat_com, MC, tr);
        //ecs.new_entity(mat_com, MC, tr)->get_component<And::TransformComponent>()->SetParent(parent->get_component<And::TransformComponent>());
    }
    
    // Living room mouse 
    {
        And::MaterialComponent mat_com;
        std::shared_ptr<And::Material> mat = std::make_shared<And::Material>();
        std::shared_ptr<And::Texture> tex = And::MakeTexture("demo/textures/aluminio/albedo.png");
        std::shared_ptr<And::Texture> normals = And::MakeTexture("demo/textures/aluminio/normals.png");
        std::shared_ptr<And::Texture> ao = And::MakeTexture("demo/textures/aluminio/ao.png");
        std::shared_ptr<And::Texture> metallic = And::MakeTexture("demo/textures/aluminio/metallic.png");
        std::shared_ptr<And::Texture> rou = And::MakeTexture("demo/textures/aluminio/roughness.png");
        mat->SetColorTexture(tex);
        mat->SetNormalTexture(normals);
        mat->SetAmbientOclusionTexture(ao);
        mat->SetMetallicTexture(metallic);
        mat->SetRoughnessTexture(rou);
        mat_com.SetMaterial(mat);

        And::MeshComponent MC;
        MC.MeshOBJ = And::Geometry::load("demo/obj/mouse.obj");
        And::RawMesh raw_mesh_mesa(MC.MeshOBJ->get_vertices(), MC.MeshOBJ->get_indices());
        std::shared_ptr<And::Mesh> mesh = std::make_shared<And::Mesh>(raw_mesh_mesa);
        MC.SetMesh(mesh);

        And::TransformComponent tr;
        tr.SetPosition(9.0f, 5.0f, -10.0f);
        tr.SetRotation(0.0f, PI * -0.5f, 0.0f);
        tr.SetScale(16.0f, 16.0f, 16.0f);
        tr.HasRigidBody(false);
        //tr.SetParent(parent->get_component<And::TransformComponent>());
        ecs.new_entity(mat_com, MC, tr);
        //ecs.new_entity(mat_com, MC, tr)->get_component<And::TransformComponent>()->SetParent(parent->get_component<And::TransformComponent>());
    }

    // Living room taza
    {
        And::MaterialComponent mat_com;
        std::shared_ptr<And::Material> mat = std::make_shared<And::Material>();
        std::shared_ptr<And::Texture> tex = And::MakeTexture("demo/textures/iron/albedo.png");
        std::shared_ptr<And::Texture> normals = And::MakeTexture("demo/textures/iron/normals.png");
        std::shared_ptr<And::Texture> ao = And::MakeTexture("demo/textures/iron/ao.png");
        std::shared_ptr<And::Texture> metallic = And::MakeTexture("demo/textures/iron/metallic.png");
        std::shared_ptr<And::Texture> rou = And::MakeTexture("demo/textures/iron/roughness.png");
        mat->SetColorTexture(tex);
        mat->SetNormalTexture(normals);
        mat->SetAmbientOclusionTexture(ao);
        mat->SetMetallicTexture(metallic);
        mat->SetRoughnessTexture(rou);
        mat_com.SetMaterial(mat);

        And::MeshComponent MC;
        MC.MeshOBJ = And::Geometry::load("demo/obj/taza.obj");
        And::RawMesh raw_mesh_mesa(MC.MeshOBJ->get_vertices(), MC.MeshOBJ->get_indices());
        std::shared_ptr<And::Mesh> mesh = std::make_shared<And::Mesh>(raw_mesh_mesa);
        MC.SetMesh(mesh);

        And::TransformComponent tr;
        tr.SetPosition(7.0f, 5.0f, -10.0f);
        tr.SetRotation(0.0f, 0.0f, 0.0f);
        tr.SetScale(16.0f, 16.0f, 16.0f);
        tr.HasRigidBody(false);
        //tr.SetParent(parent->get_component<And::TransformComponent>());
        ecs.new_entity(mat_com, MC, tr);
        //ecs.new_entity(mat_com, MC, tr)->get_component<And::TransformComponent>()->SetParent(parent->get_component<And::TransformComponent>());
    }

    // Living room cafe
    {
        And::MaterialComponent mat_com;
        std::shared_ptr<And::Material> mat = std::make_shared<And::Material>();
        std::shared_ptr<And::Texture> tex = And::MakeTexture("demo/textures/shiny_metal/albedo_white.png");
        std::shared_ptr<And::Texture> normals = And::MakeTexture("demo/textures/shiny_metal/normals.png");
        std::shared_ptr<And::Texture> ao = And::MakeTexture("demo/textures/shiny_metal/ao.png");
        std::shared_ptr<And::Texture> metallic = And::MakeTexture("demo/textures/shiny_metal/metallic.png");
        std::shared_ptr<And::Texture> rou = And::MakeTexture("demo/textures/shiny_metal/roughness.png");
        mat->SetColorTexture(tex);
        mat->SetNormalTexture(normals);
        mat->SetAmbientOclusionTexture(ao);
        mat->SetMetallicTexture(metallic);
        mat->SetRoughnessTexture(rou);
        mat_com.SetMaterial(mat);

        And::MeshComponent MC;
        MC.MeshOBJ = And::Geometry::load("sphere.obj");
        And::RawMesh raw_mesh_mesa(MC.MeshOBJ->get_vertices(), MC.MeshOBJ->get_indices());
        std::shared_ptr<And::Mesh> mesh = std::make_shared<And::Mesh>(raw_mesh_mesa);
        MC.SetMesh(mesh);

        And::TransformComponent tr;
        tr.SetPosition(7.0f, 6.2f, -10.0f);
        tr.SetRotation(0.0f, 0.0f, 0.0f);
        tr.SetScale(0.7f, 0.2f, 0.7f);
        tr.HasRigidBody(false);
        //tr.SetParent(parent->get_component<And::TransformComponent>());
        ecs.new_entity(mat_com, MC, tr);
        //ecs.new_entity(mat_com, MC, tr)->get_component<And::TransformComponent>()->SetParent(parent->get_component<And::TransformComponent>());
    }

    // Estanteria pared
    {
        And::MaterialComponent mat_com;
        std::shared_ptr<And::Material> mat = std::make_shared<And::Material>();
        std::shared_ptr<And::Texture> tex = And::MakeTexture("demo/textures/bambu/albedo.png");
        std::shared_ptr<And::Texture> normals = And::MakeTexture("demo/textures/bambu/normals.png");
        std::shared_ptr<And::Texture> ao = And::MakeTexture("demo/textures/bambu/ao.png");
        std::shared_ptr<And::Texture> metallic = And::MakeTexture("demo/textures/bambu/metallic.png");
        std::shared_ptr<And::Texture> rou = And::MakeTexture("demo/textures/bambu/roughness.png");
        mat->SetColorTexture(tex);
        mat->SetNormalTexture(normals);
        mat->SetAmbientOclusionTexture(ao);
        mat->SetMetallicTexture(metallic);
        mat->SetRoughnessTexture(rou);
        mat_com.SetMaterial(mat);

        And::MeshComponent MC;
        MC.MeshOBJ = And::Geometry::load("cube.obj");
        And::RawMesh raw_mesh_mesa(MC.MeshOBJ->get_vertices(), MC.MeshOBJ->get_indices());
        std::shared_ptr<And::Mesh> mesh = std::make_shared<And::Mesh>(raw_mesh_mesa);
        MC.SetMesh(mesh);

        const float scale_x = 4.0f;
        const float scale_z = 8.0f;
        const float pos_y = 20.0f;
        const float diference_y = 3.5f;

        And::TransformComponent tr;
        tr.SetPosition(35.0f, pos_y, 43.0f);
        tr.SetRotation(0.0f, 0.0f, 0.0f);
        tr.SetScale(scale_x, 0.25f, scale_z);
        tr.HasRigidBody(false);
        //tr.SetParent(parent->get_component<And::TransformComponent>());
        ecs.new_entity(mat_com, MC, tr);
        //ecs.new_entity(mat_com, MC, tr)->get_component<And::TransformComponent>()->SetParent(parent->get_component<And::TransformComponent>());

        tr.SetPosition(35.0f, pos_y - diference_y, 43.0f - (scale_z * 1.5f));
        ecs.new_entity(mat_com, MC, tr);
        
        tr.SetPosition(35.0f, pos_y - (diference_y * 2.0f), 43.0f - (scale_z * 3.0f));
        ecs.new_entity(mat_com, MC, tr);
    }

    // Estanteria sphere
    {
        And::MaterialComponent mat_com;
        std::shared_ptr<And::Material> mat = std::make_shared<And::Material>();
        std::shared_ptr<And::Texture> tex = And::MakeTexture("demo/textures/metal_circulos/albedo.png");
        std::shared_ptr<And::Texture> normals = And::MakeTexture("demo/textures/metal_circulos/normals.png");
        std::shared_ptr<And::Texture> ao = And::MakeTexture("demo/textures/metal_circulos/ao.png");
        std::shared_ptr<And::Texture> metallic = And::MakeTexture("demo/textures/metal_circulos/metallic.png");
        std::shared_ptr<And::Texture> rou = And::MakeTexture("demo/textures/metal_circulos/roughness.png");
        mat->SetColorTexture(tex);
        mat->SetNormalTexture(normals);
        mat->SetAmbientOclusionTexture(ao);
        mat->SetMetallicTexture(metallic);
        mat->SetRoughnessTexture(rou);
        mat_com.SetMaterial(mat);

        And::MeshComponent MC;
        MC.MeshOBJ = And::Geometry::load("sphere.obj");
        And::RawMesh raw_mesh_mesa(MC.MeshOBJ->get_vertices(), MC.MeshOBJ->get_indices());
        std::shared_ptr<And::Mesh> mesh = std::make_shared<And::Mesh>(raw_mesh_mesa);
        MC.SetMesh(mesh);

        const float scale_x = 4.0f;
        const float scale_z = 8.0f;
        const float pos_y = 22.0f;
        const float diference_y = 3.5f;

        And::TransformComponent tr;
        tr.SetPosition(35.0f, pos_y, 43.0f);
        tr.SetRotation(0.0f, PI * -0.5f, 0.0f);
        tr.SetScale(2.0f, 2.0f, 2.0f);
        tr.HasRigidBody(false);
        //tr.SetParent(parent->get_component<And::TransformComponent>());
        spheres_disco[0] = ecs.new_entity(mat_com, MC, tr);

        tr.SetPosition(35.0f, pos_y - diference_y, 43.0f - (8.0f * 1.5f));
        spheres_disco[1] = ecs.new_entity(mat_com, MC, tr);
        
        tr.SetPosition(35.0f, pos_y - (diference_y * 2.0f), 43.0f - (8.0f * 3.0f));
        spheres_disco[2] = ecs.new_entity(mat_com, MC, tr);


    }

    // Living room hexagon tiles
    {
        And::MaterialComponent mat_com;
        std::shared_ptr<And::Material> mat = std::make_shared<And::Material>();
        std::shared_ptr<And::Texture> tex = And::MakeTexture("demo/textures/fibra_carbono/albedo.png");
        std::shared_ptr<And::Texture> normals = And::MakeTexture("demo/textures/fibra_carbono/normals.png");
        std::shared_ptr<And::Texture> ao = And::MakeTexture("demo/textures/fibra_carbono/ao.png");
        std::shared_ptr<And::Texture> metallic = And::MakeTexture("demo/textures/fibra_carbono/metallic.png");
        std::shared_ptr<And::Texture> rou = And::MakeTexture("demo/textures/fibra_carbono/roughness.png");
        mat->SetColorTexture(tex);
        mat->SetNormalTexture(normals);
        mat->SetAmbientOclusionTexture(ao);
        mat->SetMetallicTexture(metallic);
        mat->SetRoughnessTexture(rou);
        mat_com.SetMaterial(mat);

        And::MeshComponent MC;
        MC.MeshOBJ = And::Geometry::load("demo/obj/furniture/hexagon_tile.obj");
        And::RawMesh raw_mesh_mesa(MC.MeshOBJ->get_vertices(), MC.MeshOBJ->get_indices());
        std::shared_ptr<And::Mesh> mesh = std::make_shared<And::Mesh>(raw_mesh_mesa);
        MC.SetMesh(mesh);

        And::TransformComponent tr;
        tr.SetPosition(37.0f, 19.0f, -38.0f);
        tr.SetRotation(0.0f, PI * 0.5f, PI * 0.5f);
        tr.SetScale(10.0f, 10.0f, 10.0f);
        tr.HasRigidBody(false);
        //tr.SetParent(parent->get_component<And::TransformComponent>());
        //ecs.new_entity(mat_com, MC, tr);

        const int tile_num = 7;
        const int tile_files = 2;

        float offset_y = 1.3f;
        float offset_z = 0.0f;

        float tmp = 0.0f;

        float index = 0.2f;

        for (int j = -tile_files; j <= tile_files; j++) {
            for (int i = 0; i < tile_num; i++) {
                tr.SetPosition(37.0f, (14.0f - (2.1f * j)) + offset_y, ( -48.0f + (2.4f * i)) + std::abs(j) + (index * std::abs(j)));
                ecs.new_entity(mat_com, MC, tr);

                tmp += 0.1f;
                //index += 0.2f;
            }
            //index = 0.0f;
            tmp = 0.0f;
            offset_y += 0.1f;
            offset_z += 0.9f;
        }
    }
}


int main(int argc, char** argv){

    And::Engine e;
    And::TaskSystem ts;

    And::WorkerCreationInfo workerCreationInfo;
    workerCreationInfo.Name = "Test";
    workerCreationInfo.Function = And::GetGenericWorkerFunction();
    workerCreationInfo.UserData = nullptr;

    ts.AddWorker(workerCreationInfo);

    std::shared_ptr<And::Window> window = And::Window::make(e, 1920, 1080, "Andromeda Engine", And::EGraphicsApiType::OpenGL);
    //window->set_vsync(true);
    std::shared_ptr<And::GraphicsContext> g_context = window->get_context();
    std::shared_ptr<And::Renderer> g_renderer = And::Renderer::CreateShared(*window);

    std::vector<std::string> paths = { {"demo/skybox/lago/right.jpg"}, {"demo/skybox/lago/left.jpg"}, {"demo/skybox/lago/top.jpg"}, {"demo/skybox/lago/bottom.jpg"}, {"demo/skybox/lago/front.jpg"}, {"demo/skybox/lago/back.jpg"} };
    std::shared_ptr<And::SkyboxTexture> sky_box = And::MakeSkyboxTexture(paths);
    g_renderer->set_skybox_texture(sky_box);
    g_renderer->enable_skybox(true);


    And::FlyCamera fly_cam{*window};
    fly_cam.SetPosition(-5.0f, 10.0f, 10.0f);
    fly_cam.SetSize(1920.0f, 1080.0f);
    fly_cam.SetFar(1000.0f);
    fly_cam.SetNear(0.1f);
    fly_cam.SetPosition(0.0f, 5.0f, 10.0f);
    fly_cam.SetFov(90.0f);
    fly_cam.SetDirection(0.0f, 0.0f, -1.0f);
    fly_cam.SetSpeed(10.0f);
    g_renderer->set_camera(&fly_cam);

    And::ResourceManager r_manager{*window, ts};
    r_manager.AddGenerator<And::ShaderGenerator>();
  
    And::Editor editor{*window, &r_manager};

    editor.AddWindow(ts.GetEditorWindow());

    // Show pc info
    g_context->create_info();
    float clear_color[4] = {1.0f, 0.0f, 0.0f, 1.0f};
    g_renderer->set_clear_color(clear_color);
    And::EntityComponentSystem entity_comp;
    And::AddBasicComponents(entity_comp);

    //std::shared_ptr<And::PhysicsEngine> physics_engine = And::PhysicsEngine::Init(false, 8192);

  {

      And::TransformComponent scene;
      scene.SetPosition(0.0f, 0.0f, 0.0f);
      scene.SetScale(1.0f, 1.0f, 1.0f);
      scene.SetRotation(0.0f, 0.0f, 0.0f);
      And::Entity* scene_entity = entity_comp.new_entity(scene);
      

      CreateHabitaculo(entity_comp, scene_entity);
      CreateLighting(entity_comp);
      CreateFurnitures(entity_comp, scene_entity);
    
  }

  And::AudioManager audio_manager;
  And::Audio audio_tele;
  And::Audio audio_lapiz;

  audio_tele.load("demo/audio/jazz_estereo.wav");
  audio_tele.SetPosition(33.0f, 5.0f, -15.0f);
  audio_tele.SetGain(1.0f);
  audio_tele.SetDoppler(true);
  audio_tele.SetDopplerFactor(10.0f);
  audio_tele.SetLooping(true);
  audio_tele.SetPitch(1.0f);
  audio_tele.SetMaxDistance(30.0f);
  
  audio_lapiz.load("demo/audio/lapiz.wav");
  audio_lapiz.SetPosition(-16.0f, 10.0f, 50.0f);
  audio_lapiz.SetGain(1.0f);
  audio_lapiz.SetDoppler(true);
  audio_lapiz.SetDopplerFactor(5.0f);
  audio_lapiz.SetLooping(true);
  audio_lapiz.SetPitch(1.0f);
  audio_lapiz.SetMaxDistance(20.0f);

  audio_manager.play(audio_lapiz);

  And::Input input{ *window };
  And::ActionInput light_tv{ "LightTV", And::KeyState::Press, { And::KeyCode::T} };
  bool is_light_tv = false;

  bool change_light = false;

  float time = 0.0f;
  float fps_count = 0.0f;
  while (window->is_open()){
       
    window->update();
    g_renderer->new_frame();
    editor.ShowWindows();
  
    fly_cam.ProcessInput();
    fly_cam.ShowValues();
    
    // Code Here


    if (input.check_action(light_tv)) {

        //static void LaunchBall(const float* pos, const float* dir, float force, And::EntityComponentSystem & ecs, And::PhysicsEngine & engine, And::MeshComponent & mesh, And::MaterialComponent & material_comp)
        if (!is_light_tv) {
            change_light = !change_light;
        }
        is_light_tv = true;
    }
    else {
        is_light_tv = false;
    }



    if (change_light) {

        audio_manager.play(audio_tele);
        And::PointLight* p = point_tv->get_component<And::PointLight>();
        const float speed = 0.2f;
        //float r = std::abs(sinf(time * speed));
        float r = 1.0f;
        float g = (cosf(time * speed) * 0.5f) + 0.5f;
        float b = 0.1f;
        //float b = std::abs(cosf(time * 0.7f * speed));
        
        p->SetDiffuseColor(r, g, b);

        //printf("R %f G %f B %f\n", r, g, b);
        p->SetIntensity(300.0f);

        for (int i = 0; i < 4; i++) {
            points_habitaculo[i]->get_component<And::PointLight>()->SetEnabled(false);
        }
        And::SpotLight* spot = spot_estanteria0->get_component<And::SpotLight>();
        spot->SetDiffuseColor(1.0f, 0.0f, 0.0f);
        spot->SetIntensity(300.0f);

        spot = spot_estanteria1->get_component<And::SpotLight>();
        spot->SetDiffuseColor(1.0f, 0.0f, 0.0f);
        spot->SetIntensity(300.0f);

        spot = spot_estanteria2->get_component<And::SpotLight>();
        spot->SetDiffuseColor(1.0f, 0.0f, 0.0f);
        spot->SetIntensity(300.0f);

        for (int i = 0; i < 3; i++) {
            spheres_disco[i]->get_component<And::TransformComponent>()->SetRotation(0.0f, time, 0.0f);
            spot_cuadro[i]->get_component<And::SpotLight>()->SetEnabled(true);
        }

        point_hexagonos->get_component<And::PointLight>()->SetDiffuseColor(1.0f, 0.0f, 0.0f);
        point_hexagonos->get_component<And::PointLight>()->SetIntensity(100.0f);

        
    } else {
        audio_manager.stop(audio_tele);
        And::PointLight* p = point_tv->get_component<And::PointLight>();
        //p->SetIntensity(0.0f);
        //p->SetIntensity(1.0f);
        p->SetDiffuseColor(1.0f, 0.01f, 0.01f);
        p->SetIntensity(400.0f);
        for (int i = 0; i < 4; i++) {
            points_habitaculo[i]->get_component<And::PointLight>()->SetEnabled(true);
        }

        And::SpotLight* spot = spot_estanteria0->get_component<And::SpotLight>();
        spot->SetDiffuseColor(1.0f, 1.0f, 1.0f);
        spot->SetIntensity(300.0f);

        spot = spot_estanteria1->get_component<And::SpotLight>();
        spot->SetDiffuseColor(1.0f, 1.0f, 1.0f);
        spot->SetIntensity(300.0f);
        
        spot = spot_estanteria2->get_component<And::SpotLight>();
        spot->SetDiffuseColor(1.0f, 1.0f, 1.0f);
        spot->SetIntensity(300.0f);

        for (int i = 0; i < 3; i++) {
            spot_cuadro[i]->get_component<And::SpotLight>()->SetEnabled(false);
        }

        point_hexagonos->get_component<And::PointLight>()->SetDiffuseColor(1.0f, 1.0f, 1.0f);
        point_hexagonos->get_component<And::PointLight>()->SetIntensity(300.0f);
        

    }

    const float* src_pos = fly_cam.GetPosition();
    const float* src_dir = fly_cam.GetDirection();

    audio_tele.UpdateListenerPosition(src_pos);
    //audio_tele.UpdateListenerDirection(src_dir);
    audio_tele.ApplyEffects();

    audio_manager.Update();
  
 




    //physics_engine->Simulate(window->get_delta_time() > 1.0f ? 1.0f / 30.0f : window->get_delta_time());
    //physics_engine->Apply(entity_comp);
    
    //g_renderer->draw_forward(entity_comp);
    //g_renderer->draw_deferred(entity_comp);
    g_renderer->draw_pbr(entity_comp);
    
    time += window->get_delta_time();
    fps_count += 0.5f * window->get_delta_time();
    g_renderer->end_frame();
    window->swap_buffers();
  }

  //physics_engine->Release(entity_comp);

  return 0;
}