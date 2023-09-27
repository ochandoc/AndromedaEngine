#include "OpenGLRenderer.h"
#include "GL/glew.h"

#include "Core/Window.h"

namespace And
{

OpenGLRenderer::OpenGLRenderer(Window& window) : Renderer(window)
{
  static float default_color[4] = {1.0f, 1.0f, 1.0f, 1.0f};
  set_clear_color(default_color);
}

OpenGLRenderer::~OpenGLRenderer(){

}

void OpenGLRenderer::new_frame()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRenderer::end_frame()
{
  m_Window.update();
}

void OpenGLRenderer::set_viewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height){
  glViewport(x, y, width, height);
}
  
void OpenGLRenderer::set_clear_color(float* color){
  glClearColor(color[0], color[1], color[2], color[3]);
}

}
