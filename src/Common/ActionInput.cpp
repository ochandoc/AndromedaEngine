#include "Common/ActionInput.h"

namespace And
{
	ActionInput::ActionInput(std::string name, KeyState state) : m_Name(name), m_State(state)	{}

	ActionInput::ActionInput(std::string name, KeyState state, std::vector<KeyCode> keys) : m_Name(name), m_Keys(keys), m_State(state) {}

	ActionInput::~ActionInput() {}

	void ActionInput::add_key(KeyCode key)
	{
		m_Keys.push_back(key);
	}

	std::string& ActionInput::get_name()
	{
		return m_Name;
	}
}