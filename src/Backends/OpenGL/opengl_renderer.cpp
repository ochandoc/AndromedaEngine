
#include "Backends/OpenGL/OpenGL.h"

#include "Andromeda/Graphics/Renderer.h"
#include "Andromeda/HAL/Window.h"


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
#include "Backends/OpenGL/opengl_uniform_buffer.h"

namespace And
{

Renderer::Renderer(Window& window) : m_Window(window), m_Camera(window)
{
  static float default_color[] = { 0.094f, 0.094f, 0.094f, 1.0f };

  m_Camera.SetPosition(0.0f, 14.0f, 11.0f);



  m_Camera.SetFov(90.0f);
  m_Camera.SetDirection(0.0f, 0.0f, -1.0f);

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
  info.Formats.push_back(ETextureFormat::RGBA8);
  m_shadows_buffer_ = MakeRenderTarget(info);



  // Crear shader de profundidad
  m_depth_shader = OldShader::make_default("lights/depth_shader.shader", "none", LightType::None);
  //m_shadow_shader = Shader::make_default("lights/shadow_shader.shader", "none", LightType::None);

  // Primero creamos todos los shaders y luego cogemos los datos de los uniform buffers de cada uno
  // Cada vez que quiera usar el shader los casteo a opengl shader
  m_shader_ambient = MakeShader("lights/ambient.shader");
  m_shader_directional = MakeShader("lights/directional.shader");
  m_shader_point = MakeShader("lights/point.shader");
  m_shader_spot = MakeShader("lights/spot.shader");

  // Create uniform buffers for lights
  m_buffer_matrix = std::make_shared<UniformBuffer>(0, 208);
  m_buffer_ambient_light = std::make_shared<UniformBuffer>(2, 48);
  m_buffer_directional_light = std::make_shared<UniformBuffer>(3, 48);
  m_buffer_point_light = std::make_shared<UniformBuffer>(4, 64);
  m_buffer_spot_light = std::make_shared<UniformBuffer>(5, 96);

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

void Renderer::draw_obj(MeshComponent* obj, OldShader* s, TransformComponent* tran)
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

  unsigned int VBO = obj->MeshOBJ->get_vbo();
  unsigned int VAO = obj->MeshOBJ->get_vao();
  WAIT_GPU_LOAD();

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBindVertexArray(VAO); // Aqui salta el error en el 2

  const std::vector<Vertex_info>& vertices = obj->MeshOBJ->getVertexInfo();

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_info), (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_info), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex_info), (void*)(6 * sizeof(float)));

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glEnable(GL_DEPTH_TEST);

  const std::vector<unsigned int>& indices = obj->MeshOBJ->getIndices();
  glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, indices.data());
  //glFlush();
  WAIT_GPU_LOAD();

}

