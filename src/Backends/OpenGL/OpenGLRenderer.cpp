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


void OpenGLRenderer::showDemo(){

  float triangle[6] = {
    -0.5f, -0.5f,
    0.0f, 0.5f,
    0.5f, -0.5f,
  };
  
  // Bindeamos el vao
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  // Generamos los buffers de pintado
  unsigned int VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);

  // Pasamos el layout
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

  // Pintamos
  unsigned int indices[3] = {2, 1, 0};
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, indices);

  // Desbindeamos el vao
  glBindVertexArray(0);

  // Desbindeamos el vbo
  glBindBuffer(GL_ARRAY_BUFFER, 0);

}

std::shared_ptr<Shader> OpenGLRenderer::createShader(std::vector<ShaderInfo> s_info){
  return std::shared_ptr<Shader>(new OpenGLShader(s_info));
}

}
