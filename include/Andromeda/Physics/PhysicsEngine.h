#pragma once
#include "Andromeda/Misc/CoreMiscDefines.h"


namespace And {

struct PhysicsEngineData;

class PhysicsEngine final{


    public:

    std::shared_ptr<PhysicsEngine> Init();
    ~PhysicsEngine();
    NON_COPYABLE_CLASS(PhysicsEngine)
        NON_MOVABLE_CLASS(PhysicsEngine)

    void Simulate(double dt, bool fetch = true);
    void SetGravity(float x, float y, float z);
    void SetObjectLenght(unsigned int l);
    void SetSpeedScale(unsigned int speed);

    void GetError();

    private:
    
    PhysicsEngine();
    std::shared_ptr<PhysicsEngineData> m_physics_data;
};
}