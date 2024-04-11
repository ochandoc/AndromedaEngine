#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace And
{
	namespace DirectX11InputStruct
	{
		struct Object
		{
			glm::mat4 view;
			glm::mat4 projection;
			glm::mat4 model;
		};
	}
}