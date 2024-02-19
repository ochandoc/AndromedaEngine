#include "Andromeda/Graphics/Renderer.h"
#include "Andromeda/HAL/Window.h"

#include "Backends/OpenGL/OpenGL.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui_impl_opengl3.h"

#include "Andromeda/Graphics/Shader.h"
#include "Andromeda/Graphics/Triangle.h"
#include "Andromeda/Graphics/ObjLoader.h"

#include "Andromeda/UI/Plot/implot.h"

#include "Andromeda/ECS/Components/TransformComponent.h"
#include "Andromeda/ECS/Components/MeshComponent.h"
#include "Backends/OpenGL/OpenGLTexture2D.h"

namespace And
{

Renderer::Renderer(Window& window) : m_Window(window), m_Camera(window)
{
  static float default_color[] = { 0.094f, 0.094f, 0.094f, 1.0f };

  m_Camera.SetPosition(0.0f, 7.0f, -60.0f);



  m_Camera.SetFov(90.0f);
  m_Camera.SetDirection(0.0f, -7.0f, 60.0f);

  int width = m_Window.get_width();
  int height = m_Window.get_height();
  m_Camera.SetSize((float)width, (float)height);

  m_Camera.SetFar(1000.0f);
  m_Camera.SetNear(10.0f);

  set_clear_color(default_color);
  window.imgui_start();
  ImGui_ImplOpenGL3_Init("#version 430 core");

  /* {
    std::vector<ETextureFormat> Formats = { ETextureFormat::RGBA8, ETextureFormat::RGBA8, ETextureFormat::Depth };
    m_RenderTarget = std::make_shared<RenderTarget>(width, height, Formats);
    m_Window.OnWindowResize.AddDynamic(m_RenderTarget.get(), &RenderTarget::Resize);
    m_bDrawOnTexture = false;
  }*/
  RenderTargetCreationInfo info;
  info.Width = width;
  info.Height = height;
  info.Formats.push_back(ETextureFormat::Depth);
  m_shadows_buffer_ = MakeRenderTarget(info);


  // Crear shader de profundidad
  m_depth_shader = Shader::make_default("lights/depth_shader.shader", "none", LightType::None);
}

Renderer::~Renderer(){
	m_Window.imgui_end();
}

void Renderer::new_frame()
{
  ImGui_ImplOpenGL3_NewFrame(); 
	m_Window.new_frame();
  ImGui::NewFrame();

  //glDepthMask(GL_FALSE);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glEnable(GL_DEPTH_TEST);
  //glDepthFunc(GL_LEQUAL);

  glEnable(GL_BLEND);
  glBlendFunc(GL_ONE, GL_ZERO);

  //if (m_bDrawOnTexture)

  /*if (m_bDrawOnTexture)
  {
    m_RenderTarget->Bind();
  }*/
  //glDepthFunc(GL_ALWAYS);
  //glClearDepthf(0.5f);
}

void Renderer::end_frame()
{
  //m_RenderTarget->Unbind();
  //ImPlot::ShowDemoWindow();
  //ImGui::ShowDemoWindow();

  m_Camera.ProcessInput();

  if (ImGui::Begin("Camera"))
  {
    m_Camera.ShowValues();
  }
  ImGui::End();

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	m_Window.end_frame();
}

void Renderer::set_viewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height){
  glViewport(x, y, width, height);
}

void Renderer::set_draw_on_texture(bool value)
{
  m_bDrawOnTexture = value;
}
  
void Renderer::set_clear_color(float* color){
  glClearColor(color[0], color[1], color[2], color[3]);
}

std::shared_ptr<RenderTarget> Renderer::get_render_target() const
{
  return m_RenderTarget;
}

void Renderer::draw_triangle(Triangle *t){
    
  Vertex *v = t->get_vertex();
  const float vertices[9] = {
    v[0].position[0], v[0].position[1], v[0].position[2],
    v[1].position[0], v[1].position[1], v[1].position[2],
    v[2].position[0], v[2].position[1], v[2].position[2],
    
    };

  
  unsigned int VAO = t->m_vao;
  unsigned int VBO = t->m_vbo;
  
  //glGenVertexArrays(1, &VAO);
  //glGenBuffers(1, &VBO);
  
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, v->indices);

  // Desbindeamos el vao
  glBindVertexArray(0);

  // Desbindeamos el vbo
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Borramos los buffers
  //glDeleteBuffers(1, &VAO);
  //glDeleteBuffers(1, &VBO);

  
}

