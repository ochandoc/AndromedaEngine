
#include "Backends/OpenGL/OpenGL.h"

#include "Backends/OpenGL/RendererOpenGL.h"
#include "Andromeda/HAL/Window.h"


#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui_impl_opengl3.h"

#include "Andromeda/Graphics/Triangle.h"
#include "Andromeda/Graphics/Geometry.h"

#include "Andromeda/UI/Plot/implot.h"

#include "Backends/OpenGL/OpenGLRenderTarget.h"
#include "Andromeda/Graphics/Shader.h"
#include "Andromeda/ECS/Components/TransformComponent.h"
#include "Andromeda/ECS/Components/MeshComponent.h"
#include "Backends/OpenGL/OpenGLTexture2D.h"
#include "Backends/OpenGL/opengl_uniform_buffer.h"
#include "Backends/OpenGL/OpenGLShader.h"
#include "Andromeda/Graphics/Lights/SpotLight.h"
#include "Andromeda/Graphics/Lights/AmbientLight.h"
#include "Andromeda/Graphics/Lights/DirectionalLight.h"
#include "Andromeda/Graphics/Lights/PointLight.h"

namespace And
{

  struct UniformBlockMatrices{
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection; // 192
    glm::vec3 camera_position;
    //float padding; // 448 bytes, aligned to 28 blocks of 16 bytes
  };
  
  struct UniformBlockMatricesPointLight{
    glm::mat4 model;
    glm::mat4 proj_view_light[6];
    glm::mat4 proj_view_cam; 
    glm::vec3 camera_position; // 524 bytes
  };

