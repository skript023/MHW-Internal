#pragma once
#include "core/commands/bool_command.hpp"
#include "core/commands/float_command.hpp"
#include "core/commands/int_command.hpp"
#include "core/commands/number_command.hpp"

#include <imgui.h>

namespace big::features
{
	// Legacy ImGui menus toggle commands through these instead of a settings
	// field, so on_enable/on_disable still runs.
	inline void checkbox(const char* label, bool_command& command)
	{
		bool state = command.get_state();
		if (ImGui::Checkbox(label, &state))
			command.set_state(state);
	}

	inline void input_int(const char* label, int_command& command, int step = 1, int step_fast = 100)
	{
		int state = command.get_state();
		if (ImGui::InputInt(label, &state, step, step_fast))
			command.set_state(state);
	}

	inline void input_int(const char* label, number_command<int>& command, int step = 1, int step_fast = 100)
	{
		int state = command.get_state();
		if (ImGui::InputInt(label, &state, step, step_fast))
			command.set_state(state);
	}

	inline void input_float(const char* label, float_command& command, float step = 0.1f, float step_fast = 1.0)
	{
		float state = command.get_state();
		if (ImGui::InputFloat(label, &state, step, step_fast))
			command.set_state(state);
	}
}
