#pragma once

#include "Graphics/GraphicsContext.h"
#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <string>

class OpenGLContext : public GraphicsContext
{
public:
  OpenGLContext(GLFWwindow* window);

  virtual void create_info() override;

  virtual ~OpenGLContext();
    
private:
  GLFWwindow* m_Window;
  std::string m_ContextInfo;
};