  struct Direction {
      glm::vec3 dir[6];
  };

RendererOpenGL::RendererOpenGL(Window& window) : m_Window(window), m_UserCamera(nullptr), m_DefaultCamera(window)
{
  static float default_color[] = { 0.094f, 0.094f, 0.094f, 1.0f };

  m_DefaultCamera.SetPosition(0.0f, 0.0f, 0.0f);
  m_DefaultCamera.SetFov(90.0f);
  m_DefaultCamera.SetDirection(0.0f, 0.0f, -1.0f);

  int width = m_Window.get_width();
  int height = m_Window.get_height();
  m_DefaultCamera.SetSize((float)width, (float)height);

  m_DefaultCamera.SetFar(1000.0f);
  m_DefaultCamera.SetNear(0.1f);


  set_clear_color(default_color);
  window.imgui_start();
  ImGui_ImplOpenGL3_Init("#version 430 core");

  RenderTargetCreationInfo info;
  info.Width = width;
  info.Height = height;
  info.Formats.push_back(ETextureFormat::Depth);
  info.Formats.push_back(ETextureFormat::RGBA8);
  m_shadows_buffer_ = MakeRenderTarget(info);

  info.Formats.clear();
  info.Formats.push_back(ETextureFormat::RGB16F); // Posicion
  info.Formats.push_back(ETextureFormat::RGB16F); // Normales
  info.Formats.push_back(ETextureFormat::RGBA8);  // Color
  m_gBuffer_ = MakeRenderTarget(info);

  // Shadow buffers for point light
  for(int i = 0; i < 6; i++){
    m_shadows_buffer_pointLight.push_back(MakeRenderTarget(info)); 
  }

  // Crear shader de profundidad
  m_depth_shader = MakeShader("lights/depth_shader.shader");
  m_shadow_shader = MakeShader("lights/shadow_shader.shader");
  m_shader_ambient = MakeShader("lights/ambient.shader");
  m_shader_directional = MakeShader("lights/directional.shader");
  m_shader_shadows_directional = MakeShader("lights/directional_shadows.shader");
  
  m_shader_point = MakeShader("lights/point.shader");
  m_shader_shadows_point = MakeShader("lights/point_shadow.shader");

  m_shader_spot = MakeShader("lights/spot.shader");
  m_shader_shadows_spot = MakeShader("lights/spot_shadows.shader");

  m_shader_geometry = MakeShader("default/geometry.shader");
  m_shader_quad = MakeShader("lights/deferred/quad.shader");

  glGenVertexArrays(1, &m_quad_vao);
  glGenBuffers(1, &m_quad_vbo);


  // Create uniform buffers for lights
  m_buffer_matrix = std::make_shared<UniformBuffer>(0, 208);
  m_buffer_matrix_pointLight = std::make_shared<UniformBuffer>(1, 524);
  m_buffer_ambient_light = std::make_shared<UniformBuffer>(2, 48);
  m_buffer_directional_light = std::make_shared<UniformBuffer>(3, 48);
  m_buffer_point_light = std::make_shared<UniformBuffer>(4, 64);
  m_buffer_spot_light = std::make_shared<UniformBuffer>(5, 96);

  m_directions = std::make_shared<Direction>();
  m_directions->dir[0] = glm::vec3(1.0f, 0.0f, 0.0f);
  m_directions->dir[1] = glm::vec3(-1.0f, 0.0f, 0.0f);
  m_directions->dir[2] = glm::vec3(0.0f, 1.0f, 0.0f);
  m_directions->dir[3] = glm::vec3(0.0f, -1.0f, 0.0f);
  m_directions->dir[4] = glm::vec3(0.0f, 0.0f, 1.0f);
  m_directions->dir[5] = glm::vec3(0.0f, 0.0f, -1.0f);
}

RendererOpenGL::~RendererOpenGL(){
	m_Window.imgui_end();
}

void RendererOpenGL::set_camera(CameraBase* cam){
    m_UserCamera = cam;
}

void RendererOpenGL::new_frame()
{
  ImGui_ImplOpenGL3_NewFrame(); 
  m_Window.new_frame();
  ImGui::NewFrame();

  //glDepthMask(GL_FALSE);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
   

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

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

void RendererOpenGL::end_frame()
{
  //m_RenderTarget->Unbind();
  //ImPlot::ShowDemoWindow();
  //ImGui::ShowDemoWindow();
  CameraBase* cam = &m_DefaultCamera;
  if(m_UserCamera) cam = m_UserCamera;

  if (!m_UserCamera) {

      if(m_DefaultCamera.GetFixed()){
          m_DefaultCamera.ProcessInput();
      }

      if (ImGui::Begin("Camera"))
      {
          m_DefaultCamera.ShowValues();
      }
      ImGui::End();
  }

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	m_Window.end_frame();
}

void RendererOpenGL::set_viewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height){
  glViewport(x, y, width, height);
}
  
void RendererOpenGL::set_clear_color(float* color){
  glClearColor(color[0], color[1], color[2], color[3]);
}

void RendererOpenGL::draw_triangle(Triangle *t){
    
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

void RendererOpenGL::draw_obj(MeshComponent* obj, Light* l, TransformComponent* tran)
{
  //auto start = std::chrono::high_resolution_clock::now(); 
  CameraBase* cam = &m_DefaultCamera;
  if(m_UserCamera) cam = m_UserCamera;
    
  glm::mat4 viewMatrix = glm::make_mat4(cam->GetViewMatrix());
  glm::mat4 projectionMatrix = glm::make_mat4(cam->GetProjectionMatrix());

  glm::mat4 modelMatrix = glm::mat4(1.0f);

  glm::vec3 objectPosition = glm::vec3(tran->position[0], tran->position[1], tran->position[2]);
  glm::vec3 objectScale = glm::vec3(tran->scale[0], tran->scale[1], tran->scale[2]);
  float rotationAngle = 0.0f;
  glm::vec3 objectRotationAxis = glm::vec3(tran->rotation[0], tran->rotation[1], tran->rotation[2]);

  modelMatrix = glm::translate(modelMatrix, objectPosition);
  modelMatrix = glm::rotate(modelMatrix, rotationAngle, objectRotationAxis);
  modelMatrix = glm::scale(modelMatrix, objectScale);

  glm::vec3 cam_pos = glm::make_vec3(cam->GetPosition());
  UniformBlockMatrices matrices_tmp = {modelMatrix, viewMatrix, projectionMatrix, cam_pos};
  m_buffer_matrix->upload_data((void*)&matrices_tmp, 208);
  m_buffer_matrix->bind();

  SpotLight* spot = dynamic_cast<SpotLight*>(l);
  if(spot){
    m_buffer_spot_light->upload_data(spot->GetData(), 96);
    m_buffer_spot_light->bind();
  }
  
  PointLight* point = dynamic_cast<PointLight*>(l);
  if(point){
    m_buffer_point_light->upload_data(point->GetData(), 64);
    m_buffer_point_light->bind();
  }

  DirectionalLight* directional = dynamic_cast<DirectionalLight*>(l);
  if(directional){
    m_buffer_directional_light->upload_data(directional->GetData(), 48);
    m_buffer_directional_light->bind();
  }
  
  AmbientLight* ambient = dynamic_cast<AmbientLight*>(l);
  if(ambient){
    m_buffer_ambient_light->upload_data(ambient->GetData(), 48);
    m_buffer_ambient_light->bind();
  }

  unsigned int VBO = obj->MeshOBJ->get_vbo();
  unsigned int VAO = obj->MeshOBJ->get_vao();

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

}

void RendererOpenGL::upload_light(Light* l){

    CameraBase* cam = &m_DefaultCamera;
    if (m_UserCamera) cam = m_UserCamera;

    glm::mat4 viewMatrix = glm::make_mat4(cam->GetViewMatrix());
    glm::mat4 projectionMatrix = glm::make_mat4(cam->GetProjectionMatrix());
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    glm::vec3 cam_pos = glm::make_vec3(cam->GetPosition());
    glm::mat4 viewProjCam = projectionMatrix * viewMatrix;


    glm::vec3 light_dir;
    DirectionalLight* dir = static_cast<DirectionalLight*>(l);
    if (dir) {
        light_dir = glm::make_vec3(dir->GetDirection());
    }

    glm::vec3 pos = glm::make_vec3(cam_pos + ((-1.0f * light_dir) * 50.0f));


    glm::vec3 up(0.0f, 1.0f, 0.0f);
    glm::vec3 right = glm::normalize(glm::cross(up, light_dir));
    up = glm::cross(light_dir, right);
    glm::mat4 viewLight = glm::lookAt(pos, pos + glm::normalize(light_dir), up);

    glm::mat4 orto = glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, cam->GetNear(), cam->GetFar());
    glm::mat4 projViewLight = orto * viewLight;
    UniformBlockMatrices matrices_tmp = { modelMatrix, viewProjCam, projViewLight, cam_pos };

    m_buffer_matrix->upload_data((void*)&matrices_tmp, 208);
    m_buffer_matrix->bind();

    SpotLight* spot = dynamic_cast<SpotLight*>(l);
    if (spot) {
        m_buffer_spot_light->upload_data(spot->GetData(), 96);
        m_buffer_spot_light->bind();
    }

    PointLight* point = dynamic_cast<PointLight*>(l);
    if (point) {
        m_buffer_point_light->upload_data(point->GetData(), 64);
        m_buffer_point_light->bind();
    }

    DirectionalLight* directional = dynamic_cast<DirectionalLight*>(l);
    if (directional) {
        m_buffer_directional_light->upload_data(directional->GetData(), 48);
        m_buffer_directional_light->bind();
    }

    AmbientLight* ambient = dynamic_cast<AmbientLight*>(l);
    if (ambient) {
        m_buffer_ambient_light->upload_data(ambient->GetData(), 48);
        m_buffer_ambient_light->bind();
    }

}

void RendererOpenGL::draw_obj_shadows(MeshComponent* obj, TransformComponent* trans, SpotLight* l){
  
  //auto start = std::chrono::high_resolution_clock::now();

  CameraBase* cam = &m_DefaultCamera;
  if(m_UserCamera) cam = m_UserCamera;

  glm::mat4 viewMatrix = glm::make_mat4(cam->GetViewMatrix());
  glm::mat4 projectionMatrix = glm::make_mat4(cam->GetProjectionMatrix());
  glm::mat4 viewProjCam = projectionMatrix * viewMatrix;

  glm::mat4 modelMatrix = glm::mat4(1.0f);

  glm::vec3 objectPosition = glm::vec3(trans->position[0], trans->position[1], trans->position[2]);
  glm::vec3 objectScale = glm::vec3(trans->scale[0], trans->scale[1], trans->scale[2]);
  float rotationAngle = 0.0f;
  glm::vec3 objectRotationAxis = glm::vec3(trans->rotation[0], trans->rotation[1], trans->rotation[2]);

  modelMatrix = glm::translate(modelMatrix, objectPosition);
  modelMatrix = glm::rotate(modelMatrix, rotationAngle, objectRotationAxis);
  modelMatrix = glm::scale(modelMatrix, objectScale);

  float aspect_ratio = (float)m_shadows_buffer_->GetCreationInfo().Width / (float)m_shadows_buffer_->GetCreationInfo().Height;

  glm::mat4 projViewLight = glm::make_mat4(l->GetProjectViewMatrix(aspect_ratio));
  
  glm::vec3 cam_pos = glm::make_vec3(cam->GetPosition());
  UniformBlockMatrices matrices_tmp = {modelMatrix, viewProjCam, projViewLight, cam_pos };

  m_buffer_matrix->upload_data((void*)&matrices_tmp, 208);
  m_buffer_matrix->bind();
  m_buffer_spot_light->upload_data(l->GetData(), 96);
  m_buffer_spot_light->bind();

  unsigned int VBO = obj->MeshOBJ->get_vbo();
  unsigned int VAO = obj->MeshOBJ->get_vao();

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBindVertexArray(VAO);

  const std::vector<Vertex_info>& vertices = obj->MeshOBJ->getVertexInfo();

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_info), (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_info), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex_info), (void*)(6 * sizeof(float)));

  const std::vector<unsigned int>& indices = obj->MeshOBJ->getIndices();
  glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, indices.data());
}

