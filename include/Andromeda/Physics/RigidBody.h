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

class RigidBody : public RigidBodyBase {

public:
	//RigidBody(std::shared_ptr<RigidBodyEngine> e);
	RigidBody();
	~RigidBody();
	RigidBody(RigidBody&&);
	RigidBody(const RigidBody&);
	RigidBody operator=(const RigidBody&);

	void AddBoxCollider(float* position, float* scale);

	void AffectsGravity(bool value);
	void SetMass(float mass);

	void AddForce(const float x, const float y, const float z, ForceMode fmod = ForceMode::FORCE);
	void AddForce(const float* direction, ForceMode fmod);


	void GetPosition(float* position);
	void GetRotation(float* rotation);
	void GetPositionRotation(float* position, float* rotation);

	void Release();
	friend class PhysicsEngine;
private:
	bool m_affects_gravity;

	std::shared_ptr<PhysicsData> m_data;

};

}