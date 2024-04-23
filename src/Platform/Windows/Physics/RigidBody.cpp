#include "Andromeda/Physics/RigidBody.h"
#include "PxPhysicsAPI.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace And{


	struct PhysicsData {
		physx::PxPhysics* physics;
		physx::PxRigidDynamic* actor;
		physx::PxScene* scene;
	};

RigidBody::RigidBody() : m_data(){
}

RigidBody::~RigidBody() {

}

RigidBody::RigidBody(const RigidBody& other) {
	
	this->m_data = other.m_data;
	/*this->m_affects_gravity = other.m_affects_gravity;
	this->m_actor = other.m_actor;
	this->m_physics = other.m_physics;
	this->m_scene = other.m_scene;*/
}

RigidBody RigidBody::operator=(const RigidBody& other){
	this->m_data = other.m_data;
	/*this->m_actor = other.m_actor;
	this->m_physics = other.m_physics;
	this->m_scene = other.m_scene;*/
	return *this;
}


RigidBody::RigidBody(RigidBody&& other) {
	this->m_data = other.m_data;
	/*this->m_actor = other.m_actor;
	this->m_physics = other.m_physics;
	this->m_scene = other.m_scene;*/
}

void RigidBody::AffectsGravity(bool value)
{
}

void RigidBody::AddBoxCollider(float* position, float* scale){

	physx::PxTransform transform(physx::PxVec3(0.0f));
	transform.p = physx::PxVec3(position[0], position[1], position[2]);
	m_data->actor = m_data->physics->createRigidDynamic(transform);
	
	physx::PxBoxGeometry box(scale[0], scale[1], scale[2]);
	physx::PxMaterial* mat = m_data->physics->createMaterial(0.5f, 0.5f, 0.0f); // static friction, dynamic friction, restitution);
	physx::PxShape* boxShape = m_data->physics->createShape(box, *mat, true);
	//boxShape->setLocalPose(physx::PxTransform(0.0f, 0.0f, 0.0f));
	//boxShape->setLocalPose(transform);

	m_data->actor->attachShape(*boxShape);
	m_data->actor->setGlobalPose(transform);
	m_data->actor->getGlobalPose();
	m_data->actor->setMass(1.0f);
	m_data->scene->addActor(*(m_data->actor));
}

void RigidBody::Release(){
	m_data->actor->release();
}

void RigidBody::GetPosition(float* position){

	physx::PxTransform transform = m_data->actor->getGlobalPose();
	position[0] = transform.p.x;
	position[1] = transform.p.y;
	position[2] = transform.p.z;
}

void RigidBody::GetRotation(float* rotation)
{
	
	physx::PxTransform transform = m_data->actor->getGlobalPose();
	rotation[0] = transform.q.x;
	rotation[1] = transform.q.y;
	rotation[2] = transform.q.z;
}

void RigidBody::GetPositionRotation(float* position, float* rotation){

	if (m_data->actor) {
		printf("Hay actor\n");
	}
	physx::PxTransform transform = m_data->actor->getGlobalPose();
	rotation[0] = transform.q.x;
	rotation[1] = transform.q.y;
	rotation[2] = transform.q.z;

	position[0] = transform.p.x;
	position[1] = transform.p.y;
	position[2] = transform.p.z;
}

}