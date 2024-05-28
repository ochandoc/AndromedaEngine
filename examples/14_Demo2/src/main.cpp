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
#include "Andromeda.h"


#define POOL_SIZE 5
const float PI = 3.14159265f;


static And::Entity* big_balls[POOL_SIZE];
static std::random_device rd;
static std::mt19937 gen(rd());

static And::Entity* cuadros[6];
static And::Entity* cubos_1[6*6];
static And::Entity* cubos_2[6*6];

static void LaunchBall(const float* pos, const float* dir, float force, And::EntityComponentSystem& ecs, And::PhysicsEngine& engine, And::MeshComponent& mesh, And::MaterialComponent& material_comp, bool big = false) {

    And::TransformComponent tran;
    tran.position[0] = pos[0] + (dir[0] * 5.0f);
    tran.position[1] = pos[1] + (dir[1] * 5.0f);
    tran.position[2] = pos[2] + (dir[2] * 5.0f);
    
    //tran.position[0] = 1.0f;
    //tran.position[1] = 1.0f;
    //tran.position[2] = 1.0f;
    
    tran.rotation[0] = 0.0f;
    tran.rotation[1] = 0.0f;
    tran.rotation[2] = 0.0f;
    tran.rotation[3] = 0.0f;

    tran.scale[0] = 0.5f;
    tran.scale[1] = 0.5f;
    tran.scale[2] = 0.5f;


    And::RigidBody rb = engine.CreateRigidBody();
    
    if (big) {
        tran.scale[0] = 2.0f;
        tran.scale[1] = 2.0f;
        tran.scale[2] = 2.0f;

        rb.AddSphereCollider(tran.position, 2.0f, And::ColliderType::RigidDynamic, 0.4f, 0.4f, 0.8f);
        rb.SetMass(60.0f);
        force *= 4.0f;
    }else {

        rb.AddSphereCollider(tran.position, 0.5f, And::ColliderType::RigidDynamic, 0.4f, 0.4f, 0.8f);
        rb.SetMass(20.0f);
    }
   
    rb.AffectsGravity(true);

    tran.HasRigidBody();

    //And::Entity* e = ecs.new_entity(mesh, material_comp, tran);
    And::Entity* e = ecs.new_entity(mesh, material_comp, tran, rb);

    float* transform_tmp = e->get_component<And::TransformComponent>()->GetModelMatrix();

    float new_dir[3] = {dir[0] * force, dir[1] * force, dir[2] * force };
    
    And::RigidBody* tmp = e->get_component<And::RigidBody>();
    tmp->AddForce(new_dir, And::ForceMode::IMPULSE);
}

static void ThrowBall(And::Entity* pool[], int index) {

    And::RigidBody* rb = pool[index]->get_component<And::RigidBody>();
    rb->SetPosition(0.0f, 5.0, 0.0f);

    int minimo = 1;
    int maximo = 1000;
    std::uniform_int_distribution<> distribucion(minimo, maximo);
    int numero_aleatorio_x = distribucion(gen);
    int numero_aleatorio_z = distribucion(gen);

    float x = numero_aleatorio_x / 100;
    float z = numero_aleatorio_z / 100;

    rb->AddForce(x, 50.0f, z, And::ForceMode::IMPULSE);
    printf("Lanuch at X:%f Y:%f Z:%f\n", x, 50.0f, z);

}

static void CreateBowlingSuelo(And::EntityComponentSystem& ecs, And::PhysicsEngine& engine, And::MeshComponent& MC_suelo,  And::MaterialComponent& suelo_mat_comp) {
    float scale_tmp[3] = { 1.0f, 10.0f, 10.0f };
    And::TransformComponent suelo_tran;
    suelo_tran.SetRotation(0.0f, 0.0f, 0.0f);
    suelo_tran.SetScale(scale_tmp);

    suelo_tran.HasRigidBody();

    const float suelo_scale = 20.0f;
    const int suelo_num = 3;
    suelo_tran.SetScale(20.0f, 1.0f, suelo_scale);

    float scale[3] = { suelo_scale , 1.0f, suelo_scale };

    for (int i = -suelo_num; i <= suelo_num; i++) {
      float pos[3] = { 0.0f , 0.0f, i * suelo_scale };

      And::RigidBody rb = engine.CreateRigidBody();
      rb.AddBoxCollider(pos, scale, And::ColliderType::RigidStatic, 0.5f, 0.5f, 0.8f);
      suelo_tran.SetPosition(pos[0], pos[1], pos[2]);
      suelo_tran.HasRigidBody();
      And::Entity* ett = ecs.new_entity(MC_suelo, suelo_mat_comp, suelo_tran, rb);
      if (i == suelo_num)
      {
        float Color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
        std::shared_ptr<And::Material> mat = std::make_shared<And::Material>(*suelo_mat_comp.GetMaterial());
        mat->SetColor(Color);
        mat->SetColorTexture(nullptr);
        ett->get_component<And::MaterialComponent>()->SetMaterial(mat);

        float pos2[3] = { pos[0], pos[1] + 8.75f, pos[2] };
        suelo_tran.SetPosition(pos2[0], pos2[1], pos2[2]);
        And::RigidBody rb2 = engine.CreateRigidBody();
        rb2.AddBoxCollider(pos2, scale, And::ColliderType::RigidStatic, 0.5f, 0.5f, 0.8f);
        ett = ecs.new_entity(MC_suelo, suelo_mat_comp, suelo_tran, rb2);
        ett->get_component<And::MaterialComponent>()->SetMaterial(mat);

        float scale2[3] = { scale[1], 10.0f, scale[2] };
        {
          pos2[1] -= 4.0f;
          pos2[0] += 10.5f;
          And::TransformComponent tr;
          tr.SetPosition(pos2);
          tr.SetScale(scale2);
          tr.HasRigidBody();
          And::RigidBody rb3 = engine.CreateRigidBody();
          rb3.AddBoxCollider(pos2, scale2, And::ColliderType::RigidStatic, 0.5f, 0.5f, 0.8f);
          ett = ecs.new_entity(MC_suelo, suelo_mat_comp, tr, rb3);
          ett->get_component<And::MaterialComponent>()->SetMaterial(mat);
        }

        {
          pos2[0] -= 10.5f * 2.0f;
          And::TransformComponent tr;
          tr.SetPosition(pos2);
          tr.SetScale(scale2);
          tr.HasRigidBody();
          And::RigidBody rb3 = engine.CreateRigidBody();
          rb3.AddBoxCollider(pos2, scale2, And::ColliderType::RigidStatic, 0.5f, 0.5f, 0.8f);
          ett = ecs.new_entity(MC_suelo, suelo_mat_comp, tr, rb3);
          ett->get_component<And::MaterialComponent>()->SetMaterial(mat);
        }

      }
    }
}