void RendererOpenGL::draw_obj_shadows(MeshComponent* obj, TransformComponent* trans, PointLight* l, float* dirLight){
  
  //auto start = std::chrono::high_resolution_clock::now();
  CameraBase* cam = &m_DefaultCamera;
  if(m_UserCamera) cam = m_UserCamera;

  glm::mat4 viewMatrix = glm::make_mat4(cam->GetViewMatrix());
  glm::mat4 projectionMatrix = glm::make_mat4(cam->GetProjectionMatrix());

  glm::mat4 modelMatrix = glm::mat4(1.0f);

  glm::vec3 objectPosition = glm::vec3(trans->position[0], trans->position[1], trans->position[2]);
  glm::vec3 objectScale = glm::vec3(trans->scale[0], trans->scale[1], trans->scale[2]);
  float rotationAngle = 0.0f;
  glm::vec3 objectRotationAxis = glm::vec3(trans->rotation[0], trans->rotation[1], trans->rotation[2]);

  modelMatrix = glm::translate(modelMatrix, objectPosition);
  modelMatrix = glm::rotate(modelMatrix, rotationAngle, objectRotationAxis);
  modelMatrix = glm::scale(modelMatrix, objectScale);

  // Cambiar lo de subir la luz al uniform buffer de ahora
  UniformBlockMatricesPointLight matrices_tmp;
  matrices_tmp.proj_view_cam = projectionMatrix * viewMatrix;

  glm::vec3 pos = glm::make_vec3(l->GetPosition());
  float fov_radians = glm::radians(90.0f);
  float aspect_ratio = (float)m_shadows_buffer_->GetCreationInfo().Width / (float)m_shadows_buffer_->GetCreationInfo().Height;
  float near = 10.0f;
  float far = 310.0f;
  glm::mat4 projLight = glm::perspective(fov_radians, aspect_ratio, near, far);

  for(int i = 0; i < 6; i++){
    glm::vec3 dir = glm::make_vec3(m_directions->dir[i]);
    glm::vec3 up(0.0f, 1.0f, 0.0f);

    float dot = glm::dot(up, dir);
    dot = glm::abs(dot);
    if (dot == 1.0f) {
      up = glm::vec3(0.0f, 0.0f, 1.0f);
    }

    glm::vec3 right = glm::normalize(glm::cross(up, dir));
    up = glm::cross(dir, right);
    glm::mat4 viewLight = glm::lookAt(pos, pos + glm::normalize(dir), up);
    
    matrices_tmp.proj_view_light[i] = projLight * viewLight;
  }
  
  matrices_tmp.model = modelMatrix;
  matrices_tmp.camera_position = glm::make_vec3(cam->GetPosition());  
   
  //UniformBlockMatrices matrices_tmp = {modelMatrix, viewProjCam, projViewLight, cam_pos };

  //int32 size_matrix = shader_tmp->GetUniformBlockSize(EUniformBlockType::UniformBuffer0);
  // Cambiar el shader para que coincida con mi struct
  m_buffer_matrix_pointLight->upload_data((void*)&matrices_tmp, 524);
  m_buffer_matrix_pointLight->bind();
  m_buffer_point_light->upload_data(l->GetData(), 64);
  m_buffer_point_light->bind();

  unsigned int VBO = obj->MeshOBJ->get_vbo();
  unsigned int VAO = obj->MeshOBJ->get_vao();

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBindVertexArray(VAO);

  const std::vector<Vertex_info>& vertices = obj->MeshOBJ->getVertexInfo();

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_info), (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_info), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex_info), (void*)(6 * sizeof(float)));

  const std::vector<unsigned int>& indices = obj->MeshOBJ->getIndices();
  glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, indices.data());
}

