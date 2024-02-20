#include "Andromeda/Editor/Editor.h"
#include "Andromeda/Debug/Log.h"
#include "Andromeda/Debug/ImGuiSink.h"
#include "Andromeda/Editor/ShaderTextEditor.h"
#include "Andromeda/Editor/Windows/GameInfoWindow.h"

#include "Andromeda/UI/Plot/implot.h"



namespace And{

Editor::Editor(Window& window, ResourceManager* rm) : m_MainWindow(window), m_resourceManager(rm)
{

  // Logger, OldShader editor, job system, content browser
	m_Windows.insert({ "Console Log 1", std::shared_ptr<LogWindow>(new LogWindow("Console Log 1")) });
	m_Windows.insert({ "Console Log 2", std::shared_ptr<LogWindow>(new LogWindow("Console Log 2")) });
	m_Windows.insert({ "Console Log 3", std::shared_ptr<LogWindow>(new LogWindow("Console Log 3")) });
	m_Windows.insert({ "Console Log 4", std::shared_ptr<LogWindow>(new LogWindow("Console Log 4")) });
	m_Windows.insert({ "OldShader Editor 1", std::shared_ptr<ShaderTextEditor>(new ShaderTextEditor("OldShader Editor 1")) });
	m_Windows.insert({ "OldShader Editor 2", std::shared_ptr<ShaderTextEditor>(new ShaderTextEditor("OldShader Editor 2")) });
	m_Windows.insert({ "Game Info Window", std::shared_ptr<GameInfoWindow>(new GameInfoWindow()) });

	for (auto& [name, window] : m_Windows)
	{
		window->m_Editor = this;
		window->m_Window = &m_MainWindow;
		window->m_resourceManager = rm;
	}

	// Debug
	std::static_pointer_cast<ShaderTextEditor>(m_Windows["OldShader Editor 1"])->Load("default/deafult_OldShader.OldShader");
}

Editor::~Editor(){

}

void Editor::AddWindow(std::shared_ptr<EditorWindow> window)
{
	window->m_Editor = this;
	window->m_Window = &m_MainWindow;
	window->m_resourceManager = m_resourceManager;
	m_Windows.insert({window->m_title, window});
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
				if (ImGui::BeginMenu("OldShaders"))
				{
					ImGui::MenuItem("OldShader Editor 1", nullptr, &m_Windows["OldShader Editor 1"]->m_is_open);
					ImGui::MenuItem("OldShader Editor 2", nullptr, &m_Windows["OldShader Editor 2"]->m_is_open);
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Profiling"))
			{
				if (m_Windows.contains("Task System Info"))
				{
					ImGui::MenuItem("Task System Info", nullptr, &m_Windows["Task System Info"]->m_is_open);
				}
				ImGui::EndMenu();
			}

			if(ImGui::BeginMenu("Game Info")){
				ImGui::MenuItem("Game info window", nullptr, &m_Windows["Game Info Window"]->m_is_open);	
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
  
}
}