#pragma once

#include "Andromeda/Misc/CoreMiscDefines.h"
#include "Andromeda/HAL/Types.h"

#include "KeyCodes.h"

namespace And
{
	class ActionInput
	{
		NON_COPYABLE_CLASS(ActionInput)
		NON_MOVABLE_CLASS(ActionInput)
	public:
		ActionInput(std::string name, KeyState state);
		ActionInput(std::string name, KeyState state, std::vector<KeyCode> keys);

		~ActionInput();

		void add_key(KeyCode key);

		std::string& get_name();

		friend class Input;
	private:
		std::vector<KeyCode> m_Keys;
		std::string m_Name;
		KeyState m_State;
	};
}