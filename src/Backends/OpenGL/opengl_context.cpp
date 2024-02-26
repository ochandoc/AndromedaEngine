#include "Andromeda/Graphics/GraphicsContext.h"

#include "Andromeda/Graphics/Renderer.h"
#include "Andromeda/HAL/Window.h"

#include "Backends/OpenGL/OpenGL.h"
#include "GLFW/glfw3.h"

DEFINE_LOG_CATEGORY(GraphicsContextLog)

namespace And
{

  struct ContextData
  {
    std::string context_info;
    GLFWwindow* window;
  };

// Cuando hay un error en opengl, llama a esta funcion y recibe el error
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
			source_ = "Shader COMPILER";
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
			AND_LOG(GraphicsContextLog, Info, "{}: {} , raised from {}: {}", id, type_, source_, message);
			break;

		case GL_DEBUG_SEVERITY_MEDIUM:
			AND_LOG(GraphicsContextLog, Warning, "{}: {} , raised from {}: {}", id, type_, source_, message);
			break;

		case GL_DEBUG_SEVERITY_LOW:
			AND_LOG(GraphicsContextLog, Warning, "{}: {} , raised from {}: {}", id, type_, source_, message);
			break;

		case GL_DEBUG_SEVERITY_NOTIFICATION:
			//_severity = "NOTIFICATION";
			//AND_LOG(GraphicsContextLog, Debug, "{}: {} , raised from {}: {}", id, type_, source_, message);
			break;

		default:
			//_severity = "UNKNOWN";
			AND_LOG(GraphicsContextLog, Info, "{}: {} , raised from {}: {}", id, type_, source_, message);
			break;
		}
	}

GraphicsContext::GraphicsContext(Window& window) : m_Data(new ContextData)
{
  m_Data->window = (GLFWwindow*)window.get_native_window();

  glfwMakeContextCurrent(m_Data->window);
  glewInit();
  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(MessageCallback, 0);
}

GraphicsContext::~GraphicsContext() {}


void GraphicsContext::create_info(){
  m_Data->context_info = "Graphics Context:  OpenGL\n\n";
  m_Data->context_info += "Vendor: ";
  m_Data->context_info += (const char*)glGetString(GL_VENDOR); // proveedor de los graficos
  m_Data->context_info += "\n";
  m_Data->context_info += "Graphics Device: ";
  m_Data->context_info += (const char*)glGetString(GL_RENDERER);
  m_Data->context_info += "\n";
  m_Data->context_info += "Version: ";
  m_Data->context_info += (const char*)glGetString(GL_VERSION);
  m_Data->context_info += "\n";
  m_Data->context_info += "GLSL Version: ";
  m_Data->context_info += (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
  m_Data->context_info += "\n";

  printf("%s\n", m_Data->context_info.c_str());
}

}