void RendererOpenGL::draw_obj_shadows(MeshComponent* obj, TransformComponent* trans, DirectionalLight* l){
  
  //auto start = std::chrono::high_resolution_clock::now();
  CameraBase* cam = &m_DefaultCamera;
  if(m_UserCamera) cam = m_UserCamera;
  glm::mat4 viewMatrix = glm::make_mat4(cam->GetViewMatrix());
  glm::mat4 projectionMatrix = glm::make_mat4(cam->GetProjectionMatrix());
  glm::mat4 viewProjCam = projectionMatrix * viewMatrix;

  glm::mat4 modelMatrix = glm::mat4(1.0f);

  glm::vec3 objectPosition = glm::vec3(trans->position[0], trans->position[1], trans->position[2]);
  glm::vec3 objectScale = glm::vec3(trans->scale[0], trans->scale[1], trans->scale[2]);
  float rotationAngle = 0.0f;
  glm::vec3 objectRotationAxis = glm::vec3(trans->rotation[0], trans->rotation[1], trans->rotation[2]);

  modelMatrix = glm::translate(modelMatrix, objectPosition);
  modelMatrix = glm::rotate(modelMatrix, rotationAngle, objectRotationAxis);
  modelMatrix = glm::scale(modelMatrix, objectScale);

  // TODO add campo en lights para las matrices asi solo tengo que hacerlo una vez y me lo guardo
  
  glm::vec3 cam_pos = glm::make_vec3(cam->GetPosition());
  glm::vec3 light_dir = glm::make_vec3(l->GetDirection());
  glm::vec3 pos = glm::make_vec3(cam_pos + ( (-1.0f * light_dir) * 50.0f));
  

  glm::vec3 up(0.0f, 1.0f, 0.0f);
  glm::vec3 right = glm::normalize(glm::cross(up, light_dir));
  up = glm::cross(light_dir, right);
  glm::mat4 viewLight = glm::lookAt(pos, pos + glm::normalize(light_dir), up);

  glm::mat4 orto = glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, cam->GetNear(), cam->GetFar());
  glm::mat4 projViewLight = orto * viewLight;
  
  UniformBlockMatrices matrices_tmp = {modelMatrix, viewProjCam, projViewLight, cam_pos };

  //int32 size_matrix = shader_tmp->GetUniformBlockSize(EUniformBlockType::UniformBuffer0);
  m_buffer_matrix->upload_data((void*)&matrices_tmp, 208);
  m_buffer_matrix->bind();
  m_buffer_directional_light->upload_data(l->GetData(), 48);
  m_buffer_directional_light->bind();

  unsigned int VBO = obj->MeshOBJ->get_vbo();
  unsigned int VAO = obj->MeshOBJ->get_vao();

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBindVertexArray(VAO);

  const std::vector<Vertex_info>& vertices = obj->MeshOBJ->getVertexInfo();

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_info), (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_info), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex_info), (void*)(6 * sizeof(float)));

  const std::vector<unsigned int>& indices = obj->MeshOBJ->getIndices();
  glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, indices.data());
}

