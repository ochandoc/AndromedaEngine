#include "Andromeda/ECS/Components/TransformComponent.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace And {

	struct Mat4 {
		glm::mat4 model;
	};

	TransformComponent::TransformComponent() : m_matrix(){
		m_matrix = std::make_shared<Mat4>();

	}

	TransformComponent::~TransformComponent(){}

	TransformComponent::TransformComponent(const TransformComponent& other) {
		for (int i = 0; i < 3; i++) {
			this->position[i] = other.position[i];
			this->rotation[i] = other.rotation[i];
			this->scale[i] = other.scale[i];
		}
		this->m_matrix = std::make_shared<Mat4>();
		this->m_should_recalculate = true;
	}

	TransformComponent::TransformComponent(TransformComponent&& other){
		for (int i = 0; i < 3; i++) {
			this->position[i] = other.position[i];
			this->rotation[i] = other.rotation[i];
			this->scale[i] = other.scale[i];
		}
		this->m_matrix = std::make_shared<Mat4>();
		this->m_should_recalculate = true;
	}
	

	TransformComponent TransformComponent::operator=(const TransformComponent& other)
	{
		//this->m_matrix = other.m_matrix;
		for (int i = 0; i < 3; i++) {
			this->position[i] = other.position[i];
			this->rotation[i] = other.rotation[i];
			this->scale[i] = other.scale[i];
		}
		this->m_matrix = std::make_shared<Mat4>();
		this->m_should_recalculate = true;
		return *this;
	}

	float* TransformComponent::GetModelMatrix() {

		if (m_should_recalculate) {
			m_matrix->model = glm::mat4(1.0f);

			glm::vec3 objPosition = glm::vec3(position[0], position[1], position[2]);
			glm::vec3 objScale = glm::vec3(scale[0], scale[1], scale[2]);

			m_matrix->model = glm::translate(m_matrix->model, objPosition);
			m_matrix->model = glm::rotate(m_matrix->model, rotation[0], glm::vec3(1.0f, 0.0f, 0.0f));
			m_matrix->model = glm::rotate(m_matrix->model, rotation[1], glm::vec3(0.0f, 1.0f, 0.0f));
			m_matrix->model = glm::rotate(m_matrix->model, rotation[2], glm::vec3(0.0f, 0.0f, 1.0f));
			m_matrix->model = glm::scale(m_matrix->model, objScale);

			if (m_parent) {
				m_matrix->model = glm::make_mat4(m_parent->GetModelMatrix()) * m_matrix->model;
			}

			
			m_should_recalculate = false;

		}
		
		return glm::value_ptr(m_matrix->model);


	}
	
	void TransformComponent::SetParent(TransformComponent* parent) {
		m_parent = parent;
	}

	void TransformComponent::SetPosition(float* p) {

		position[0] = p[0];
		position[1] = p[1];
		position[2] = p[2];

		m_should_recalculate = true;
	}

	void TransformComponent::SetPosition(float x, float y, float z){
		position[0] = x;
		position[1] = y;
		position[2] = z;

		m_should_recalculate = true;
	}

	void TransformComponent::SetRotation(float* r){

		rotation[0] = r[0];
		rotation[1] = r[1];
		rotation[2] = r[2];

		m_should_recalculate = true;
	}
	
	void TransformComponent::SetRotation(float x, float y, float z){

		rotation[0] = x;
		rotation[1] = y;
		rotation[2] = z;

		m_should_recalculate = true;
	}

	void TransformComponent::SetScale(float* s){

		scale[0] = s[0];
		scale[1] = s[1];
		scale[2] = s[2];

		m_should_recalculate = true;
	}
	
	void TransformComponent::SetScale(float x, float y, float z){

		scale[0] = x;
		scale[1] = y;
		scale[2] = z;

		m_should_recalculate = true;
	}

	void TransformComponent::Reset(){
		m_should_recalculate = true;
	}

}