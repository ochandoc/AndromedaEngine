#include "Andromeda/Engine.h"

#include "GLFW/glfw3.h"

namespace And
{
	Engine::Engine() : m_Initialized(true)
	{
		if (m_Initialized) m_Initialized = (glfwInit() == GLFW_TRUE);
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