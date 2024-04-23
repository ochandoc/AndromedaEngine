#pragma once
#include "Andromeda/Misc/CoreMiscDefines.h"
#include "Andromeda/ECS/ComponentBase.h"
#include <memory>


namespace And {

//struct PhysicsData;

class RigidBodyBase : public ComponentBase{

public:
	RigidBodyBase();
	~RigidBodyBase();
	NON_COPYABLE_CLASS(RigidBodyBase)
	NON_MOVABLE_CLASS(RigidBodyBase)

private:
	//std::shared_ptr<PhysicsData> m_rigidbody_data;
};

}