void CheckError(){
  GLenum error = glGetError();
  switch (error) {
    case GL_NO_ERROR:
        // No se ha producido ningún error.
        break;
    case GL_INVALID_ENUM:
        // Manejar el error de enumerador no válido.
        printf("Invalid enum\n");
        break;
    case GL_INVALID_VALUE:
        // Manejar el error de valor no válido.
        printf("Invalid value\n");
        break;
    case GL_INVALID_OPERATION:
        // Manejar el error de operación no válida.
        printf("Invalid operation\n");
        break;
    case GL_OUT_OF_MEMORY:
        // Manejar el error de falta de memoria.
        printf("Out of memory\n");
        break;
    case GL_STACK_OVERFLOW:
        // Manejar el error de desbordamiento de la pila.
        printf("Stack overflow\n");
        break;
    case GL_STACK_UNDERFLOW:
        // Manejar el error de subdesbordamiento de la pila.
        printf("Stack underflow\n");
        break;
    // Puedes agregar más casos para otros errores si es necesario.
    default:
        // Manejar cualquier otro error no reconocido.
        break;
}
}

void Renderer::draw_obj(MeshComponent* obj, Shader* s, TransformComponent* tran)
{
  //if(s){
    //s->use();
  //}
  //auto start = std::chrono::high_resolution_clock::now();
  
    
  glm::mat4 viewMatrix = glm::make_mat4(m_Camera.GetViewMatrix());
  glm::mat4 projectionMatrix = glm::make_mat4(m_Camera.GetProjectionMatrix());

  glm::mat4 modelMatrix = glm::mat4(1.0f);

  glm::vec3 objectPosition = glm::vec3(tran->position[0], tran->position[1], tran->position[2]);
  glm::vec3 objectScale = glm::vec3(tran->scale[0], tran->scale[1], tran->scale[2]);
  float rotationAngle = 0.0f;
  glm::vec3 objectRotationAxis = glm::vec3(tran->rotation[0], tran->rotation[1], tran->rotation[2]);

  modelMatrix = glm::scale(modelMatrix, objectScale);
  modelMatrix = glm::rotate(modelMatrix, rotationAngle, objectRotationAxis);
  modelMatrix = glm::translate(modelMatrix, objectPosition);

  s->set_camera_position(m_Camera.GetPosition());
  s->setModelViewProj(glm::value_ptr(modelMatrix), glm::value_ptr(viewMatrix), glm::value_ptr(projectionMatrix));
  s->upload_data();

  unsigned int VBO = obj->Mesh->get_vbo();
  unsigned int VAO = obj->Mesh->get_vao();

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBindVertexArray(VAO);

  const std::vector<Vertex_info>& vertices = obj->Mesh->getVertexInfo();

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_info), (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_info), (void*)(3 * sizeof(float)));

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glEnable(GL_DEPTH_TEST);

  const std::vector<unsigned int>& indices = obj->Mesh->getIndices();
  glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, indices.data());
  //glFlush();
  //WAIT_GPU_LOAD();
}

void Renderer::draw_obj(MeshComponent* obj, Shader* s, TransformComponent* tran, AmbientLight* ambient, PointLight* point, Texture* texture) {
  if(s){
    s->use();
  }

  glm::mat4 viewMatrix = glm::make_mat4(m_Camera.GetViewMatrix());
  glm::mat4 projectionMatrix = glm::make_mat4(m_Camera.GetProjectionMatrix());

  glm::mat4 modelMatrix = glm::mat4(1.0f);

  glm::vec3 objectPosition = glm::vec3(tran->position[0], tran->position[1], tran->position[2]);
  glm::vec3 objectScale = glm::vec3(tran->scale[0], tran->scale[1], tran->scale[2]);
  float rotationAngle = 0.0f;
  glm::vec3 objectRotationAxis = glm::vec3(tran->rotation[0], tran->rotation[1], tran->rotation[2]);

  modelMatrix = glm::scale(modelMatrix, objectScale);
  modelMatrix = glm::rotate(modelMatrix, rotationAngle, objectRotationAxis);
  modelMatrix = glm::translate(modelMatrix, objectPosition);

  s->set_camera_position(m_Camera.GetPosition());
  s->set_light(ambient);
  s->set_light(point);
  s->set_texture(texture);
  s->setModelViewProj(glm::value_ptr(modelMatrix), glm::value_ptr(viewMatrix), glm::value_ptr(projectionMatrix));
  s->upload_data();


  unsigned int VBO = obj->Mesh->get_vbo();
  unsigned int VAO = obj->Mesh->get_vao();

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBindVertexArray(VAO);
  //err = glGetError();


  const std::vector<Vertex_info>& vertices = obj->Mesh->getVertexInfo();  

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_info), (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_info), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex_info), (void*)(6 * sizeof(float)));
  //err = glGetError();

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glEnable(GL_DEPTH_TEST);

  std::vector<unsigned int> indices = obj->Mesh->getIndices();
  glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, indices.data());
  s->un_configure_shader();
  //err = glGetError();

}

void Renderer::draw_scene(Scene& scene, Shader* s)
{
  EntityComponentSystem& ECS = scene.m_ECS;

  for (auto [transform, obj] : ECS.get_components<TransformComponent, MeshComponent>())
  {
    draw_obj(obj, s, transform);
  }
}

