#include "Common/Renderer.h"
#include "Common/Window.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "imgui_impl_opengl3.h"

#include "OpenGLShader.h"
#include "Common/Shader.h"

namespace And
{

Renderer::Renderer(Window& window) : m_Window(window)
{
	static float default_color[] = { 0.094f, 0.094f, 0.094f, 1.0f };
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
  ImGui::ShowDemoWindow();

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

}

std::shared_ptr<Shader> Renderer::createShader(std::vector<ShaderInfo> s_info){
  return std::shared_ptr<Shader>(new OpenGLShader(s_info));
}



}
