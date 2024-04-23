#pragma once
#include "Andromeda/Misc/CoreMiscDefines.h"
#include "Andromeda/Physics/RigidBody.h"
#include "Andromeda/ECS/EntityComponentSystem.h"


namespace And {

struct PhysicsEngineData;

class PhysicsEngine final{


    public:

    static std::shared_ptr<PhysicsEngine> Init(unsigned int substeps = 1);
    ~PhysicsEngine();
    PhysicsEngine(PhysicsEngine&&);
    NON_COPYABLE_CLASS(PhysicsEngine)


    void Simulate(double dt, bool fetch = true);
    void Apply(EntityComponentSystem& ecs);
    void Release(EntityComponentSystem& ecs);

    void SetGravity(float x, float y, float z);
    void SetObjectLenght(unsigned int l);
    void SetSpeedScale(unsigned int speed);

    void GetError();

    
    RigidBody CreateRigidBody();

    std::shared_ptr<PhysicsEngineData> m_physics_data;
    private:
    
    PhysicsEngine();
};
}