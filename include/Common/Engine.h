#pragma once

#include "base.h"

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

	private:
		bool m_Initialized;
	};
}