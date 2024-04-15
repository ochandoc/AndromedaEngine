#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace And
{
	namespace DirectX11
	{
		namespace VertexShader
		{
			struct ObjectData
			{
				glm::mat4 view;
				glm::mat4 projection;
				glm::mat4 model;
			};
		}
		namespace PixelShader
		{
			struct ObjectData
			{
				glm::vec4 Color;
				uint32 HasColorTxture;
				uint8 PADDING[12];
			};
		}
	}

}