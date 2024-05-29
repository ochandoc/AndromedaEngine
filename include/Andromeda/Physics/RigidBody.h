#pragma once
#include "RigidBodyBase.h"
//#include "PxPhysicsAPI.h"
//#include "Andromeda/Physics/PhysicsEngine.h"
#include "Andromeda/Misc/CoreMiscDefines.h"

namespace And {


	struct PhysicsData;
	/**
	 * @enum ForceMode
	 * @brief Enumeration for different force modes.
	 */
	enum class ForceMode {
		FORCE = 1,				//!< parameter has unit of mass * distance/ time^2, i.e. a force
		IMPULSE = 2,			//!< parameter has unit of mass * distance /time
		VELOCITY_CHANGE = 3,	//!< parameter has unit of distance / time, i.e. the effect is mass independent: a velocity change.
		ACCELERATION = 4		//!< parameter has unit of distance/ time^2, i.e. an acceleration. It gets treated just like a force except the mass is not divided out before integration.
	};
	/**
	 * @enum ColliderType
	 * @brief Enumeration for different types of colliders.
	 */
	enum class ColliderType {
		None = 0,
		RigidDynamic = 1,
		RigidStatic = 2
	};

	/**
	 * @class RigidBody
	 * @brief Represents a rigid body for physics simulation.
	 *
	 * This class provides functionality to create and manipulate rigid bodies for physics simulation.
	 */
class RigidBody : public RigidBodyBase {

public:
	//RigidBody(std::shared_ptr<RigidBodyEngine> e);
	RigidBody();
	~RigidBody();
	RigidBody(RigidBody&&);
	RigidBody(const RigidBody&);
	RigidBody operator=(const RigidBody&);

	// Collider creation methods
	void AddBoxCollider(const float* position, const float* scale, ColliderType type, float static_friction = 0.5f, float dinamic_friction = 0.5f, float restitution = 0.1f);
	void AddSphereCollider(const float* position, const float radius, ColliderType type, float static_friction = 0.5f, float dinamic_friction = 0.5f, float restitution = 0.1f);

	// Physics properties setters
	void AffectsGravity(bool value);
	void SetMass(float mass);

	// Force application methods
	void AddForce(const float x, const float y, const float z, ForceMode fmod = ForceMode::FORCE);
	void AddForce(const float* direction, ForceMode fmod);

	// Transformation methods
	void SetPosition(float* pos, bool Awake = true);
	void SetPosition(float x, float y, float z, bool Awake = true);

	void SetRotation(float x, float y, float z, bool Awake = true);

	// Information retrieval methods
	void GetPosition(float* position);
	void GetRotation(float* rotation);
	void GetPositionRotation(float* position, float* rotation);

	// Sleeping and waking methods
	void Sleep(); ///< Put the rigid body to sleep.
	void WakeUp(); ///< Wake up the rigid body.

	// Release method
	void Release(); ///< Release the rigid body resources.
	friend class PhysicsEngine;
private:
	bool m_affects_gravity; ///< Flag indicating whether the rigid body is affected by gravity.

	std::shared_ptr<PhysicsData> m_data; ///< Pointer to the physics data.

	ColliderType m_collider_type = ColliderType::None;  ///< Type of collider associated with the rigid body.

};

}