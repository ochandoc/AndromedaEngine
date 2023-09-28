#include "OpenGLContext.h"

#include "Common/Renderer.h"

namespace And
{

// Cuando hay un error en opengl, llama a esta funcion y recibe el error
static void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message,
		const void* userParam) {}

OpenGLContext::OpenGLContext(GLFWwindow* window){
  m_Window = window;
  
  glfwMakeContextCurrent(window);
  glewInit();
# ifdef DEBUG
  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(MessageCallback, 0);
# endif
}

OpenGLContext::~OpenGLContext() {}


void OpenGLContext::create_info(){
  m_ContextInfo = "Graphics Context:  OpenGL\n\n";
  m_ContextInfo += "Vendor: ";
  m_ContextInfo += (const char*)glGetString(GL_VENDOR); // proveedor de los graficos
  m_ContextInfo += "\n";
  m_ContextInfo += "Graphics Device: ";
  m_ContextInfo += (const char*)glGetString(GL_RENDERER);
  m_ContextInfo += "\n";
  m_ContextInfo += "Version: ";
  m_ContextInfo += (const char*)glGetString(GL_VERSION); 
  m_ContextInfo += "\n";
  m_ContextInfo += "GLSL Version: ";
  m_ContextInfo += (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
  m_ContextInfo += "\n";

  printf("%s\n", m_ContextInfo.c_str());
}

}

