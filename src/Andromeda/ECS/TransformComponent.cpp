#include "Andromeda/ECS/Components/TransformComponent.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp >

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
			this->position_offset[i] = other.position_offset[i];
		}
		this->rotation[3] = other.rotation[3];
		this->m_has_rb_ = other.m_has_rb_;
		this->m_matrix = std::make_shared<Mat4>();
		this->m_should_recalculate = true;
		this->m_parent = other.m_parent;
	}

	TransformComponent::TransformComponent(TransformComponent&& other){
		for (int i = 0; i < 3; i++) {
			this->position[i] = other.position[i];
			this->rotation[i] = other.rotation[i];
			this->scale[i] = other.scale[i];
			this->position_offset[i] = other.position_offset[i];
		}
		
		this->rotation[3] = other.rotation[3];
		this->m_has_rb_ = other.m_has_rb_;
		this->m_matrix = std::make_shared<Mat4>();
		this->m_should_recalculate = true;
		this->m_parent = other.m_parent;

		for (int i = 0; i < 3; i++) {
			other.position[i] = 0.0f;
			other.rotation[i] = 0.0f;
			other.scale[i] = 0.0f;;
			other.position_offset[i] = 0.0f;
		}
		other.rotation[3] = 0.0f;
		other.m_has_rb_ = false;
		other.m_matrix = 0;
		other.m_should_recalculate = false;
		other.m_parent = nullptr;
	}
	

	TransformComponent TransformComponent::operator=(const TransformComponent& other)
	{
		//this->m_matrix = other.m_matrix;
		for (int i = 0; i < 3; i++) {
			this->position[i] = other.position[i];
			this->rotation[i] = other.rotation[i];
			this->scale[i] = other.scale[i];
			this->position_offset[i] = other.position_offset[i];
		}
		this->rotation[3] = other.rotation[3];
		this->m_has_rb_ = other.m_has_rb_;
		this->m_matrix = std::make_shared<Mat4>();
		this->m_should_recalculate = true;
		this->m_parent = other.m_parent;
		return *this;
	}

	TransformComponent TransformComponent::operator=(TransformComponent&& other){
		float tmp;
		for (int i = 0; i < 3; i++) {
			tmp = this->position[i];
			this->position[i] = other.position[i];
			other.position[i] = tmp;

			tmp = this->rotation[i];
			this->rotation[i] = other.rotation[i];
			other.rotation[i] = tmp;

			tmp = this->scale[i];
			this->scale[i] = other.scale[i];
			other.scale[i] = tmp;

			tmp = this->position_offset[i];
			this->position_offset[i] = other.position_offset[i];
			other.position_offset[i] = tmp;
		}

		tmp = this->rotation[3];
		this->rotation[3] = other.rotation[3];
		other.rotation[3] = tmp;

		bool b_tmp = this->m_has_rb_;
		this->m_has_rb_ = other.m_has_rb_;
		other.m_has_rb_ = b_tmp;

		this->m_matrix = std::make_shared<Mat4>();
		this->m_should_recalculate = true;

		TransformComponent* tr_tmp = this->m_parent;
		this->m_parent = other.m_parent;
		other.m_parent = tr_tmp;

		return *this;
		
	}

	float* TransformComponent::GetModelMatrix() {

		if (m_should_recalculate) {
			m_matrix->model = glm::mat4(1.0f);

			glm::vec3 objPosition = glm::vec3(position[0] + position_offset[0], position[1] + position_offset[1], position[2] + position_offset[2]);
			glm::vec3 objScale = glm::vec3(scale[0], scale[1], scale[2]);

			m_matrix->model = glm::mat4(1.0f);
			m_matrix->model = glm::translate(m_matrix->model, objPosition);

			if (m_has_rb_) {
				glm::quat rotationQuat(rotation[3], rotation[0], rotation[1], rotation[2]);
				glm::mat4 rotationMatrix = glm::mat4_cast(rotationQuat);
				m_matrix->model = m_matrix->model * rotationMatrix;
			}else {
				//m_matrix->model = glm::translate(m_matrix->model, objPosition);
				m_matrix->model = glm::rotate(m_matrix->model, rotation[2], glm::vec3(0.0f, 0.0f, 1.0f));
				m_matrix->model = glm::rotate(m_matrix->model, rotation[1], glm::vec3(0.0f, 1.0f, 0.0f));
				m_matrix->model = glm::rotate(m_matrix->model, rotation[0], glm::vec3(1.0f, 0.0f, 0.0f));
			}

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

	void TransformComponent::SetPositionOffet(float* offset){
		for (int i = 0; i < 3; i++) {
			position_offset[i] = offset[i];
		}
	}

	void TransformComponent::SetPositionOffet(float x, float y, float z){
		
		position_offset[0] = x;
		position_offset[1] = y;
		position_offset[2] = z;
		
	}

	void TransformComponent::Reset(){
		m_should_recalculate = true;
	}

}