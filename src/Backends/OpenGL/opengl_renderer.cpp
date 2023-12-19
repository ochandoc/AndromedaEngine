#include "Common/Renderer.h"
#include "Common/Window.h"

#include "Backends/OpenGL/OpenGL.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui_impl_opengl3.h"

#include "Common/Shader.h"
#include "Common/Triangle.h"
#include "Common/ObjLoader.h"

namespace And
{

Renderer::Renderer(Window& window) : m_Window(window) 
{
  static float default_color[] = { 0.094f, 0.094f, 0.094f, 1.0f };
  m_camera_pos[0] = 0.0f;
  m_camera_pos[1] = 7.0f;
  m_camera_pos[2] = -60.0f;

  m_camera_target[0] = 0.0f;
  m_camera_target[1] = 0.0f;
  m_camera_target[2] = 0.0f;
  m_fov = 45.0f;

  GLFWwindow *window_tmp = (GLFWwindow*) m_Window.get_native_window();
  int width, height;
  glfwGetWindowSize(window_tmp, &width, &height);
  m_aspectRatio = width/height;

  m_near = 0.1f;
  m_far = 100.0f;



  set_clear_color(default_color);
  window.imgui_start();
  ImGui_ImplOpenGL3_Init("#version 430");
}

Renderer::~Renderer(){
	m_Window.imgui_end();
}

void Renderer::new_frame()
{
	static ImGuiID s_Dockspace;
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  ImGui_ImplOpenGL3_NewFrame(); 
	m_Window.new_frame();
  ImGui::NewFrame();

	ImGuiWindowFlags flags = ImGuiWindowFlags_None;
	flags |= ImGuiWindowFlags_NoTitleBar;
	flags |= ImGuiWindowFlags_NoResize;
	flags |= ImGuiWindowFlags_NoMove;
	flags |= ImGuiWindowFlags_NoScrollbar;
	flags |= ImGuiWindowFlags_NoScrollWithMouse;
	flags |= ImGuiWindowFlags_NoCollapse;
	flags |= ImGuiWindowFlags_NoBackground;
	flags |= ImGuiWindowFlags_NoSavedSettings;
	flags |= ImGuiWindowFlags_NoDecoration;
	flags |= ImGuiWindowFlags_NoInputs;
	flags |= ImGuiWindowFlags_NoNav;
	flags |= ImGuiWindowFlags_NoFocusOnAppearing;
	flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;

	ImGuiViewport* viewport = ImGui::GetMainViewport();

	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(1.0f, 1.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	if (ImGui::Begin("WindowDockspace", nullptr, flags))
	{
		s_Dockspace = ImGui::GetID("WindowDockspace");
		ImGui::DockSpace(s_Dockspace, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_PassthruCentralNode);
	}
	ImGui::End();
	ImGui::PopStyleVar(3);
}

void Renderer::end_frame()
{
  //ImGui::ShowDemoWindow();
  if(ImGui::CollapsingHeader("Camera")){
    ImGui::DragFloat3("Camera position", m_camera_pos);
    ImGui::DragFloat3("Camera target", m_camera_target);
    ImGui::DragFloat("FOV", &m_fov);
  }

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	m_Window.end_frame();
}

void Renderer::set_viewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height){
  glViewport(x, y, width, height);
}
  
void Renderer::set_clear_color(float* color){
  glClearColor(color[0], color[1], color[2], color[3]);
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

void Renderer::draw_obj(ObjLoader obj, Shader* s, Transform tran) {

  if(s){
    s->use();
  }

  
  //glCullFace(GL_CW);
  //glEnable(GL_FRONT_AND_BACK);

  //glDisable(GL_CULL_FACE)

  glm::vec3 cameraPosition(m_camera_pos[0], m_camera_pos[1], m_camera_pos[2]);
  glm::vec3 cameraTarget(m_camera_target[0], m_camera_target[1], m_camera_target[2]);
  glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);

  glm::mat4 viewMatrix = glm::lookAt(cameraPosition, cameraTarget, cameraUp);
  glm::mat4 projectionMatrix = glm::perspective(glm::radians(m_fov), m_aspectRatio, m_near, m_far);

  glm::mat4 modelMatrix = glm::mat4(1.0f);

  glm::vec3 objectPosition = glm::vec3(tran.position[0], tran.position[1], tran.position[2]);
  glm::vec3 objectScale = glm::vec3(tran.scale[0], tran.scale[1], tran.scale[2]);
  float rotationAngle = 0.0f;
  glm::vec3 objectRotationAxis = glm::vec3(tran.rotation[0], tran.rotation[1], tran.rotation[2]);

  modelMatrix = glm::translate(modelMatrix, objectPosition);
  modelMatrix = glm::rotate(modelMatrix, rotationAngle, objectRotationAxis);
  modelMatrix = glm::scale(modelMatrix, objectScale);

  s->setMat4("view", glm::value_ptr(viewMatrix));
  s->setMat4("projection", glm::value_ptr(projectionMatrix));
  s->setMat4("model", glm::value_ptr(modelMatrix));

  unsigned int VBO = obj.get_vbo();
  unsigned int VAO = obj.get_vao();

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBindVertexArray(VAO);


  std::vector<Vertex_info> vertices = obj.getVertexInfo();  

  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex_info), &vertices[0], GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_info), (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_info), (void*)(3 * sizeof(float)));


  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glEnable(GL_DEPTH_TEST);

  std::vector<unsigned int> indices = obj.getIndices();
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, indices.data());

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


}
