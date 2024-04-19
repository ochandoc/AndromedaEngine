#include "Andromeda/ECS/Components/TransformComponent.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace And {

	float* TransformComponent::GetModelMatrix() {

		glm::mat4 modelMatrix = glm::mat4(1.0f);
		//if (m_should_recalculate) {

			glm::vec3 objPosition = glm::vec3(position[0], position[1], position[2]);
			glm::vec3 objScale = glm::vec3(scale[0], scale[1], scale[2]);
			float rotationAngle = 0.0f;
			glm::vec3 objRotationAxis = glm::vec3(rotation[0], rotation[1], rotation[2]);

			modelMatrix = glm::translate(modelMatrix, objPosition);
			//modelMatrix = glm::rotate(modelMatrix, rotationAngle, objRotationAxis);
			modelMatrix = glm::rotate(modelMatrix, rotation[0], glm::vec3(1.0f, 0.0f, 0.0f));
			modelMatrix = glm::rotate(modelMatrix, rotation[1], glm::vec3(0.0f, 1.0f, 0.0f));
			modelMatrix = glm::rotate(modelMatrix, rotation[2], glm::vec3(0.0f, 0.0f, 1.0f));
			modelMatrix = glm::scale(modelMatrix, objScale);

			if (m_parent) {
				//modelMatrix = modelMatrix * glm::make_mat4(m_parent->GetModelMatrix());
			}

			m_model_matrix = glm::value_ptr(modelMatrix);
			m_should_recalculate = false;

		//}
		
	//return
		return m_model_matrix;


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

}