/*void Renderer::draw_obj_shadows(MeshComponent* obj, OldShader* s, TransformComponent* trans, const Light& l) {
  //if(s){
    //s->use();
  //}
  //auto start = std::chrono::high_resolution_clock::now();
  
    
  glm::mat4 viewMatrix = glm::make_mat4(m_Camera.GetViewMatrix());
  glm::mat4 projectionMatrix = glm::make_mat4(m_Camera.GetProjectionMatrix());

  glm::mat4 modelMatrix = glm::mat4(1.0f);

  glm::vec3 objectPosition = glm::vec3(trans->position[0], trans->position[1], trans->position[2]);
  glm::vec3 objectScale = glm::vec3(trans->scale[0], trans->scale[1], trans->scale[2]);
  float rotationAngle = 0.0f;
  glm::vec3 objectRotationAxis = glm::vec3(trans->rotation[0], trans->rotation[1], trans->rotation[2]);

  modelMatrix = glm::scale(modelMatrix, objectScale);
  modelMatrix = glm::rotate(modelMatrix, rotationAngle, objectRotationAxis);
  modelMatrix = glm::translate(modelMatrix, objectPosition);
  glm::mat4 viewProjCam = projectionMatrix * viewMatrix;

  // Projection & view of light
  // TODO add campo en lights para las matrices asi solo tengo que hacerlo una vez y me lo guardo
  // Esto solo con la spot para probar
  glm::vec3 pos(l.spot->position[0],l.spot->position[1], l.spot->position[2]);
  glm::vec3 dir(l.spot->direction[0],l.spot->direction[1], l.spot->direction[2]);
  glm::vec3 up(0.0f, 1.0f, 0.0f);
  glm::vec3 right = glm::normalize(glm::cross(up, dir));
  up = glm::cross(dir, right);
  glm::mat4 viewLight = glm::lookAt(pos, pos + glm::normalize(dir), up);

  float fov_radians = glm::radians(l.spot->outer_cut_off) * 1.5f;
  float aspect_ratio = (float)m_shadows_buffer_->GetCreationInfo().Width / (float)m_shadows_buffer_->GetCreationInfo().Height;
  float near = 10.0f;
  float far = 310.0f;
  glm::mat4 projLight = glm::perspective(fov_radians, aspect_ratio, near, far);
  glm::mat4 projViewLight = projLight * viewLight;


  s->set_camera_position(m_Camera.GetPosition());
  s->setModelViewProj(glm::value_ptr(modelMatrix), glm::value_ptr(viewProjCam), glm::value_ptr(projViewLight));
  s->upload_data();

  unsigned int VBO = obj->MeshOBJ->get_vbo();
  unsigned int VAO = obj->MeshOBJ->get_vao();
  WAIT_GPU_LOAD();

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBindVertexArray(VAO);

  const std::vector<Vertex_info>& vertices = obj->MeshOBJ->getVertexInfo();

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_info), (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_info), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex_info), (void*)(6 * sizeof(float)));

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glEnable(GL_DEPTH_TEST);

  const std::vector<unsigned int>& indices = obj->MeshOBJ->getIndices();
  glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, indices.data());
  //glFlush();
  WAIT_GPU_LOAD();

}*/

void Renderer::draw_scene(Scene& scene, OldShader* s)
{
  EntityComponentSystem& ECS = scene.m_ECS;

  for (auto [transform, obj] : ECS.get_components<TransformComponent, MeshComponent>())
  {
    draw_obj(obj, s, transform);
  }
}

void Renderer::draw_deep_obj(MeshComponent* obj, OldShader* s, TransformComponent* tran, float* view, float* projection){

  glm::mat4 modelMatrix = glm::mat4(1.0f);

  glm::vec3 objectPosition = glm::vec3(tran->position[0], tran->position[1], tran->position[2]);
  glm::vec3 objectScale = glm::vec3(tran->scale[0], tran->scale[1], tran->scale[2]);
  float rotationAngle = 0.0f;
  glm::vec3 objectRotationAxis = glm::vec3(tran->rotation[0], tran->rotation[1], tran->rotation[2]);

  modelMatrix = glm::scale(modelMatrix, objectScale);
  modelMatrix = glm::rotate(modelMatrix, rotationAngle, objectRotationAxis);
  modelMatrix = glm::translate(modelMatrix, objectPosition);

  // projection * view de la light
  //glm::mat4 viewLight = glm::make_mat4(view);
  //glm::mat4 projectionLight = glm::make_mat4(projection);
  //glm::mat4 projViewLight = projectionLight * viewLight;

  // Projection * view de la camara
  //glm::mat4 viewCam = glm::make_mat4(m_Camera.GetViewMatrix());
  //glm::mat4 projectionCam = glm::make_mat4(m_Camera.GetProjectionMatrix());
  //glm::mat4 prjViewCam = projectionCam * viewCam;

  s->set_camera_position(m_Camera.GetPosition());
  //s->setModelViewProj(glm::value_ptr(modelMatrix), glm::value_ptr(projViewLight), glm::value_ptr(prjViewCam));
  s->setModelViewProj(glm::value_ptr(modelMatrix), view, projection);
  s->upload_data();

  unsigned int VBO = obj->MeshOBJ->get_vbo();
  unsigned int VAO = obj->MeshOBJ->get_vao();
  WAIT_GPU_LOAD();

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBindVertexArray(VAO); // Aqui tambien

  const std::vector<Vertex_info>& vertices = obj->MeshOBJ->getVertexInfo();

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_info), (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_info), (void*)(3 * sizeof(float)));

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glEnable(GL_DEPTH_TEST);

  const std::vector<unsigned int>& indices = obj->MeshOBJ->getIndices();
  glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, indices.data());
  //glFlush();
  //WAIT_GPU_LOAD();
  s->un_configure_OldShader();
  //err = glGetError();

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

