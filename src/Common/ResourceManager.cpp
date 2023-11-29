#include "Common/ResourceManager.h"

#include "Common/Window.h"

#include "Backends/OpenGL/OpenGL.h"
#include "GLFW/glfw3.h"

DEFINE_LOG_CATEGORY(ResourceManagerLog)

namespace And
{

static void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message,
	const void* userParam)
{

	const char* source_ = nullptr;

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:
		source_ = "API";
		break;

	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
		source_ = "WINDOW SYSTEM";
		break;

	case GL_DEBUG_SOURCE_SHADER_COMPILER:
		source_ = "SHADER COMPILER";
		break;

	case GL_DEBUG_SOURCE_THIRD_PARTY:
		source_ = "THIRD PARTY";
		break;

	case GL_DEBUG_SOURCE_APPLICATION:
		source_ = "APPLICATION";
		break;

	case GL_DEBUG_SOURCE_OTHER:
		source_ = "UNKNOWN";
		break;

	default:
		source_ = "UNKNOWN";
		break;
	}

	const char* type_ = nullptr;

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:
		type_ = "ERROR";
		break;

	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		type_ = "DEPRECATED BEHAVIOR";
		break;

	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		type_ = "UDEFINED BEHAVIOR";
		break;

	case GL_DEBUG_TYPE_PORTABILITY:
		type_ = "PORTABILITY";
		break;

	case GL_DEBUG_TYPE_PERFORMANCE:
		type_ = "PERFORMANCE";
		break;

	case GL_DEBUG_TYPE_OTHER:
		type_ = "OTHER";
		break;

	case GL_DEBUG_TYPE_MARKER:
		type_ = "MARKER";
		break;

	default:
		type_ = "UNKNOWN";
		break;
	}

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:
		//_severity = "HIGH";
		AND_LOG(ResourceManagerLog, Info, "{}: {} , raised from {}: {}", id, type_, source_, message);
		break;

	case GL_DEBUG_SEVERITY_MEDIUM:
		AND_LOG(ResourceManagerLog, Warning, "{}: {} , raised from {}: {}", id, type_, source_, message);
		break;

	case GL_DEBUG_SEVERITY_LOW:
		AND_LOG(ResourceManagerLog, Warning, "{}: {} , raised from {}: {}", id, type_, source_, message);
		break;

	case GL_DEBUG_SEVERITY_NOTIFICATION:
		//_severity = "NOTIFICATION";
		AND_LOG(ResourceManagerLog, Debug, "{}: {} , raised from {}: {}", id, type_, source_, message);
		break;

	default:
		//_severity = "UNKNOWN";
		AND_LOG(ResourceManagerLog, Info, "{}: {} , raised from {}: {}", id, type_, source_, message);
		break;
	}
}

  struct ResourceManagerData
  {
    std::unique_ptr<std::thread> thread;
    GLFWwindow* window = nullptr;
  };

  ResourceManager::ResourceManager(Window& w, JobSystem& js) : m_Window(w), m_JobSystem(js), m_Stop(false), m_Data(new ResourceManagerData)
  {
    m_ThreadsData = js.m_ThreadsData;
    GLFWwindow* main_window = (GLFWwindow*)w.get_native_window();

    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
#ifdef AND_OPENGL
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // Version principal de OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);  // Version menor de OpenGL
#   ifdef DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE); // Activar debug
#   endif
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Perfil de OpenGL
#endif
    m_Data->window = glfwCreateWindow(100, 100, "OpenGL", nullptr, main_window);
    m_Data->thread = std::make_unique<std::thread>([this]() {
      glfwMakeContextCurrent(m_Data->window);
			glEnable(GL_DEBUG_OUTPUT);
			glDebugMessageCallback(MessageCallback, 0);
			AND_LOG(ResourceManagerLog, Info, "Load thread created!");
      while (true)
      {
        internal::job j;
        std::function<void()> swap;
        {
          std::unique_lock<std::mutex> lock{ m_ThreadsData->m_ResourceQueueMutex};
          m_ThreadsData->m_ResourceCondition.wait(lock, [this]() {return !m_ThreadsData->m_ResourceJobsQueue.empty() || m_Stop; });
          if (m_ThreadsData->m_ResourceJobsQueue.empty() && m_Stop) return;
          j = std::move(m_ThreadsData->m_ResourceJobsQueue.front());
          m_ThreadsData->m_ResourceJobsQueue.pop();
          future_availability fa = j.get_future_availability();
          swap = m_SwapMap[fa.get_id()];
          m_SwapMap.erase(fa.get_id());
        }
        j();

        swap();
      }
      });
  }

  ResourceManager::~ResourceManager()
  {
    m_Stop = true;
    m_ThreadsData->m_ResourceCondition.notify_all();
    if (m_Data->thread->joinable())
    {
      m_Data->thread->join();
    }
    glfwDestroyWindow(m_Data->window);
  }

}