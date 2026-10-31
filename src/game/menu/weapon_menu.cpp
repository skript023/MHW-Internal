#include "game/features/ui.hpp"
#include "game/features/features.hpp"
#include <imgui.h>
#include "weapon_menu.h"


namespace big
{
	void weapon_menu::render_menu()
	{
		ImGui::BeginGroup();
		features::checkbox("Infinite Frostcraft", features::_inf_frostcraft);
		features::checkbox("Max Sharpness", features::_max_sharpness);
		features::checkbox("Maxed Stat Set", features::_maxed_stat_set);
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("These will trigger when you hover over a piece of equipment, when you view the 'Change Equipment' menu, OR(Set Bonus) when you press 'ESC'. I find that pressing 'ESC' is the easiest method to use, since you can just equip an armor set, enable the script and press 'ESC' for everything to take effect. You can then disable the script as to not screw with anything else. Skills will still need to be hovered over.");
		features::checkbox("Maxed Armor Effect", features::_maxed_armor_effect);
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("These will trigger when you hover over a piece of equipment, when you view the 'Change Equipment' menu, OR(Set Bonus) when you press 'ESC'. I find that pressing 'ESC' is the easiest method to use, since you can just equip an armor set, enable the script and press 'ESC' for everything to take effect. You can then disable the script as to not screw with anything else. Skills will still need to be hovered over.");
		ImGui::EndGroup();

		ImGui::SameLine(200.f);

		ImGui::BeginGroup();
		features::checkbox("Max Slot Armor", features::_max_slot_armor);
		features::checkbox("Inf Slinger Ammo", features::_inf_slinger_ammo);
		ImGui::EndGroup();

		ImGui::BeginGroup();
		features::checkbox("Frostcraft Recharge", features::_frostcraft_recharge);
		features::input_float("Frostcraft Drawn", features::_frostcraft_drawn, 1.f, 10.f);
		features::input_float("Frostcraft Sheathed", features::_frostcraft_heat, 1.f, 10.f);
		ImGui::EndGroup();
	}
}