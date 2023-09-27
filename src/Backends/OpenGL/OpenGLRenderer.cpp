#include "OpenGLRenderer.h"
#include "OpenGLShader.h"
#include "GL/glew.h"

namespace And
{

OpenGLRenderer::OpenGLRenderer()
{
  static float default_color[4] = {1.0f, 1.0f, 1.0f, 1.0f};
  set_clear_color(default_color);
}

OpenGLRenderer::~OpenGLRenderer(){

}


void OpenGLRenderer::set_viewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height){
  glViewport(x, y, width, height);
}
  
void OpenGLRenderer::set_clear_color(float* color){
  glClearColor(color[0], color[1], color[2], color[3]);
}

void OpenGLRenderer::clear()
{
  // Limpia el buffer
  // Limpia el buffer de profundidad
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void OpenGLRenderer::showDemo(float triangle[6]){
  
  GLenum error =  glGetError();
  
  // Bindeamos el vao
  unsigned int vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  unsigned int buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), triangle, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);

  error =  glGetError();
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0); // Error
  error =  glGetError();

  // Desbindeamos el vao
  glBindVertexArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);



  error =  glGetError();
  /*GLenum error =  glGetError();
  if(error != GL_NO_ERROR){
    printf("cagaste");
  }*/
}


void OpenGLRenderer::printDemo(){
  GLenum error =  glGetError();
  glDrawArrays(GL_TRIANGLES, 0, 3);
  error =  glGetError();
}

std::shared_ptr<Shader> OpenGLRenderer::createShader(std::vector<ShaderInfo> s_info){
  return std::shared_ptr<Shader>(new OpenGLShader(s_info));
}

}
