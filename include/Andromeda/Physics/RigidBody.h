#pragma once
#include "RigidBodyBase.h"
//#include "PxPhysicsAPI.h"
//#include "Andromeda/Physics/PhysicsEngine.h"
#include "Andromeda/Misc/CoreMiscDefines.h"

namespace And {


	struct PhysicsData;

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