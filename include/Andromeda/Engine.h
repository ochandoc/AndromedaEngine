#pragma once

#include "Andromeda/Misc/CoreMiscDefines.h"
#include "Andromeda/HAL/Types.h"
#include "Andromeda/ECS/ComponentBase.h"
#include "Andromeda/ECS/Components/MeshComponent.h"
#include "Andromeda/ECS/Components/TransformComponent.h"
#include "Andromeda/Graphics/Lights/DirectionalLight.h"
#include "Andromeda/Graphics/Lights/SpotLight.h"
#include "Andromeda/Graphics/Lights/PointLight.h"
#include "Andromeda/ECS/EntityComponentSystem.h"

namespace And
{
	class Engine final
	{		
		NON_COPYABLE_CLASS(Engine)
		NON_MOVABLE_CLASS(Engine)
	public:
		Engine();
		~Engine();

		bool is_initialized() const;

		friend class ResourceManager;
		friend class JobSystem;
	private:
		bool m_Initialized;
	};

	static void AddBasicComponents(EntityComponentSystem& e){
		e.add_component_class<And::MeshComponent>();
		e.add_component_class<And::TransformComponent>();
		e.add_component_class<And::SpotLight>();
		e.add_component_class<And::DirectionalLight>();
		e.add_component_class<And::PointLight>();
	}
}