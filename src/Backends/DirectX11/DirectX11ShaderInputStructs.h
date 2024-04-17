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
				glm::vec3 CameraPos;
				uint32 HasColorTxture;
			};

			struct LightData
			{
				glm::vec3 Position; // S-P
				float ConstantAttenuation; // S-P
				glm::vec3 Direction; // S-D
				float LinearAttenuation; // S-P
				glm::vec3 DiffuseColor; // S-P-D
				float QuadraticAttenuation; // S-P
				float SpecularStrength; // S-P-D
				float SpecularShininess; // S-P-D
				float InnerConeAngle; // S
				float OuterConeAngle; // S
				//float AmbientStrength; // A
			};
		}
	}

}