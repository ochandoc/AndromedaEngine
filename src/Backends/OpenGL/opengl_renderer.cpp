#include "Common/Renderer.h"
#include "Common/Window.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"

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
  m_camera_pos[2] = -10.0f;

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

  
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  unsigned int VBO;
  glGenBuffers(1, &VBO);
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

void Renderer::init_obj(ObjLoader* obj){

  if(obj->get_vao() == 0){
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    obj->set_VAO(VAO);
  }

  if(obj->get_vbo() == 0){
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    obj->set_VBO(VBO);

    glBindVertexArray(obj->get_vao());
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    std::vector<float> vertices = obj->getVertices();  
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    // Posiciones x y z
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3 ,GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    


    // Desbindeamos el vao
    glBindVertexArray(0);


  }

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

void Renderer::draw_obj(ObjLoader obj, Shader* s){

  CheckError();

  // Configura la matriz de vista manualmente
  GLfloat viewMatrix[16]; // Una matriz de 4x4

  // Calcula la dirección hacia la que mira la cámara (un vector en la dirección opuesta a cameraTarget)
  GLfloat direction[3] = {m_camera_pos[0] - m_camera_target[0], m_camera_pos[1] - m_camera_target[1], m_camera_pos[2] - m_camera_target[2]};
  GLfloat up[3] = {0.0f, 1.0f, 0.0f}; // Vector hacia arriba

  // Normaliza la dirección
  GLfloat directionLength = sqrt(direction[0] * direction[0] + direction[1] * direction[1] + direction[2] * direction[2]);
  direction[0] /= directionLength;
  direction[1] /= directionLength;
  direction[2] /= directionLength;

  // Calcula el vector derecho (cross product entre up y dirección)
  GLfloat right[3];
  right[0] = up[1] * direction[2] - up[2] * direction[1];
  right[1] = up[2] * direction[0] - up[0] * direction[2];
  right[2] = up[0] * direction[1] - up[1] * direction[0];

  // Normaliza el vector derecho
  GLfloat rightLength = sqrt(right[0] * right[0] + right[1] * right[1] + right[2] * right[2]);
  right[0] /= rightLength;
  right[1] /= rightLength;
  right[2] /= rightLength;

  // Calcula el nuevo vector hacia arriba (cross product entre dirección y derecho)
  up[0] = direction[1] * right[2] - direction[2] * right[1];
  up[1] = direction[2] * right[0] - direction[0] * right[2];
  up[2] = direction[0] * right[1] - direction[1] * right[0];

  // Construye la matriz de vista
  viewMatrix[0] = right[0];
  viewMatrix[1] = up[0];
  viewMatrix[2] = direction[0];
  viewMatrix[3] = 0.0f;

  viewMatrix[4] = right[1];
  viewMatrix[5] = up[1];
  viewMatrix[6] = direction[1];
  viewMatrix[7] = 0.0f;

  viewMatrix[8] = right[2];
  viewMatrix[9] = up[2];
  viewMatrix[10] = direction[2];
  viewMatrix[11] = 0.0f;

  viewMatrix[12] = -right[0] * m_camera_pos[0] - right[1] * m_camera_pos[1] - right[2] * m_camera_pos[2];
  viewMatrix[13] = -up[0] * m_camera_pos[0] - up[1] * m_camera_pos[1] - up[2] * m_camera_pos[2];
  viewMatrix[14] = -direction[0] * m_camera_pos[0] - direction[1] * m_camera_pos[1] - direction[2] * m_camera_pos[2];
  viewMatrix[15] = 1.0f;

  if(s){
    s->use();
  }


  float f = 1.0f / tan(m_fov * 0.5f * (dPI / 180.0f));

  // Perspectiva
  float projectionMatrix[16] = {
    f / m_aspectRatio, 0.0f, 0.0f, 0.0f,
    0.0f, f, 0.0f, 0.0f,
    0.0f, 0.0f, (m_far + m_near) / (m_near - m_far), -1.0f,
    0.0f, 0.0f, (2.0f * m_far * m_near) / (m_near - m_far), 0.0f
  };

   CheckError();
  s->setMat4("view", viewMatrix);
  CheckError();
  

  CheckError();
  s->setMat4("projection", projectionMatrix);
  CheckError();

  //s->setVec3("normals", &normals[0]);
  //CheckError();
  /*
  glm::vec3 cameraPosition{m_camera_pos[0], m_camera_pos[1], m_camera_pos[2]};

  glm::mat4 viewMatrix = glm::lookAt(cameraPosition, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  */
  //glEnable(GL_FRONT_AND_BACK);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  //glDisable(GL_CULL_FACE);

  CheckError(); // Aqui tiene invalid operation


  CheckError();

  
  CheckError();

  unsigned int VBO, VAO;
  VAO = obj.get_vao();
  VBO = obj.get_vbo();


  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBindVertexArray(VAO);

  CheckError();

  std::vector<float> vertices = obj.getVertices();  
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3 ,GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


/*
  std::vector<float> normals = obj.getNormals();
  glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), &normals[0], GL_STATIC_DRAW);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1,3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

  s->setVec3("normals", normals.data());
*/
  CheckError();


 
  //glDrawArrays(GL_TRIANGLES, vertices[0], vertices.size() / 3);

  //glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 3);

  std::vector<unsigned int> indices = obj.getIndices();
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, indices.data());

  CheckError();
  glBindVertexArray(0);

  //glBindBuffer(GL_ARRAY_BUFFER, 0);


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


}