void RendererOpenGL::draw_scene(Scene& scene, Shader* s){
  EntityComponentSystem& ECS = scene.m_ECS;

  for (auto [transform, obj] : ECS.get_components<TransformComponent, MeshComponent>())
  {
    draw_obj(obj, nullptr, transform);
  }
}

void RendererOpenGL::draw_deep_obj(MeshComponent* obj, std::shared_ptr<Shader> s, TransformComponent* tran, float* view, float* projection){
  CameraBase* cam = &m_DefaultCamera;
  if(m_UserCamera) cam = m_UserCamera;

  OpenGLShader* shader_tmp = static_cast<OpenGLShader*>(s.get());
  shader_tmp->Use();
  
  glm::mat4 modelMatrix = glm::mat4(1.0f);

  glm::vec3 objectPosition = glm::vec3(tran->position[0], tran->position[1], tran->position[2]);
  glm::vec3 objectScale = glm::vec3(tran->scale[0], tran->scale[1], tran->scale[2]);
  float rotationAngle = 0.0f;
  glm::vec3 objectRotationAxis = glm::vec3(tran->rotation[0], tran->rotation[1], tran->rotation[2]);

  modelMatrix = glm::translate(modelMatrix, objectPosition);
  modelMatrix = glm::rotate(modelMatrix, rotationAngle, objectRotationAxis);
  modelMatrix = glm::scale(modelMatrix, objectScale);

  const float* tmp = cam->GetPosition();
  glm::vec3 cam_pos(tmp[0], tmp[1], tmp[2]);

 
  UniformBlockMatrices matrices_tmp = {modelMatrix, glm::make_mat4(view), glm::make_mat4(projection), cam_pos};
  m_buffer_matrix->upload_data((void*)&matrices_tmp, 208);
  m_buffer_matrix->bind();

  unsigned int VBO = obj->MeshOBJ->get_vbo();
  unsigned int VAO = obj->MeshOBJ->get_vao();
  //WAIT_GPU_LOAD();

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBindVertexArray(VAO);

  const std::vector<Vertex_info>& vertices = obj->MeshOBJ->getVertexInfo();

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_info), (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_info), (void*)(3 * sizeof(float)));

  const std::vector<unsigned int>& indices = obj->MeshOBJ->getIndices();
  glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, indices.data());

}

std::shared_ptr<RenderTarget> RendererOpenGL::get_shadow_buffer(){
  return m_shadows_buffer_;
}

