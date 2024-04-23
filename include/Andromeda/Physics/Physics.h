#pragma once
#include "PhysicsBase.h"
#include "Andromeda/Physics/PhysicsEngine.h"
#include "Andromeda/Misc/CoreMiscDefines.h"

namespace And {


class Physics : public PhysicsBase {

public:
	//Physics(std::shared_ptr<PhysicsEngine> e);
	Physics();
	~Physics();
	Physics(Physics&&);
	Physics(const Physics&);
	


	void AffectsGravity(bool value);
	void AddBoxCollider(float x_size, float y_size, float z_size);

private:
	bool m_affects_gravity;
	std::shared_ptr<PhysicsEngine> m_engine;
	

};

}