#include "weapon_menu.h"
#include "features/weapon.hpp"

namespace big
{
	void weapon_menu::render_menu()
	{
		ImGui::BeginGroup();
		ImGui::Checkbox("Infinite Frostcraft", &g_settings->weapon.inf_frostcraft);
		ImGui::Checkbox("Max Sharpness", &g_settings->player.max_sharpness);
		ImGui::Checkbox("Maxed Stat Set", &g_settings->player.maxed_stat_set);
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("These will trigger when you hover over a piece of equipment, when you view the 'Change Equipment' menu, OR(Set Bonus) when you press 'ESC'. I find that pressing 'ESC' is the easiest method to use, since you can just equip an armor set, enable the script and press 'ESC' for everything to take effect. You can then disable the script as to not screw with anything else. Skills will still need to be hovered over.");
		ImGui::Checkbox("Maxed Armor Effect", &g_settings->player.maxed_armor_effect);
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("These will trigger when you hover over a piece of equipment, when you view the 'Change Equipment' menu, OR(Set Bonus) when you press 'ESC'. I find that pressing 'ESC' is the easiest method to use, since you can just equip an armor set, enable the script and press 'ESC' for everything to take effect. You can then disable the script as to not screw with anything else. Skills will still need to be hovered over.");
		ImGui::EndGroup();

		ImGui::SameLine(200.f);

		ImGui::BeginGroup();
		ImGui::Checkbox("Max Slot Armor", &g_settings->weapon.max_slot_armor);
		ImGui::Checkbox("Inf Slinger Ammo", &g_settings->weapon.inf_slinger_ammo);
		ImGui::EndGroup();

		ImGui::BeginGroup();
		ImGui::Checkbox("Frostcraft Recharge", &g_settings->weapon.frostcraft_recharge);
		ImGui::InputFloat("Frostcraft Drawn", &g_settings->weapon.frostcraft_drawn, 1.f, 10.f);
		ImGui::InputFloat("Frostcraft Sheathed", &g_settings->weapon.frostcraft_heat, 1.f, 10.f);
		ImGui::EndGroup();
	}
}