static void CreateSuelo(And::EntityComponentSystem& ecs, And::PhysicsEngine& engine, And::MeshComponent& MC_suelo, And::MaterialComponent& suelo_mat_comp) {
  
  And::TransformComponent suelo_tran;
  suelo_tran.SetRotation(0.0f, 0.0f, 0.0f);

  suelo_tran.HasRigidBody();

  const float suelo_scale = 20.0f;
  const int suelo_num = 1;
  const int suelo_num_y = 3;
  suelo_tran.SetScale(20.0f, 2.0f, suelo_scale);

  float scale[3] = { suelo_scale , 2.0f, suelo_scale };

  for (int j = -suelo_num; j <= suelo_num; j++) {
    if (j == 0) continue;

    for (int i = -suelo_num_y; i < suelo_num_y; i++) {

      float pos[3] = { j * suelo_scale , 1.5f, i * suelo_scale };

      And::RigidBody rb = engine.CreateRigidBody();
      rb.AddBoxCollider(pos, scale, And::ColliderType::RigidStatic, 0.5f, 0.5f, 0.8f);
      suelo_tran.SetPosition(pos[0], pos[1], pos[2]);
      suelo_tran.HasRigidBody();
      ecs.new_entity(MC_suelo, suelo_mat_comp, suelo_tran, rb);
    }
  }
}

static void CreateWalls(And::EntityComponentSystem& ecs, And::PhysicsEngine& engine, And::MeshComponent& MC_suelo, And::MaterialComponent& suelo_mat_comp) {

  And::TransformComponent suelo_tran;
  suelo_tran.SetRotation(0.0f, 0.0f, 0.0f);

  suelo_tran.HasRigidBody();

  const float suelo_scale = 20.0f;
  const int suelo_num = 1;
  const int suelo_num_y = 3;
  suelo_tran.SetScale(2.0f, 25.0f, 20.0f);

  float scale[3] = { 2.0f , 25.0f, suelo_scale };

  constexpr int32 wall_num = 3;
  for (int32 i = -wall_num; i < wall_num; i++)
  {
    float pos[3] = { 30.0f , 12.5f, i * suelo_scale };

    And::RigidBody rb = engine.CreateRigidBody();
    rb.AddBoxCollider(pos, scale, And::ColliderType::RigidStatic, 0.5f, 0.5f, 0.8f);
    suelo_tran.SetPosition(pos[0], pos[1], pos[2]);
    suelo_tran.HasRigidBody();
    ecs.new_entity(MC_suelo, suelo_mat_comp, suelo_tran, rb);
  }

  for (int32 i = -wall_num; i < wall_num; i++)
  {
    float pos[3] = { -30.0f , 12.5f, i * suelo_scale };

    And::RigidBody rb = engine.CreateRigidBody();
    rb.AddBoxCollider(pos, scale, And::ColliderType::RigidStatic, 0.5f, 0.5f, 0.8f);
    suelo_tran.SetPosition(pos[0], pos[1], pos[2]);
    suelo_tran.HasRigidBody();
    ecs.new_entity(MC_suelo, suelo_mat_comp, suelo_tran, rb);
  }

  constexpr int32 wall2_num = 1;
  std::swap(scale[0], scale[2]);
  for (int32 i = -wall2_num; i <= wall2_num; i++)
  {
    float pos[3] = { i * suelo_scale , 12.5f, 50.0f };
    if (i == 0)
    {
      scale[1] = 25.0f * 0.65f;
      pos[1] += 4.375f;
      suelo_tran.SetScale(scale[0], scale[1], scale[2]);
      And::RigidBody rb = engine.CreateRigidBody();
      rb.AddBoxCollider(pos, scale, And::ColliderType::RigidStatic, 0.5f, 0.5f, 0.8f);
      suelo_tran.SetPosition(pos[0], pos[1], pos[2]);
      suelo_tran.HasRigidBody();
      ecs.new_entity(MC_suelo, suelo_mat_comp, suelo_tran, rb);
    }
    else
    {
      scale[1] = 25.0f;
      suelo_tran.SetScale(scale);
      And::RigidBody rb = engine.CreateRigidBody();
      rb.AddBoxCollider(pos, scale, And::ColliderType::RigidStatic, 0.5f, 0.5f, 0.8f);
      suelo_tran.SetPosition(pos[0], pos[1], pos[2]);
      suelo_tran.HasRigidBody();
      ecs.new_entity(MC_suelo, suelo_mat_comp, suelo_tran, rb);
    }
  }
}

