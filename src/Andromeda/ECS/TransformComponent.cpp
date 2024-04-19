#include "Andromeda/ECS/Components/TransformComponent.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace And {

	float* TransformComponent::GetModelMatrix() {

		glm::mat4 modelMatrix = glm::mat4(1.0f);
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

		return glm::value_ptr(modelMatrix);


	}
	void TransformComponent::SetParent(TransformComponent* parent) {
		m_parent = parent;
	}

}