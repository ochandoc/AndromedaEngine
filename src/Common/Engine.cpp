#include "Common/Engine.h"

#include "GLFW/glfw3.h"
#include "Common/Logger.h"

namespace And
{
	Engine::Engine() : m_Initialized(true)
	{
		if (m_Initialized) m_Initialized = (glfwInit() == GLFW_TRUE);
		Logger::InitLogger();
	}

	Engine::~Engine()
	{
		Logger::TerminateLogger();
		if (m_Initialized) glfwTerminate();
	}

	bool Engine::is_initialized() const
	{
		return m_Initialized;
	}
}