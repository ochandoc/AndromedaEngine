#pragma once

#include "Andromeda/Misc/CoreMiscDefines.h"
#include "Andromeda/HAL/Types.h"

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
}