std::vector<std::shared_ptr<RenderTarget>> RendererOpenGL::get_shadow_buffer_pointLight(){
  return m_shadows_buffer_pointLight;
}

void RendererOpenGL::draw_shadows(SpotLight* l, MeshComponent* obj, TransformComponent* tran) {

  float aspect_ratio = (float)m_shadows_buffer_->GetCreationInfo().Width / (float)m_shadows_buffer_->GetCreationInfo().Height;
  glm::mat4 view = glm::make_mat4(l->GetViewMatrix(aspect_ratio));
  glm::mat4 persp = glm::make_mat4(l->GetProjectMatrix(aspect_ratio));

  draw_deep_obj(obj, m_depth_shader, tran, glm::value_ptr(view), glm::value_ptr(persp));
}

void RendererOpenGL::draw_shadows(DirectionalLight* l, MeshComponent* obj, TransformComponent* tran) {  
  CameraBase* cam = &m_DefaultCamera;
  if(m_UserCamera) cam = m_UserCamera;
  glm::vec3 cam_pos = glm::make_vec3(cam->GetPosition());
  glm::vec3 light_dir = glm::make_vec3(l->GetDirection());

  float x = cam_pos.x + ( (-1.0f * light_dir.x) * 50.0f);
  float z = cam_pos.z + ( (-1.0f * light_dir.z) * 50.0f);
  
  glm::vec3 pos = glm::vec3(x, cam_pos.y, z);
  glm::vec3 up(0.0f, 1.0f, 0.0f);
  glm::vec3 right = glm::normalize(glm::cross(up, light_dir));
  up = glm::cross(light_dir, right);
  glm::mat4 viewLight = glm::lookAt(pos, pos + glm::normalize(light_dir), up);
  glm::mat4 orto = glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, cam->GetNear(), cam->GetFar());

  draw_deep_obj(obj, m_depth_shader, tran, glm::value_ptr(viewLight), glm::value_ptr(orto));
}

void RendererOpenGL::draw_shadows(PointLight* l, MeshComponent* obj, TransformComponent* tran, float* lightDir){  
  glm::vec3 pos = glm::make_vec3(l->GetPosition());
  glm::vec3 dir = glm::make_vec3(lightDir);
 
  // Para la directional, la posicion tiene que estar en la mitad del flusthrum en z, y en x e y tengo que sacar la posicion segun la direccion a la que viene la luz,
  // y luego ir moviendola ligeramente hasta sacar los valores correctos

  glm::vec3 up(0.0f, 1.0f, 0.0f);
  float dot = glm::dot(up, dir);
  dot = glm::abs(dot);
  if (dot == 1.0f) {
      up = glm::vec3(0.0f, 0.0f, 1.0f);
  }

  glm::vec3 right = glm::normalize(glm::cross(up, dir));
  up = glm::cross(dir, right);
  glm::mat4 view = glm::lookAt(pos, pos + glm::normalize(dir), up);
  int width = m_shadows_buffer_->GetCreationInfo().Width;
  int height = m_shadows_buffer_->GetCreationInfo().Height;

  float fov_radians = glm::radians(90.0f);
  float aspect_ratio = (float)width / (float)height;
  float near = 10.0f;
  float far = 310.0f;
  
  glm::mat4 persp = glm::perspective(fov_radians, aspect_ratio, near, far);

  draw_deep_obj(obj, m_depth_shader, tran, glm::value_ptr(view), glm::value_ptr(persp));
}

