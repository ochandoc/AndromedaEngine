#pragma once
#include "Andromeda/Misc/CoreMiscDefines.h"
#include <memory>


namespace And {

struct PhysicsData;

class PhysicsBase {

public:
	PhysicsBase();
	~PhysicsBase();
	NON_COPYABLE_CLASS(PhysicsBase)
	NON_MOVABLE_CLASS(PhysicsBase)

private:
	std::shared_ptr<PhysicsData> m_physics_data;
};

}