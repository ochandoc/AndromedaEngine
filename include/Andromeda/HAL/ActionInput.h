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
		/**
		 * @brief Construct a new Action Input object with a name and a key state
		 * 
		 * @param name 
		 * @param state 
		 */
		ActionInput(std::string name, KeyState state);

		/**
		 * @brief Construct a new Action Input object with a name and a vector of states
		 * 
		 * @param name 
		 * @param state 
		 * @param keys 
		 */
		ActionInput(std::string name, KeyState state, std::vector<KeyCode> keys);

		~ActionInput();
		
		/**
		 * @brief Add key to check input
		 * 
		 * @param key 
		 */
		void add_key(KeyCode key);

		/**
		 * @brief Get the name of the action input
		 * 
		 * @return std::string& 
		 */
		std::string& get_name();

		friend class Input;
	private:
		std::vector<KeyCode> m_Keys;
		std::string m_Name;
		KeyState m_State;
	};
}