#include "Common/GraphicsContext.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "Common/Renderer.h"

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


}

GraphicsContext::GraphicsContext(Window& window) : m_Data(new ContextData)
{
  m_Data->window = (GLFWwindow*)window.get_native_window();

  glfwMakeContextCurrent(m_Data->window);
  glewInit();
# ifdef DEBUG
  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(MessageCallback, 0);
# endif
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

