#include "Andromeda/Physics/Physics.h"



namespace And{

Physics::Physics() {

}

Physics::~Physics() {

}

Physics::Physics(const Physics& other) {
	this->m_affects_gravity = other.m_affects_gravity;
}


Physics::Physics(Physics&& other) {
	this->m_affects_gravity = other.m_affects_gravity;
}

void Physics::AffectsGravity(bool value)
{
}

void Physics::AddBoxCollider(float x_size, float y_size, float z_size)
{
}

}