void CreateBallsPool(And::Entity* e[], std::shared_ptr<And::PhysicsEngine> engine, And::EntityComponentSystem& entity_comp, And::MaterialComponent& mat_comp) {
    for (int i = 0; i < POOL_SIZE; i++) {
        float pos[3] = { i * 2.0f , 100.0f, 0.0f };

        float base[3] = { 0.5f, 0.5f, 0.5f };
        float scale[3] = { 0.5f, 0.5f, 0.5f };

        And::MeshComponent MC;
        MC.MeshOBJ = And::Geometry::load("sphere.obj");
        And::RawMesh raw_mesh_tmp(MC.MeshOBJ->get_vertices(), MC.MeshOBJ->get_indices());
        std::shared_ptr<And::Mesh> mesh_tmp = std::make_shared<And::Mesh>(raw_mesh_tmp);
        MC.SetMesh(mesh_tmp);


        And::TransformComponent tran;
        tran.SetPosition(pos);
        tran.SetRotation(0.0f, 0.0f, 0.0f);
        tran.SetScale(scale);
        tran.HasRigidBody();

        And::RigidBody rb = engine->CreateRigidBody();
        rb.AddSphereCollider(pos, 0.5f, And::ColliderType::RigidDynamic, 0.4f, 0.4f, 0.8f);
        rb.AffectsGravity(true);
        rb.SetMass(4.0f);


        e[i] = entity_comp.new_entity(MC, mat_comp, tran, rb);
    }

}


struct Bowls
{
  float CreationPos[3];
  And::Entity* Entities[16];
};

void CreateBowls(Bowls& InOutBowls, And::EntityComponentSystem& ecs, And::PhysicsEngine& engine, And::MeshComponent& mesh, And::MaterialComponent& material_comp) {
    
    And::TransformComponent tr;
    
    tr.SetRotation(0.0f, 0.0f, 0.0f);
    tr.SetScale(15.0f, 15.0f, 15.0f);
    tr.HasRigidBody();

    float initial_position_x = InOutBowls.CreationPos[0];
    float initial_position_z = InOutBowls.CreationPos[1];
    float initial_position_y = InOutBowls.CreationPos[2];
    float separation = 4.0f;
    float separation_x = 3.0f;


    float scale[3] = {1.2f, 5.5f, 1.2f};
    float position[3] = { 1.0f, 1.0f, 1.0f };

    tr.SetPositionOffet(0.0f, -3.0f, 0.0f);

    uint32 EntityIndex = 0;
    for (int i = -3; i <= 3; i++) {
        And::RigidBody rb = engine.CreateRigidBody();
        tr.SetPosition(initial_position_x + (i * separation_x), initial_position_y, initial_position_z);
        position[0] = initial_position_x + (i * separation_x);
        position[1] = initial_position_y;
        position[2] = initial_position_z;
        rb.AddBoxCollider(position, scale, And::ColliderType::RigidDynamic, 0.4f, 0.4f, 0.5f);
        rb.SetMass(5.0f);
        rb.AffectsGravity(true);
        tr.HasRigidBody();
        InOutBowls.Entities[EntityIndex] = ecs.new_entity(mesh, material_comp, tr, rb);
        EntityIndex++;
    }
    
    for (int i = -2; i <= 2; i++) {
        And::RigidBody rb = engine.CreateRigidBody();
        tr.SetPosition(initial_position_x + (i * separation_x), initial_position_y, initial_position_z + (-separation));
        position[0] = initial_position_x + (i * separation_x);
        position[1] = initial_position_y;
        position[2] = initial_position_z + (-separation);
        rb.AddBoxCollider(position, scale, And::ColliderType::RigidDynamic, 0.4f, 0.4f, 0.5f);
        rb.SetMass(5.0f);
        rb.AffectsGravity(true);
        tr.HasRigidBody();
        InOutBowls.Entities[EntityIndex] = ecs.new_entity(mesh, material_comp, tr, rb);
        EntityIndex++;
    }
    
    for (int i = -1; i <= 1; i++) {
        And::RigidBody rb = engine.CreateRigidBody();
        tr.SetPosition(initial_position_x + (i * separation_x), initial_position_y, initial_position_z + ( -separation * 2.0f));
        position[0] = initial_position_x + (i * separation_x);
        position[1] = initial_position_y;
        position[2] = initial_position_z + (-separation * 2.0f);
        rb.AddBoxCollider(position, scale, And::ColliderType::RigidDynamic, 0.4f, 0.4f, 0.5f);
        rb.SetMass(5.0f);
        rb.AffectsGravity(true);
        tr.HasRigidBody();
        InOutBowls.Entities[EntityIndex] = ecs.new_entity(mesh, material_comp, tr, rb);
        EntityIndex++;
    }

    tr.SetPosition(initial_position_x, initial_position_y, initial_position_z + (-separation * 3.0f));
    position[0] = initial_position_x;
    position[1] = initial_position_y;
    position[2] = initial_position_z + (-separation * 3.0f);
    And::RigidBody rb = engine.CreateRigidBody();
    rb.AddBoxCollider(position, scale, And::ColliderType::RigidDynamic, 0.4f, 0.4f, 0.5f);
    rb.SetMass(5.0f);
    rb.AffectsGravity(true);
    tr.HasRigidBody();
    InOutBowls.Entities[EntityIndex] = ecs.new_entity(mesh, material_comp, tr, rb);
    EntityIndex++;
}

