#include "Andromeda/Physics/RigidBody.h"
#include "PxPhysicsAPI.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp >


namespace And{


	struct PhysicsData {
		physx::PxPhysics* physics;
		
		physx::PxRigidActor* actor;
		//physx::PxRigidDynamic* actor;
		//physx::PxRigidStatic* actor_static;
		physx::PxScene* scene;
	};

RigidBody::RigidBody() : m_data(){
}

RigidBody::~RigidBody() {

}

RigidBody::RigidBody(const RigidBody& other) {
	
	this->m_data = other.m_data;
	this->m_collider_type = other.m_collider_type;
	this->m_affects_gravity = other.m_affects_gravity;
}

RigidBody RigidBody::operator=(const RigidBody& other){
	this->m_data = other.m_data;
	this->m_collider_type = other.m_collider_type;
	this->m_affects_gravity = other.m_affects_gravity;

	return *this;
}


RigidBody::RigidBody(RigidBody&& other) {
	this->m_data = other.m_data;
	this->m_collider_type = other.m_collider_type;
	this->m_affects_gravity = other.m_affects_gravity;
}

void RigidBody::AffectsGravity(bool value){
	// Por algun motivo esto va al reves
	m_data->actor->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, !value);
}

void RigidBody::AddBoxCollider(const float* position, const float* scale, ColliderType type, float static_friction, float dinamic_friction, float restitution){

	physx::PxTransform transform(physx::PxVec3(0.0f));
	transform.p = physx::PxVec3(position[0], position[1], position[2]);
	transform.q = physx::PxQuat(1.0f);

	switch (type) {
		case ColliderType::RigidDynamic:m_data->actor = m_data->physics->createRigidDynamic(transform);m_collider_type = ColliderType::RigidDynamic; break;
		case ColliderType::RigidStatic: m_data->actor = m_data->physics->createRigidStatic(transform); m_collider_type = ColliderType::RigidStatic; break;
	}
	
	assert(m_data->actor, "Creating box collider | Actor is null");
			
	
	
	physx::PxBoxGeometry box(scale[0] * 0.5f, scale[1] * 0.5f, scale[2] * 0.5f);
	physx::PxMaterial* mat = m_data->physics->createMaterial(static_friction, dinamic_friction, restitution); // static friction, dynamic friction, restitution;
	physx::PxShape* shape = m_data->physics->createShape(box, *mat, false); // is exclusive = false
	assert(shape);
	m_collider_type = type;
	//boxShape->setLocalPose(physx::PxTransform(0.0f, 0.0f, 0.0f));
	//boxShape->setLocalPose(transform);

	m_data->actor->attachShape(*shape);
	m_data->actor->setGlobalPose(transform);
	m_data->actor->getGlobalPose();
	if (m_collider_type == ColliderType::RigidDynamic) {
		physx::PxRigidDynamic* tmp = static_cast<physx::PxRigidDynamic*>(m_data->actor);
		tmp->setMass(0.0f);
	}
	m_data->scene->addActor(*(m_data->actor));
}

void RigidBody::AddSphereCollider(const float* position, const float radius, ColliderType type, float static_friction, float dinamic_friction, float restitution){

	physx::PxTransform transform(physx::PxVec3(0.0f));
	transform.p = physx::PxVec3(position[0], position[1], position[2]);
	
	switch (type) {
		case ColliderType::RigidDynamic: m_data->actor = m_data->physics->createRigidDynamic(transform); m_collider_type = ColliderType::RigidDynamic; break;
		case ColliderType::RigidStatic: m_data->actor = m_data->physics->createRigidStatic(transform); m_collider_type = ColliderType::RigidStatic; break;
	}
	assert(m_data->actor, "Creating sphere collider | Actor is null");
	m_collider_type = type;


	float r = radius;

	physx::PxSphereGeometry sphere(r);

	// Static fricction -> Force to start moving an object
	// Dynamic friction -> Force against a object in movement
	// Restitution -> Bounce level

	physx::PxMaterial* mat = m_data->physics->createMaterial(static_friction, dinamic_friction, restitution); // static friction, dynamic friction, restitution);
	physx::PxShape* shape = m_data->physics->createShape(sphere, *mat, true);
	assert(shape);

	m_data->actor->attachShape(*shape);
	m_data->actor->setGlobalPose(transform);
	m_data->actor->getGlobalPose();
	if (m_collider_type == ColliderType::RigidDynamic) {
		physx::PxRigidDynamic* tmp = static_cast<physx::PxRigidDynamic*>(m_data->actor);
		tmp->setMass(0.0f);
	}
	m_data->scene->addActor(*(m_data->actor));
}

void RigidBody::Release(){
	m_data->actor->release();
}

