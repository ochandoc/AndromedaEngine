#pragma once
#include "RigidBodyBase.h"
//#include "PxPhysicsAPI.h"
//#include "Andromeda/Physics/PhysicsEngine.h"
#include "Andromeda/Misc/CoreMiscDefines.h"

namespace And {


	struct PhysicsData;

	enum class ForceMode {
		FORCE = 1,				//!< parameter has unit of mass * distance/ time^2, i.e. a force
		IMPULSE = 2,			//!< parameter has unit of mass * distance /time
		VELOCITY_CHANGE = 3,	//!< parameter has unit of distance / time, i.e. the effect is mass independent: a velocity change.
		ACCELERATION = 4		//!< parameter has unit of distance/ time^2, i.e. an acceleration. It gets treated just like a force except the mass is not divided out before integration.
	};

	enum class ColliderType {
		None = 0,
		RigidDynamic = 1,
		RigidStatic = 2
	};

class RigidBody : public RigidBodyBase {

public:
	//RigidBody(std::shared_ptr<RigidBodyEngine> e);
	RigidBody();
	~RigidBody();
	RigidBody(RigidBody&&);
	RigidBody(const RigidBody&);
	RigidBody operator=(const RigidBody&);

	void AddBoxCollider(const float* position, const float* scale, ColliderType type, float static_friction = 0.5f, float dinamic_friction = 0.5f, float restitution = 0.1f);
	void AddSphereCollider(const float* position, const float radius, ColliderType type, float static_friction = 0.5f, float dinamic_friction = 0.5f, float restitution = 0.1f);

	void AffectsGravity(bool value);
	void SetMass(float mass);

	void AddForce(const float x, const float y, const float z, ForceMode fmod = ForceMode::FORCE);
	void AddForce(const float* direction, ForceMode fmod);

	void SetPosition(float* pos);
	void SetPosition(float x, float y, float z);


	void GetPosition(float* position);
	void GetRotation(float* rotation);
	void GetPositionRotation(float* position, float* rotation);

	void Sleep();
	void WakeUp();

	void Release();
	friend class PhysicsEngine;
private:
	bool m_affects_gravity;

	std::shared_ptr<PhysicsData> m_data;

	ColliderType m_collider_type = ColliderType::None;

};

}