#include "Common/Engine.h"

#include "Common/JobSystem.h"

#include "GLFW/glfw3.h"

namespace And
{
	Engine::Engine() : m_Initialized(true)
	{
		if (m_Initialized) m_Initialized = (glfwInit() == GLFW_TRUE);
		m_ThreadsData = std::make_shared<ThreadsData>();
	}

	Engine::~Engine()
	{
		if (m_Initialized) glfwTerminate();
	}

	bool Engine::is_initialized() const
	{
		return m_Initialized;
	}
}