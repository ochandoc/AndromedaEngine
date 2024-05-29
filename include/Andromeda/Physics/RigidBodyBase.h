#pragma once
#include "Andromeda/Misc/CoreMiscDefines.h"
#include "Andromeda/ECS/ComponentBase.h"
#include <memory>


namespace And {

//struct PhysicsData;
	/**
 * @class RigidBodyBase
 * @brief Base class for rigid body components.
 *
 * This class serves as the base class for all rigid body components in the ECS system.
 */
class RigidBodyBase : public ComponentBase{

public:
	/**
	* @brief Default constructor.
	*/
	RigidBodyBase();

	/**
	 * @brief Destructor.
	 */
	~RigidBodyBase();
	NON_COPYABLE_CLASS(RigidBodyBase)
	NON_MOVABLE_CLASS(RigidBodyBase)

private:
	//std::shared_ptr<PhysicsData> m_rigidbody_data;
};

}