/*void Renderer::draw_shadows(Light l, MeshComponent* obj, TransformComponent* tran) {

  // Esto para la spot light
  glm::vec3 pos;
  glm::vec3 dir;

  if(l.spot){
    pos = glm::vec3(l.spot->position[0],l.spot->position[1], l.spot->position[2]);
    dir = glm::vec3(l.spot->direction[0],l.spot->direction[1], l.spot->direction[2]);
  }

  if(l.directional){
    //glm::vec3cam_pos = m_Camera.GetPosition();
    //float middle = 
  }
  
  // Para la directional, la posicion tiene que estar en la mitad del flusthrum en z, y en x e y tengo que sacar la posicion segun la direccion a la que viene la luz,
  // y luego ir moviendola ligeramente hasta sacar los valores vorrectos


  glm::vec3 up(0.0f, 1.0f, 0.0f);
  glm::vec3 right = glm::normalize(glm::cross(up, dir));
  up = glm::cross(dir, right);
  glm::mat4 view = glm::lookAt(pos, pos + glm::normalize(dir), up);
  int width = m_shadows_buffer_->GetCreationInfo().Width;
  int height = m_shadows_buffer_->GetCreationInfo().Height;

  float fov_radians = glm::radians(l.spot->outer_cut_off) * 1.5f;
  float aspect_ratio = (float)width / (float)height;
  float near = 10.0f;
  float far = 310.0f;
  
  glm::mat4 persp = glm::perspective(fov_radians, aspect_ratio, near, far);


  m_depth_shader->use();
  draw_deep_obj(obj, m_depth_shader.get(), tran, glm::value_ptr(view), glm::value_ptr(persp));




}*/

void DrawForward(EntityComponentSystem& entity, Renderer& renderer){
    /*/std::shared_ptr<And::RenderTarget> shadow_buffer = renderer.get_shadow_buffer();

    shadow_buffer->Activate();
    for (auto light : l_manager.get_lights()) {

      //And::Shader* s = l_manager.bind_light(light);

      if(light.type == And::LightType::Spot){

        for (auto [transform, obj] : entity.get_components<And::TransformComponent, And::MeshComponent>()){
          renderer.draw_shadows(light, obj, transform);
        }
      }

    }
    shadow_buffer->Desactivate();



     for (auto light : l_manager.get_lights()) {

      And::OldShader* s = l_manager.bind_light(light);
        
        
      //And::Shader* s = l_manager.bind_light(light);
      
      if(light.type == And::LightType::Spot){
        std::vector<std::shared_ptr<And::Texture>> shadow_texture = shadow_buffer->GetTextures();
        s->set_texture(shadow_texture[0].get());
      }
      //s->set_texture(texture.get());
      
      //start = std::chrono::high_resolution_clock::now();
      for (auto [transform, obj] : entity.get_components<And::TransformComponent, And::MeshComponent>()){
        // A este draw obj, si es de la spot tengo que pasarle bien las matrices premultiplicadas, que esta llamando a la funcion normal y no las estoy precalculando
        if(light.type == And::LightType::Spot){
          renderer.draw_obj_shadows(obj, s, transform, light);
        }else{
          renderer.draw_obj(obj, s, transform);
        }
      }
      //end = std::chrono::high_resolution_clock::now();
      //elapsed = end - start;
      //printf("Duration inner loop-> %f\n", elapsed.count() * 1000.0f);

    }*/
}

}