void PlaceBowls(Bowls& InBowls)
{
  float initial_position_x = InBowls.CreationPos[0];
  float initial_position_z = InBowls.CreationPos[1];
  float initial_position_y = InBowls.CreationPos[2];
  float separation = 4.0f;
  float separation_x = 3.0f;

  float scale[3] = { 1.2f, 5.5f, 1.2f };
  float position[3] = { 1.0f, 1.0f, 1.0f };

  for (int i = 0; i < 16; i++)
  {
    InBowls.Entities[i]->get_component<And::RigidBody>()->Sleep();
  }

  uint32 EntityIndex = 0;
  for (int i = -3; i <= 3; i++) {
    InBowls.Entities[EntityIndex]->get_component<And::TransformComponent>()->SetPosition(initial_position_x + (i * separation_x), initial_position_y, initial_position_z);
    position[0] = initial_position_x + (i * separation_x);
    position[1] = initial_position_y;
    position[2] = initial_position_z;

    InBowls.Entities[EntityIndex]->get_component<And::RigidBody>()->SetPosition(position, false);
    InBowls.Entities[EntityIndex]->get_component<And::RigidBody>()->SetRotation(0.0f, 0.0f, 0.0f, false);

    EntityIndex++;
  }

  for (int i = -2; i <= 2; i++) {
    InBowls.Entities[EntityIndex]->get_component<And::TransformComponent>()->SetPosition(initial_position_x + (i * separation_x), initial_position_y, initial_position_z + (-separation));
    position[0] = initial_position_x + (i * separation_x);
    position[1] = initial_position_y;
    position[2] = initial_position_z + (-separation);

    InBowls.Entities[EntityIndex]->get_component<And::RigidBody>()->SetPosition(position, false);
    InBowls.Entities[EntityIndex]->get_component<And::RigidBody>()->SetRotation(0.0f, 0.0f, 0.0f, false);

    EntityIndex++;
  }

  for (int i = -1; i <= 1; i++) {
    InBowls.Entities[EntityIndex]->get_component<And::TransformComponent>()->SetPosition(initial_position_x + (i * separation_x), initial_position_y, initial_position_z + (-separation * 2.0f));
    position[0] = initial_position_x + (i * separation_x);
    position[1] = initial_position_y;
    position[2] = initial_position_z + (-separation * 2.0f);

    InBowls.Entities[EntityIndex]->get_component<And::RigidBody>()->SetPosition(position, false);
    InBowls.Entities[EntityIndex]->get_component<And::RigidBody>()->SetRotation(0.0f, 0.0f, 0.0f, false);

    EntityIndex++;
  }

  InBowls.Entities[EntityIndex]->get_component<And::TransformComponent>()->SetPosition(initial_position_x, initial_position_y, initial_position_z + (-separation * 3.0f));
  position[0] = initial_position_x;
  position[1] = initial_position_y;
  position[2] = initial_position_z + (-separation * 3.0f);

  InBowls.Entities[EntityIndex]->get_component<And::RigidBody>()->SetPosition(position, false);
  InBowls.Entities[EntityIndex]->get_component<And::RigidBody>()->SetRotation(0.0f, 0.0f, 0.0f, false);

  EntityIndex++;
}

void UpdateLight(And::Entity* point, And::Entity* vela, And::FlyCamera& cam) {
    const float* pos = cam.GetPosition();
    const float* dir= cam.GetDirection();
    const float* up = cam.GetUp();
    const float* right = cam.GetRight();

    float distance = 2.0f;
    float pos_x = ((pos[0] + (dir[0] * distance)) + (right[0] * -2.0f)) + (up[0] * -3.0f);
    float pos_y = ((pos[1] + (dir[1] * distance)) + (right[1] * -2.0f)) + (up[1] * -3.0f);
    float pos_z = ((pos[2] + (dir[2] * distance)) + (right[2] * -2.0f)) + (up[2] * -3.0f);


    float pos_x_light = ((pos[0] + (dir[0] * (distance + 3.0f))) + right[0]) + up[0];
    float pos_y_light = ((pos[1] + (dir[1] * (distance + 3.0f))) + right[1]) + up[1];
    float pos_z_light = ((pos[2] + (dir[2] * (distance + 3.0f))) + right[2]) + up[2];


    float yaw = atan2f(dir[0],dir[2]);
    

    point->get_component<And::PointLight>()->SetPosition(pos_x_light, pos_y_light, pos_z_light);
    And::TransformComponent* tmp = vela->get_component<And::TransformComponent>();
    tmp->SetPosition(pos_x, pos_y, pos_z);
    tmp->SetRotation(0.0f, yaw, 0.0f);
}

void CreateCuadros(And::EntityComponentSystem& ecs) {

    {
        And::MaterialComponent mat_com;
        std::shared_ptr<And::Material> mat = std::make_shared<And::Material>();
        mat->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
        mat_com.SetMaterial(mat);

        And::MeshComponent MC;
        MC.MeshOBJ = And::Geometry::load("cube.obj");
        And::RawMesh raw_mesh(MC.MeshOBJ->get_vertices(), MC.MeshOBJ->get_indices());
        std::shared_ptr<And::Mesh> mesh = std::make_shared<And::Mesh>(raw_mesh);
        MC.SetMesh(mesh);

        And::TransformComponent tr;
        tr.SetPosition(28.0f, 14.0f, -42.0f);
        tr.SetRotation(0.0f, PI * 0.5f, 0.0f);
        tr.SetScale(7.5f, 7.5f, 1.0f);

        const float distance = 25.0f;
        for (int i = 0; i < 3; i++) {
            tr.SetPosition(28.0f, 14.0f, -42.0f + (float)(i * distance)),
            cuadros[i] = ecs.new_entity(mat_com, MC, tr);
        }
        
        for (int i = 0; i < 3; i++) {
            tr.SetPosition(-28.0f, 14.0f, -42.0f + (float)(i * distance)),
            cuadros[i + 3] = ecs.new_entity(mat_com, MC, tr);
        }
    }
}

