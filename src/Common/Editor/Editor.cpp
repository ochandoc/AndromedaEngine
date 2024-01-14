#include "Common/Editor/Editor.h"
#include "Common/Log.h"
#include "Common/ImGuiSink.h"
#include "Common/ShaderTextEditor.h"

#include "Common/UI/Plot/implot.h"

struct RollingBuffer {
	float Span;
	ImVector<ImVec2> Data;
	RollingBuffer() {
		Span = 10.0f;
		Data.reserve(2000);
	}
	void AddPoint(float x, float y) {
		float xmod = fmodf(x, Span);
		if (!Data.empty() && xmod < Data.back().x)
			Data.shrink(0);
		Data.push_back(ImVec2(xmod, y));
	}
};

namespace And{

Editor::Editor(){

  // Logger, shader editor, job system, content browser
	m_Windows.insert({ "Console Log 1", std::shared_ptr<LogWindow>(new LogWindow("Console Log 1")) });
	m_Windows.insert({ "Console Log 2", std::shared_ptr<LogWindow>(new LogWindow("Console Log 2")) });
	m_Windows.insert({ "Console Log 3", std::shared_ptr<LogWindow>(new LogWindow("Console Log 3")) });
	m_Windows.insert({ "Console Log 4", std::shared_ptr<LogWindow>(new LogWindow("Console Log 4")) });
	m_Windows.insert({ "Shader Editor 1", std::shared_ptr<ShaderTextEditor>(new ShaderTextEditor("Shader Editor 1")) });
	m_Windows.insert({ "Shader Editor 2", std::shared_ptr<ShaderTextEditor>(new ShaderTextEditor("Shader Editor 2")) });
}

Editor::~Editor(){

}

void Editor::ShowWindows(){
	static ImGuiID s_Dockspace;

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

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Windows"))
		{
			if (ImGui::BeginMenu("Log"))
			{
				ImGui::MenuItem("Console Log 1", nullptr, &m_Windows["Console Log 1"]->m_is_open);
				ImGui::MenuItem("Console Log 2", nullptr, &m_Windows["Console Log 2"]->m_is_open);
				ImGui::MenuItem("Console Log 3", nullptr, &m_Windows["Console Log 3"]->m_is_open);
				ImGui::MenuItem("Console Log 4", nullptr, &m_Windows["Console Log 4"]->m_is_open);
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Code Editor"))
			{
				if (ImGui::BeginMenu("Shaders"))
				{
					ImGui::MenuItem("Shader Editor 1", nullptr, &m_Windows["Shader Editor 1"]->m_is_open);
					ImGui::MenuItem("Shader Editor 2", nullptr, &m_Windows["Shader Editor 2"]->m_is_open);
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	for (auto& [name, window] : m_Windows)
	{
		window->Show();
	}

	static float history = 10.0f;
	static RollingBuffer rb;
	static float t = 0;

	t += 0.0005f;
	rb.AddPoint(t, 0.0f);
	rb.Span = history;

	if (ImGui::Begin("Plot"))
	{
		if (ImPlot::BeginPlot("##Rolling", ImVec2(-1, 150))) {
			ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_NoTickLabels, ImPlotAxisFlags_NoTickLabels);
			ImPlot::SetupAxisLimits(ImAxis_X1, 0, history, ImGuiCond_None);
			ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 1);
			ImPlot::PlotLine("Mouse X", &rb.Data[0].x, &rb.Data[0].y, rb.Data.size(), 0, 0, 2 * sizeof(float));
			ImPlot::EndPlot();
		}
	}
	ImGui::End();
  
}
}