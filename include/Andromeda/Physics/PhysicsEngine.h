#pragma once
#include "Andromeda/Misc/CoreMiscDefines.h"
#include "Andromeda/Physics/RigidBody.h"
#include "Andromeda/ECS/EntityComponentSystem.h"


namespace And {

struct PhysicsEngineData;

/**
 * @class PhysicsEngine
 * @brief Manages the physics simulation.
 *
 * This class provides functionality to initialize, simulate, and manage the physics engine.
 */
class PhysicsEngine final{


    public:
    /**
     * @brief Creates Physics engine
     * 
     * @param substeps number of iterations per frame to calculate phisics
     * @return std::shared_ptr<PhysicsEngine> 
     */
    static std::shared_ptr<PhysicsEngine> Init(bool executeOnGPU = false, unsigned int substeps = 8192, bool create_plane = false);
    /// Destructor.
    ~PhysicsEngine();
    /// Move constructor.
    PhysicsEngine(PhysicsEngine&&);
    /// Move constructor.
    NON_COPYABLE_CLASS(PhysicsEngine)

    /**
     * @brief Simulates the physics for a given time step.
     *
     * @param dt The time step for the physics simulation.
     * @param fetch Flag indicating whether to fetch simulation results.
     */
    void Simulate(float dt, bool fetch = true);
    /**
     * @brief Applies physics to entities in the ECS.
     *
     * @param ecs The EntityComponentSystem to apply physics to.
     */
    void Apply(EntityComponentSystem& ecs);
    /**
     * @brief Releases physics resources associated with entities in the ECS.
     *
     * @param ecs The EntityComponentSystem from which to release physics resources.
     */
    void Release(EntityComponentSystem& ecs);

    /**
      * @brief Sets the gravity vector.
      *
      * @param x The x-component of the gravity vector.
      * @param y The y-component of the gravity vector.
      * @param z The z-component of the gravity vector.
      */
    void SetGravity(float x, float y, float z);
    /**
     * @brief Sets the length of physics objects.
     *
     * @param l The length of physics objects.
     */
    void SetObjectLenght(unsigned int l);
    /**
     * @brief Sets the speed scale for physics simulation.
     *
     * @param speed The speed scale for physics simulation.
     */
    void SetSpeedScale(unsigned int speed);
    /**
     * @brief Retrieves the error status of the physics engine.
     */
    void GetError();

    /**
     * @brief Creates a new rigid body for the physics simulation.
     *
     * @return RigidBody The created rigid body.
     */
    RigidBody CreateRigidBody();

    std::shared_ptr<PhysicsEngineData> m_physics_data; /**< Pointer to the physics engine data. */
    private:
    /// Default constructor.
    PhysicsEngine();
};
}