void CreateMuros(And::EntityComponentSystem& ecs, And::PhysicsEngine& engine){
    {
        And::MaterialComponent mat_com;
        std::shared_ptr<And::Material> mat = std::make_shared<And::Material>();
        mat->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
        mat_com.SetMaterial(mat);

        And::MeshComponent MC;
        MC.MeshOBJ = And::Geometry::load("cube.obj");
        And::RawMesh raw_mesh(MC.MeshOBJ->get_vertices(), MC.MeshOBJ->get_indices());
        std::shared_ptr<And::Mesh> mesh = std::make_shared<And::Mesh>(raw_mesh);
        MC.SetMesh(mesh);

        And::TransformComponent tr;
        //tr.SetPosition(18.0f, 3.0f, -42.0f);
        tr.SetRotation(0.0f, 0.0f, 0.0f);
        tr.SetScale(2.0f, 2.0f, 2.0f);

        float scale[3] = {2.0f, 2.0f, 2.0f};

        int index = 0;
        for (int i = 0; i < 6; i++) {
            for (int j = 0; j < 6; j++) {
                And::RigidBody rb = engine.CreateRigidBody();
                float x = 18.0f;
                float y = 3.5f + (float)(j) * 2.0f;
                float z = -42.0f + (float)(i * 2.0f);

                float pos[3] = {x,y,z};
                tr.SetPosition(x, y, z);
                rb.AddBoxCollider(pos, scale, And::ColliderType::RigidDynamic, 0.5f, 0.5f, 6.0f);
                rb.SetMass(10.0f);
                rb.AffectsGravity(true);
                tr.HasRigidBody(true);
                cubos_1[index] = ecs.new_entity(mat_com, MC, tr, rb);
                index++;
            }
        }

        index = 0;
        for (int i = 0; i < 6; i++) {
            for (int j = 0; j < 6; j++) {
                And::RigidBody rb = engine.CreateRigidBody();
                float x = -18.0f;
                float y = 3.5f + (float)(j) * 2.0f;
                float z = -42.0f + (float)(i * 2.0f);

                float pos[3] = { x,y,z };
                tr.SetPosition(x, y, z);
                rb.AddBoxCollider(pos, scale, And::ColliderType::RigidDynamic, 0.5f, 0.5f, 6.0f);
                rb.SetMass(10.0f);
                rb.AffectsGravity(true);
                tr.HasRigidBody(true);
                cubos_2[index] = ecs.new_entity(mat_com, MC, tr, rb);
                index++;
            }
        }

        
    }
}

void UpdateCuadrosColor(float time) {

    for (int i = 0; i < 6; i++) {
        float r = cosf(time * 0.8f) * 0.5f + 0.5f;
        float g = sinf(time * 0.7f) * 0.5f + 0.5f;
        float b = cosf(time * 0.6f) * 0.5f + 0.5f;
        cuadros[i]->get_component<And::MaterialComponent>()->GetMaterial()->SetColor(r,g,b, 1.0f);
    }

}

