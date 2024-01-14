#include "Common/Editor/EditorWindow.h"
#include "Common/Editor/Editor.h"

#include "Common/inicpp.h"

namespace And{


EditorWindow::EditorWindow(const std::string& Title) : m_title(Title)
{
  ini::IniFile file;
  file.load("./Editor.config");
	if (!file.empty())
	{
		if (!file[Title].empty())
		{
			m_is_open = file[Title]["Open"].as<bool>();
		}
		else
		{
			m_is_open = false;
		}
	}
	else
	{
		m_is_open = false;
	}
}

EditorWindow::~EditorWindow()
{
	ini::IniFile file;
	file.load("./Editor.config");
	file[m_title]["Open"] = m_is_open;
	file.save("./Editor.config");
}


void EditorWindow::SetEditor(Editor* e){
  m_Editor = e;
}

}