void RendererOpenGL::draw_forward(EntityComponentSystem& entity){

    std::shared_ptr<And::RenderTarget> shadow_buffer = get_shadow_buffer();

    // Ambient light
    glBlendFunc(GL_ONE, GL_ZERO);
    for(auto [light] : entity.get_components<AmbientLight>()){

      m_shader_ambient->Use();
      for (auto [transform, obj] : entity.get_components<And::TransformComponent, And::MeshComponent>()) {
        obj->MeshOBJ->UseTexture(1);
        OpenGLShader* tmp = static_cast<OpenGLShader*>(m_shader_ambient.get());
        tmp->SetTexture("texMaterial",1);
        draw_obj(obj, light, transform);
      }
      glBlendFunc(GL_ONE, GL_ONE);
    }
    //glEnable(GL_BLEND);


    // Shadows Directional 
    // Directional Light (should be 1)
    for(auto [light] : entity.get_components<DirectionalLight>()){ 
      shadow_buffer->Activate();
      glDisable(GL_BLEND);
      if(light->GetCastShadows()){
        // Por cada luz que castea sombras guardamos textura de profundidad
        for (auto [transform, obj] : entity.get_components<And::TransformComponent, And::MeshComponent>()){
          draw_shadows(light, obj, transform);
        }
      }
      shadow_buffer->Desactivate();
      glEnable(GL_BLEND);

      // Render Directional
      for (auto [light] : entity.get_components<DirectionalLight>()) {
          for (auto [transform, obj] : entity.get_components<And::TransformComponent, And::MeshComponent>()) {
              if (light->GetCastShadows()) {
                  std::vector<std::shared_ptr<And::Texture>> shadow_texture = shadow_buffer->GetTextures();
                  OpenGLShader* tmp = static_cast<OpenGLShader*>(m_shader_shadows_directional.get());
                  OpenGLTexture2D* tex = static_cast<OpenGLTexture2D*>(shadow_texture[0].get());

                  tmp->Use();
                  tex->Activate(0);
                  tmp->SetTexture("texShadow", 0);

                  obj->MeshOBJ->UseTexture(1);
                  tmp->SetTexture("texMaterial",1);                  

                  draw_obj_shadows(obj, transform, light);
              }else {
                  m_shader_directional->Use();
                  
                  obj->MeshOBJ->UseTexture(1);
                  OpenGLShader* tmp = static_cast<OpenGLShader*>(m_shader_directional.get());
                  tmp->SetTexture("texMaterial",1);
                  
                  draw_obj(obj, light, transform);
              }
          }
          glBlendFunc(GL_ONE, GL_ONE);
      }

    }
    // -----------------------


    // Shadows spot light
    // Spot Lights
    for(auto [light] : entity.get_components<SpotLight>()){
      shadow_buffer->Activate();
      glDisable(GL_BLEND);
      if(light->GetCastShadows()){
        // Por cada luz que castea sombras guardamos textura de profundidad
        for (auto [transform, obj] : entity.get_components<And::TransformComponent, And::MeshComponent>()){
          draw_shadows(light, obj, transform);
        }
      }
      // Cuando saque la sombra de una spot, tengo que renderizar para que cuando saque la segunda no se sobreescriba el buffer
      shadow_buffer->Desactivate();
      glEnable(GL_BLEND);

      // Render SpotLight 
      for (auto [transform, obj] : entity.get_components<And::TransformComponent, And::MeshComponent>()) {
            if (light->GetCastShadows()) {
                std::vector<std::shared_ptr<And::Texture>> shadow_texture = shadow_buffer->GetTextures();
                OpenGLShader* tmp = static_cast<OpenGLShader*>(m_shader_shadows_spot.get());
                OpenGLTexture2D* tex = static_cast<OpenGLTexture2D*>(shadow_texture[0].get());
                tmp->Use();
                tex->Activate(0);
                tmp->SetTexture("texShadow", 0);
                
                obj->MeshOBJ->UseTexture(1);
                tmp->SetTexture("texMaterial",1);
                  
                draw_obj_shadows(obj, transform, light);
            }
            else {
                m_shader_spot->Use();
                
                obj->MeshOBJ->UseTexture(1);
                OpenGLShader* tmp = static_cast<OpenGLShader*>(m_shader_spot.get());
                tmp->SetTexture("texMaterial",1);
                
                draw_obj(obj, light, transform);
            }
        }
        glBlendFunc(GL_ONE, GL_ONE);

    }

    // Coger vector de shadow buffer de la point light
    /* Shadows Point light*/
    std::vector<std::shared_ptr<And::RenderTarget>> render_targets = get_shadow_buffer_pointLight();
      
    for(auto [light] : entity.get_components<PointLight>()){
      glDisable(GL_BLEND);
      if(light->GetCastShadows()){
          int index = 0;
          for (auto& target : render_targets) {
              target->Activate();
              // Por cada luz que castea sombras guardamos textura de profundidad, aqui hay que hacerlo en 6 shadow buffers, uno por cada cara de la point
              for (auto [transform, obj] : entity.get_components<And::TransformComponent, And::MeshComponent>()) {
                  draw_shadows(light, obj, transform, glm::value_ptr(m_directions->dir[index]));
              }
              index++;
              target->Desactivate();
          }
      }
      glEnable(GL_BLEND);
      
      /* Render PointLight */
      for (auto [transform, obj] : entity.get_components<And::TransformComponent, And::MeshComponent>()) {
            if (light->GetCastShadows()) {
                int index = 0;
                OpenGLShader* tmp = static_cast<OpenGLShader*>(m_shader_shadows_point.get());
                for(auto& target : render_targets){

                  std::vector<std::shared_ptr<And::Texture>> shadow_texture = target->GetTextures();
                  OpenGLTexture2D* tex = static_cast<OpenGLTexture2D*>(shadow_texture[0].get());
                  tmp->Use();
                  tex->Activate(index);
                  tmp->SetTextureInArray("texShadow", index, index);
                  index++;
                }
                obj->MeshOBJ->UseTexture(index);
                tmp->SetTexture("texMaterial",index);
                
                draw_obj_shadows(obj, transform, light, glm::value_ptr(m_directions->dir[index]));
            
            }else {
                m_shader_point->Use();
                
                obj->MeshOBJ->UseTexture(1);
                OpenGLShader* tmp = static_cast<OpenGLShader*>(m_shader_point.get());
                tmp->SetTexture("texMaterial",1);
                

                draw_obj(obj, light, transform);
            }
        }
        glBlendFunc(GL_ONE, GL_ONE);
    }
    
}

