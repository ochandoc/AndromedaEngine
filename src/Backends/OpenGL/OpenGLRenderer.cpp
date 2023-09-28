#include "OpenGLRenderer.h"
#include "GL/glew.h"

#include "Core/Window.h"

#include "imgui_impl_opengl3.h"

namespace And
{

OpenGLRenderer::OpenGLRenderer(Window& window) : Renderer(window)
{
  static float default_color[4] = {1.0f, 1.0f, 1.0f, 1.0f};
  set_clear_color(default_color);
  ImGui_ImplOpenGL3_Init("#version 430");
}

OpenGLRenderer::~OpenGLRenderer(){
  m_ImGuiImpl.release();
}

void OpenGLRenderer::new_frame()
{
	static ImGuiID s_Dockspace;
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  ImGui_ImplOpenGL3_NewFrame(); 
  m_ImGuiImpl->new_frame();
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
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	if (ImGui::Begin("WindowDockspace", nullptr, flags))
	{
		s_Dockspace = ImGui::GetID("Window_Dockspace");
		ImGui::DockSpace(s_Dockspace, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_PassthruCentralNode);
	}
	ImGui::End();
	ImGui::PopStyleVar(2);
}

void OpenGLRenderer::end_frame()
{
  ImGui::ShowDemoWindow();

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  m_ImGuiImpl->end_frame();
}

void OpenGLRenderer::set_viewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height){
  glViewport(x, y, width, height);
}
  
void OpenGLRenderer::set_clear_color(float* color){
  glClearColor(color[0], color[1], color[2], color[3]);
}

}