void UpdateCubosColor(float time) {

    for (int i = 0; i < 6*6; i++) {
        float r = cosf(time * 0.8f) * 0.5f + 0.5f;
        float g = sinf(time * 0.7f) * 0.5f + 0.5f;
        float b = cosf(time * 0.6f) * 0.5f + 0.5f;
        cubos_1[i]->get_component<And::MaterialComponent>()->GetMaterial()->SetColor(r,g,b, 1.0f);
        cubos_2[i]->get_component<And::MaterialComponent>()->GetMaterial()->SetColor(r,g,b, 1.0f);
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

    std::vector<std::string> paths = { {"skybox/right.jpg"}, {"skybox/left.jpg"}, {"skybox/top.jpg"}, {"skybox/bottom.jpg"}, {"skybox/back.jpg"}, {"skybox/front.jpg"} };
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

    std::shared_ptr<And::PhysicsEngine> physics_engine = And::PhysicsEngine::Init(false, 8192);



    //And::MeshComponent mesh_com_tmp;
    //mesh_com_tmp.MeshOBJ = And::Geometry::load("demo/obj/fountain.obj");
    //And::RawMesh raw_mesh_tmp(mesh_com_tmp.MeshOBJ->get_vertices(), mesh_com_tmp.MeshOBJ->get_indices());
    //std::shared_ptr<And::Mesh> mesh_tmp = std::make_shared<And::Mesh>(raw_mesh_tmp);



    int num_obj = 10;
    float pos_x = 0.0f;
    float pos_y = -5.0f;

      
    And::MeshComponent MC_bolinga;
    MC_bolinga.MeshOBJ = And::Geometry::load("sphere.obj");
    And::RawMesh raw_mesh_tmp(MC_bolinga.MeshOBJ->get_vertices(), MC_bolinga.MeshOBJ->get_indices());
    std::shared_ptr<And::Mesh> mesh_tmp = std::make_shared<And::Mesh>(raw_mesh_tmp);
    MC_bolinga.SetMesh(mesh_tmp);

    And::MaterialComponent bolinga_mat_comp;
    std::shared_ptr<And::Material> bolinga_mat = std::make_shared<And::Material>();
    std::shared_ptr<And::Texture> bolinga_tex = And::MakeTexture("demo/textures/bola/albedo.png");
    std::shared_ptr<And::Texture> bolinga_normals = And::MakeTexture("demo/textures/bola/normals.png");
    std::shared_ptr<And::Texture> bolinga_ao = And::MakeTexture("demo/textures/bola/ao.png");
    std::shared_ptr<And::Texture> bolinga_metallic = And::MakeTexture("demo/textures/bola/metallic.png");
    std::shared_ptr<And::Texture> bolinga_rou = And::MakeTexture("demo/textures/bola/roughness.png");
    bolinga_mat->SetColorTexture(bolinga_tex);
    bolinga_mat->SetNormalTexture(bolinga_normals);
    bolinga_mat->SetAmbientOclusionTexture(bolinga_ao);
    bolinga_mat->SetMetallicTexture(bolinga_metallic);
    bolinga_mat->SetRoughnessTexture(bolinga_rou);
    bolinga_mat_comp.SetMaterial(bolinga_mat);

    And::TransformComponent bolinga_tran; 
    bolinga_tran.SetPosition(5.0f, 3.0f, 0.0f);
    bolinga_tran.SetRotation(0.0f, 0.0f, 0.0f);
    bolinga_tran.SetScale(1.0f, 1.0f, 1.0f);
    bolinga_tran.HasRigidBody(false);
    //And::Entity* bolinga_entity = entity_comp.new_entity(MC_bolinga, bolinga_mat_comp, bolinga_tran);
        

    And::MeshComponent MC_suelo;
    MC_suelo.MeshOBJ = And::Geometry::load("cube.obj");
    And::RawMesh raw_mesh_suelo_tmp(MC_suelo.MeshOBJ->get_vertices(), MC_suelo.MeshOBJ->get_indices());
    std::shared_ptr<And::Mesh> mesh_suelo_tmp = std::make_shared<And::Mesh>(raw_mesh_suelo_tmp);
    MC_suelo.SetMesh(mesh_suelo_tmp);

    And::MaterialComponent suelo_mat_comp;
    std::shared_ptr<And::Material> suelo_mat = std::make_shared<And::Material>();
    std::shared_ptr<And::Texture> suelo_tex_b = And::MakeTexture("demo/textures/suelo_3/albedo.png");
    std::shared_ptr<And::Texture> suelo_normals_b = And::MakeTexture("demo/textures/suelo_3/normals.png");
    std::shared_ptr<And::Texture> suelo_ao_b = And::MakeTexture("demo/textures/suelo_3/ao.png");
    std::shared_ptr<And::Texture> suelo_metallic_b = And::MakeTexture("demo/textures/suelo_3/metallic.png");
    std::shared_ptr<And::Texture> suelo_rou_b = And::MakeTexture("demo/textures/suelo_3/roughness.png");
    suelo_mat->SetColorTexture(suelo_tex_b);
    suelo_mat->SetNormalTexture(suelo_normals_b);
    suelo_mat->SetAmbientOclusionTexture(suelo_ao_b);
    suelo_mat->SetMetallicTexture(suelo_metallic_b);
    suelo_mat->SetRoughnessTexture(suelo_rou_b);
    suelo_mat_comp.SetMaterial(suelo_mat);
    
    CreateBowlingSuelo(entity_comp, *physics_engine, MC_suelo, suelo_mat_comp);
    {
      And::MaterialComponent suelo2_mat_comp;
      std::shared_ptr<And::Material> suelo2_mat = std::make_shared<And::Material>();
      std::shared_ptr<And::Texture> suelo2_tex_b = And::MakeTexture("demo/textures/marmol/albedo.png");
      std::shared_ptr<And::Texture> suelo2_normals_b = And::MakeTexture("demo/textures/marmol/normals.png");
      std::shared_ptr<And::Texture> suelo2_ao_b = And::MakeTexture("demo/textures/marmol/ao.png");
      std::shared_ptr<And::Texture> suelo2_metallic_b = And::MakeTexture("demo/textures/marmol/metallic.png");
      std::shared_ptr<And::Texture> suelo2_rou_b = And::MakeTexture("demo/textures/marmol/roughness.png");
      suelo2_mat->SetColorTexture(suelo2_tex_b);
      suelo2_mat->SetNormalTexture(suelo2_normals_b);
      suelo2_mat->SetAmbientOclusionTexture(suelo2_ao_b);
      suelo2_mat->SetMetallicTexture(suelo2_metallic_b);
      suelo2_mat->SetRoughnessTexture(suelo2_rou_b);
      suelo2_mat_comp.SetMaterial(suelo2_mat);
      CreateSuelo(entity_comp, *physics_engine, MC_suelo, suelo2_mat_comp);
      CreateWalls(entity_comp, *physics_engine, MC_suelo, suelo2_mat_comp);
    }

  {
        


      And::AmbientLight ambient;
      ambient.SetDiffuseColor(1.0f, 1.0f, 1.0f);
      ambient.SetAmbientStrenght(0.1f);
      //And::Entity* ambient_entity = entity_comp.new_entity(ambient);


      And::DirectionalLight directional;
      directional.SetDiffuseColor(1.0f, 1.0f, 1.0f);
      directional.SetDirection(0.5f, -0.5f, 0.5f);
      directional.SetSpecularColor(1.0f, 1.0f, 1.0f);
      directional.SetSpecularShininess(32.0f);
      directional.SetSpecularStrength(0.003f);
      directional.SetEnabled(true);
      directional.SetCastShadows(true);
      directional.SetIntensity(1.0f);
      entity_comp.new_entity(directional);

      float intensity = 2.0f;
      And::PointLight point;
      point.SetPosition(0.0f, 14.0f, 0.0f);
      point.SetSpecularColor(1.0f, 1.0f, 1.0f);
      point.SetSpecularShininess(32.0f);
      point.SetSpecularStrength(0.003f);
      point.SetConstantAtt(1.0f);
      point.SetLinearAtt(0.045f);
      point.SetQuadraticAtt(0.0075f);
      point.SetEnabled(true);
      point.SetCastShadows(true);
      point.SetIntensity(intensity);
      point.SetDiffuseColor(1.0f, 1.0f, 1.0f);
      //And::Entity* entity_tmp = entity_comp.new_entity(point);

      {
          And::SpotLight spot{};
          spot.SetPosition(0.0f, 16.0f, 44.0f);
          spot.SetDirection(0.0f, -1.0f, 0.0f);
          spot.SetDiffuseColor(1.0f, 1.0f, 1.0f);
          spot.SetSpecularColor(1.0f, 1.0f, 1.0f);
          spot.SetSpecularStrength(0.003f);
          spot.SetSpecularShininess(32.0f);
          spot.SetConstantAtt(1.0f);
          spot.SetLinearAtt(0.045f);
          spot.SetQuadraticAtt(0.0075f);
          spot.SetCuttOff(2.5f);
          spot.SetOuterCuttOff(50.0f);
          spot.SetCastShadows(true);
          spot.SetEnabled(true);
          spot.SetIntensity(intensity);
          //entity_comp.new_entity(spot);
      }
  }

  //std::shared_ptr<And::Geometry> geo = And::Geometry::load("cube.obj");
  float position_tmp[3] = {-1.0f, 20.0f, -15.0f};



  And::Input input{ *window };
  And::ActionInput jump{ "Jump", And::KeyState::Press, { And::KeyCode::Space} };
  And::ActionInput shot{ "Shot", And::KeyState::Press, { And::KeyCode::C} };
  And::ActionInput big_shot{ "BigShot", And::KeyState::Press, { And::KeyCode::V} };
  And::ActionInput changePoint{ "ChangePoint", And::KeyState::Press, { And::KeyCode::L} };
  And::ActionInput changeGravity{ "Changegravity", And::KeyState::Press, { And::KeyCode::G} };

  And::Entity* balls_pool[POOL_SIZE];
  int index_pool = 0;


  And::MaterialComponent mat_comp;
  std::shared_ptr<And::Material> mat = std::make_shared<And::Material>();
  std::shared_ptr<And::Texture> tex = And::MakeTexture("demo/textures/bola/albedo.png");
  std::shared_ptr<And::Texture> normals = And::MakeTexture("demo/textures/bola/normals.png");
  std::shared_ptr<And::Texture> ao = And::MakeTexture("demo/textures/bola/ao.png");
  std::shared_ptr<And::Texture> metallic = And::MakeTexture("demo/textures/bola/metallic.png");
  std::shared_ptr<And::Texture> rou = And::MakeTexture("demo/textures/bola/roughness.png");


  mat->SetColor(0.2f, 0.2f, 1.0f, 1.0f);
  mat->SetColor(0.353f, 0.698f, 1.0f, 1.0f);
  mat->SetColorTexture(tex);
  mat->SetNormalTexture(normals);
  mat->SetAmbientOclusionTexture(ao);
  mat->SetMetallicTexture(metallic);
  mat->SetRoughnessTexture(rou);

  mat_comp.SetMaterial(mat);

  //CreateBallsPool(balls_pool, physics_engine, entity_comp, bolinga_mat_comp);

  Bowls bowls;
  bowls.CreationPos[0] = 0.0f;
  bowls.CreationPos[1] = 47.0f;
  bowls.CreationPos[2] = 6.0f;

  {
      And::MaterialComponent mat_com;
      std::shared_ptr<And::Material> mat= std::make_shared<And::Material>();
      std::shared_ptr<And::Texture> tex= And::MakeTexture("demo/textures/bambu/albedo.png");
      std::shared_ptr<And::Texture> normals= And::MakeTexture("demo/textures/bambu/normals.png");
      std::shared_ptr<And::Texture> ao= And::MakeTexture("demo/textures/bambu/ao.png");
      std::shared_ptr<And::Texture> metallic= And::MakeTexture("demo/textures/bambu/metallic.png");
      std::shared_ptr<And::Texture> rou= And::MakeTexture("demo/textures/bambu/roughness.png");
      mat->SetColorTexture(tex);
      mat->SetNormalTexture(normals);
      mat->SetAmbientOclusionTexture(ao);
      mat->SetMetallicTexture(metallic);
      mat->SetRoughnessTexture(rou);
      mat_com.SetMaterial(mat);

      And::MeshComponent MC;
      MC.MeshOBJ = And::Geometry::load("demo/obj/bowling_pin.obj");
      And::RawMesh raw_mesh(MC.MeshOBJ->get_vertices(), MC.MeshOBJ->get_indices());
      std::shared_ptr<And::Mesh> mesh = std::make_shared<And::Mesh>(raw_mesh);
      MC.SetMesh(mesh);

      And::TransformComponent tr;
      tr.SetPosition(10.0f, 1.0f, 0.0f);
      tr.SetRotation(0.0f, 0.0f, 0.0f);
      tr.SetScale(5.0f, 5.0f, 5.0f);
      tr.HasRigidBody(false);

      //entity_comp.new_entity(mat_com, MC, tr);

      CreateBowls(bowls, entity_comp, *physics_engine, MC, mat_com);

  }


  CreateCuadros(entity_comp);
  CreateMuros(entity_comp, *physics_engine);


  And::AudioManager audio_manager;
  And::Audio audio_fondo;
  And::Audio audio_fuente;
  And::Audio audio_piano;
  audio_fondo.load("demo/audio/fondo.wav");
  audio_fuente.load("demo/audio/fuente.wav");
  audio_piano.load("demo/audio/piano.wav");

  
  audio_fondo.SetGain(0.3f);
  audio_fondo.SetLooping(true);

  audio_fuente.SetPosition(0.0f, 0.0f, 0.0f);
  audio_fuente.SetDoppler(true);
  audio_fuente.SetDopplerFactor(10.0f);
  audio_fuente.SetLooping(true);
  audio_fuente.SetPitch(1.0f);
  audio_fuente.SetMaxDistance(20.0f);

  audio_manager.play(audio_fondo);
  audio_manager.play(audio_fuente);


  float fps_count = 0.0f;
  const float force = 600.0f;
  int frames = 0;
  float secondsToSpawn = 4.0f;

  float fixed_update = 0.0f;

  float time = 0.0f;

  bool is_down = false;
  bool is_big_down = false;
  bool is_pointlight_down = false;
  bool is_gravity_down = false;

  bool change_gravity_tmp = false;

  //And::TransformComponent* tr_tmp = bolinga_entity->get_component<And::TransformComponent>();
  //g_renderer->enable_skybox(false);
  while (window->is_open()){
       
    
    window->update();
    g_renderer->new_frame();
    editor.ShowWindows();
    fps_count +=window->get_delta_time(); 
    fixed_update +=window->get_delta_time(); 

    //if(input.IsMouseButtonPressed(And::MouseCode::Left)){
    if (input.check_action(shot)) {

        //static void LaunchBall(const float* pos, const float* dir, float force, And::EntityComponentSystem & ecs, And::PhysicsEngine & engine, And::MeshComponent & mesh, And::MaterialComponent & material_comp)
        if (!is_down) {
            LaunchBall(fly_cam.GetPosition(), fly_cam.GetDirection(), force, entity_comp, *physics_engine, MC_bolinga, bolinga_mat_comp);
            
        }
        is_down = true;
    }else {
        is_down = false;
    }
    
    if (input.check_action(big_shot)) {

        //static void LaunchBall(const float* pos, const float* dir, float force, And::EntityComponentSystem & ecs, And::PhysicsEngine & engine, And::MeshComponent & mesh, And::MaterialComponent & material_comp)
        if (!is_big_down) {
            LaunchBall(fly_cam.GetPosition(), fly_cam.GetDirection(), force, entity_comp, *physics_engine, MC_bolinga, bolinga_mat_comp, true);
        }
        is_big_down = true;
    }else {
        is_big_down = false;
    }
    
    if (input.check_action(changeGravity)) {

        //static void LaunchBall(const float* pos, const float* dir, float force, And::EntityComponentSystem & ecs, And::PhysicsEngine & engine, And::MeshComponent & mesh, And::MaterialComponent & material_comp)
        if (!is_gravity_down) {
            change_gravity_tmp = !change_gravity_tmp;
            if (change_gravity_tmp) {
                physics_engine->SetGravity(0.0f, 9.81f, 0.0f);
            }
            else {
                physics_engine->SetGravity(0.0f, -9.81f, 0.0f);
            }
        }
        is_gravity_down = true;
    }else {
        is_gravity_down = false;
    }

    


    if (input.check_action(jump)) {
        //And::AmbientLight* tmp = ambient_entity->get_component<And::AmbientLight>();
        //tmp->SetEnabled(!tmp->GetEnabled());
      
    }


    if (input.check_action(changePoint)) {

        //static void LaunchBall(const float* pos, const float* dir, float force, And::EntityComponentSystem & ecs, And::PhysicsEngine & engine, And::MeshComponent & mesh, And::MaterialComponent & material_comp)
        if (!is_pointlight_down) {
          PlaceBowls(bowls);
        }
        is_pointlight_down = true;
    }else {
        is_pointlight_down = false;
    }

   
    //float* pos = point_tmp->GetPosition();
    //point_tmp->SetPosition(pos[0], std::abs(cosf(fps_count) * 3.0f), pos[2]);
    //printf("Position-> Y: %f\n", std::abs(cosf(fps_count) * 3.0f));

    fly_cam.ProcessInput();
    fly_cam.ShowValues();




    const float* src_pos = fly_cam.GetPosition();
    const float* src_dir = fly_cam.GetDirection();

    audio_fuente.UpdateListenerPosition(src_pos);
    audio_fuente.UpdateListenerDirection(src_dir);
     
    audio_fuente.ApplyEffects();
   

    audio_manager.Update();
    if (fps_count >= secondsToSpawn) {
        //SpawnBall(physics_engine, entity_comp);
        //ThrowBall(balls_pool, index_pool);
        fps_count -= secondsToSpawn;
        index_pool++;
        if (index_pool >= POOL_SIZE)index_pool = 0;
    }

    UpdateCuadrosColor(time);
    UpdateCubosColor(time);


    //And::TransformComponent* tmp_nose = bolinga_entity->get_component<And::TransformComponent>();
    //tmp_nose->SetRotation(tmp_nose->rotation[0], time, tmp_nose->rotation[2]);

    physics_engine->Simulate(window->get_delta_time() > 1.0f ? 1.0f / 30.0f : window->get_delta_time());

    physics_engine->Apply(entity_comp);
        
    g_renderer->draw_forward(entity_comp);
    //g_renderer->draw_deferred(entity_comp);
    
    frames++;
    time += window->get_delta_time();
    g_renderer->end_frame();
    window->swap_buffers();
  }

  physics_engine->Release(entity_comp);

  return 0;
}