void Renderer::draw_obj(MeshComponent* obj, Shader* s, TransformComponent* tran, AmbientLight* ambient, PointLight* point) {

  if (s) {
    s->use();
  }

  glm::mat4 viewMatrix = glm::make_mat4(m_Camera.GetViewMatrix());
  glm::mat4 projectionMatrix = glm::make_mat4(m_Camera.GetProjectionMatrix());

  glm::mat4 modelMatrix = glm::identity<glm::mat4>();

  glm::vec3 objectPosition = glm::vec3(tran->position[0], tran->position[1], tran->position[2]);
  glm::vec3 objectScale = glm::vec3(tran->scale[0], tran->scale[1], tran->scale[2]);
  float rotationAngle = 0.0f;
  glm::vec3 objectRotationAxis = glm::vec3(tran->rotation[0], tran->rotation[1], tran->rotation[2]); // esto esta mal

  modelMatrix = glm::scale(modelMatrix, objectScale);
  modelMatrix = glm::rotate(modelMatrix, rotationAngle, objectRotationAxis);
  modelMatrix = glm::translate(modelMatrix, objectPosition);

  s->set_camera_position(m_Camera.GetPosition());
  s->set_light(ambient);
  s->set_light(point);
  s->setModelViewProj(glm::value_ptr(modelMatrix), glm::value_ptr(viewMatrix), glm::value_ptr(projectionMatrix));
  s->upload_data();


  unsigned int VBO = obj->Mesh->get_vbo();
  unsigned int VAO = obj->Mesh->get_vao();

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBindVertexArray(VAO);
  //err = glGetError();


  std::vector<Vertex_info> vertices = obj->Mesh->getVertexInfo();

  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex_info), &vertices[0], GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_info), (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_info), (void*)(3 * sizeof(float)));
  //err = glGetError();

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glEnable(GL_DEPTH_TEST);

  std::vector<unsigned int> indices = obj->Mesh->getIndices();
  glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, indices.data());
  s->un_configure_shader();
  //err = glGetError();

}

void Renderer::draw_deep_obj(MeshComponent* obj, Shader* s, TransformComponent* tran, float* view, float* projection){

  glm::mat4 modelMatrix = glm::mat4(1.0f);

  glm::vec3 objectPosition = glm::vec3(tran->position[0], tran->position[1], tran->position[2]);
  glm::vec3 objectScale = glm::vec3(tran->scale[0], tran->scale[1], tran->scale[2]);
  float rotationAngle = 0.0f;
  glm::vec3 objectRotationAxis = glm::vec3(tran->rotation[0], tran->rotation[1], tran->rotation[2]);

  modelMatrix = glm::scale(modelMatrix, objectScale);
  modelMatrix = glm::rotate(modelMatrix, rotationAngle, objectRotationAxis);
  modelMatrix = glm::translate(modelMatrix, objectPosition);

  s->set_camera_position(m_Camera.GetPosition());
  s->setModelViewProj(glm::value_ptr(modelMatrix), view, projection);
  s->upload_data();

  unsigned int VBO = obj->Mesh->get_vbo();
  unsigned int VAO = obj->Mesh->get_vao();

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBindVertexArray(VAO);

  const std::vector<Vertex_info>& vertices = obj->Mesh->getVertexInfo();

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_info), (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_info), (void*)(3 * sizeof(float)));

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glEnable(GL_DEPTH_TEST);

  const std::vector<unsigned int>& indices = obj->Mesh->getIndices();
  glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, indices.data());
  //glFlush();
  //WAIT_GPU_LOAD();
}

void Renderer::showDemo(){

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

  // Borramos los buffers
  //glDeleteBuffers(1, &VAO);
  //glDeleteBuffers(1, &VBO);

}

void Renderer::showImGuiDemoWindow()
{
  ImGui::ShowDemoWindow();
}
std::shared_ptr<RenderTarget> Renderer::get_shadow_buffer(){
  return m_shadows_buffer_;
}

void Renderer::draw_shadows(Light l, MeshComponent* obj, TransformComponent* tran){

  // Pintar cada obj de la escena con el shader de profundidad y guardar todo lo que pinte en el render target
  // sacar la view y projeciton de la posicion de la luz
  glm::vec3 pos(l.spot->position[0],l.spot->position[1], l.spot->position[2]);
  glm::vec3 dir(l.spot->direction[0],l.spot->direction[1], l.spot->direction[2]);
  glm::vec3 up(0.0f, 1.0f, 0.0f);

  glm::vec3 right = glm::normalize(glm::cross(up, dir));
  up = glm::cross(dir, right);
  glm::mat4 view = glm::lookAt(pos, pos + glm::normalize(dir), up);
  int width = m_shadows_buffer_->GetCreationInfo().Width;
  int height = m_shadows_buffer_->GetCreationInfo().Height;
  
  glm::perspective persp(glm::radians(l.spot->outer_cut_off), width / height, 10.0f, 310.0f);


  m_depth_shader->use();
  draw_deep_obj(obj, m_depth_shader, tran, glm::value_ptr(view), glm::value_ptr(persp));




}
}