void RigidBody::SetMass(float mass) {
	if (m_collider_type == ColliderType::RigidDynamic) {

		physx::PxRigidDynamic* tmp = static_cast<physx::PxRigidDynamic*>(m_data->actor);
		if (tmp) {
			tmp->setMass(mass);
		}
	}
}

void RigidBody::AddForce(const float x, const float y, const float z, ForceMode fmod){

	physx::PxForceMode::Enum f;

	switch (fmod) {
		case ForceMode::ACCELERATION: f = physx::PxForceMode::eACCELERATION; break;
		case ForceMode::FORCE: f = physx::PxForceMode::eFORCE; break;
		case ForceMode::IMPULSE: f = physx::PxForceMode::eIMPULSE; break;
		case ForceMode::VELOCITY_CHANGE: f = physx::PxForceMode::eVELOCITY_CHANGE; break;
	}

	if (m_collider_type == ColliderType::RigidDynamic) {
		physx::PxRigidDynamic* tmp = static_cast<physx::PxRigidDynamic*>(m_data->actor);	
		if (tmp){
			tmp->addForce(physx::PxVec3(x,y,z), f);
		}
	}
	
}

void RigidBody::AddForce(const float* direction, ForceMode fmod){
	physx::PxForceMode::Enum f;

	switch (fmod) {
		case ForceMode::ACCELERATION: f = physx::PxForceMode::eACCELERATION; break;
		case ForceMode::FORCE: f = physx::PxForceMode::eFORCE; break;
		case ForceMode::IMPULSE: f = physx::PxForceMode::eIMPULSE; break;
		case ForceMode::VELOCITY_CHANGE: f = physx::PxForceMode::eVELOCITY_CHANGE; break;
	}

	if (m_collider_type == ColliderType::RigidDynamic) {
		physx::PxRigidDynamic* tmp = static_cast<physx::PxRigidDynamic*>(m_data->actor);
		if (tmp) {
			tmp->addForce(physx::PxVec3(direction[0], direction[1], direction[2]), f);
		}
	}
}

void RigidBody::SetPosition(float* pos, bool Awake){

	physx::PxTransform tr{ pos[0], pos[1], pos[2]};
	m_data->actor->setGlobalPose(tr, Awake);
}

void RigidBody::SetPosition(float x, float y, float z, bool Awake){

	physx::PxTransform tr{ x, y, z };
	m_data->actor->setGlobalPose(tr, Awake);
}

void RigidBody::SetRotation(float x, float y, float z, bool Awake) {
	
	glm::vec3 eulerAngles(glm::radians(x), glm::radians(y), glm::radians(z));
	glm::quat quaternion = glm::quat(eulerAngles);
	
	physx::PxTransform tr = m_data->actor->getGlobalPose();

	tr.q.x = quaternion.x;
	tr.q.y = quaternion.y;
	tr.q.z = quaternion.z;
	tr.q.w = quaternion.w;


	m_data->actor->setGlobalPose(tr, Awake);
	
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

	
	physx::PxTransform transform = m_data->actor->getGlobalPose();

	//glm::quat glmQuatRotation(transform.q.w, transform.q.x, transform.q.y, transform.q.z);
	//glm::vec3 eulerRotation = glm::eulerAngles(glmQuatRotation);

	//float angle;
	//physx::PxVec3 axis;
	//transform.q.toRadiansAndUnitAxis(angle, axis);

	//physx::PxVec3 eulerRotation = axis * angle;

	//glm::quat q(transform.q.w, transform.q.x, transform.q.y, transform.q.z);
	//glm::vec3 euler = glm::eulerAngles(q);// *3.14159f / 180.f; // eulerAngleYXZ takes radians but eulerAngles returns degrees
	
	//rotation[0] = euler[0];
	//rotation[1] = euler[1];
	//rotation[2] = euler[2];

	//rotation[0] = axis[0];
	//rotation[1] = axis[1];
	//rotation[2] = axis[2];
	//rotation[3] = angle;

	rotation[0] = transform.q.x;
	rotation[1] = transform.q.y;
	rotation[2] = transform.q.z;
	rotation[3] = transform.q.w;

	position[0] = transform.p.x;
	position[1] = transform.p.y;
	position[2] = transform.p.z;
}

void RigidBody::Sleep(){

	if (m_collider_type == ColliderType::RigidDynamic) {
		physx::PxRigidDynamic* tmp = static_cast<physx::PxRigidDynamic*>(m_data->actor);
		if (tmp) {
			tmp->putToSleep();
		}
	}
}

void RigidBody::WakeUp(){
	if (m_collider_type == ColliderType::RigidDynamic) {
		physx::PxRigidDynamic* tmp = static_cast<physx::PxRigidDynamic*>(m_data->actor);
		if (tmp) {
			tmp->wakeUp();
		}
	}
}

}