void RendererOpenGL::draw_deferred(EntityComponentSystem& entity) {
    
  // Geometry Passs
  m_gBuffer_->Activate();

  // Le decimos el color attachment que vamos a usar en este framebuffer para el render
  unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
  glDrawBuffers(3, attachments);

  m_shader_geometry->Use();
  glDisable(GL_BLEND);
  for (auto [transform, obj] : entity.get_components<And::TransformComponent, And::MeshComponent>()) {
    obj->MeshOBJ->UseTexture(0);
    OpenGLShader* tmp = static_cast<OpenGLShader*>(m_shader_geometry.get());
    tmp->Use();
    
    draw_obj(obj, nullptr, transform);
  }
  m_gBuffer_->Desactivate();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


  // Lighting Pass
  float dMesh[] = {
      -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // down-left
      -1.0f, +1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // up-left
      +1.0f, +1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // up-right
      +1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // down-right
  };

  unsigned int dIndices[] = {
       0, 2, 1, 0, 3, 2
  };

  // Shadows directional
  std::shared_ptr<And::RenderTarget> shadow_buffer = get_shadow_buffer();
  for (auto [light] : entity.get_components<DirectionalLight>()) {
    shadow_buffer->Activate();
    glDisable(GL_BLEND);
    if (light->GetCastShadows()) {
        // Por cada luz que castea sombras guardamos textura de profundidad
        for (auto [transform, obj] : entity.get_components<And::TransformComponent, And::MeshComponent>()) {
            draw_shadows(light, obj, transform);
        }
    }
    shadow_buffer->Desactivate();
    glEnable(GL_BLEND);


    // Render Directional
    OpenGLShader* tmp = static_cast<OpenGLShader*>(m_shader_quad.get());
    std::vector<std::shared_ptr<Texture>> tex_gbuffer = m_gBuffer_->GetTextures();
    OpenGLTexture2D* position_tex = static_cast<OpenGLTexture2D*>(tex_gbuffer[0].get());
    OpenGLTexture2D* normal_tex = static_cast<OpenGLTexture2D*>(tex_gbuffer[1].get());
    OpenGLTexture2D* color_tex = static_cast<OpenGLTexture2D*>(tex_gbuffer[2].get());
    
    std::vector<std::shared_ptr<And::Texture>> shadow_texture = shadow_buffer->GetTextures();
    OpenGLTexture2D* tex_shadow = static_cast<OpenGLTexture2D*>(shadow_texture[0].get());

    // posicion, normal, color
    tmp->Use();

    tmp->SetTexture("Frag_Position", 0);
    position_tex->Activate(0);

    tmp->SetTexture("Frag_Normal", 1);
    normal_tex->Activate(1);

    tmp->SetTexture("Frag_Color", 2);
    color_tex->Activate(2);

    tmp->SetTexture("texShadow", 3);
    tex_shadow->Activate(3);

    glBindVertexArray(m_quad_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_quad_vbo);
    glBufferData(GL_ARRAY_BUFFER, (GLsizei)(sizeof(dMesh)), &dMesh[0], GL_STATIC_DRAW);


    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_info), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_info), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex_info), (void*)(6 * sizeof(float)));

    OpenGLRenderTarget* opengl_render_target = static_cast<OpenGLRenderTarget*>(m_gBuffer_.get());

    glEnable(GL_BLEND);
    
    upload_light(light);
    glBindVertexArray(m_quad_vao);
    glDrawElements(GL_TRIANGLES, (GLsizei)(sizeof(dMesh)), GL_UNSIGNED_INT, &dIndices[0]);
    glBlendFunc(GL_ONE, GL_ONE);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, opengl_render_target->GetId());
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBlitFramebuffer(0, 0, 1920, 1080, 0, 0, 1920, 1080, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
      

  }


  
}

}