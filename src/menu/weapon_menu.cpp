#include "weapon_menu.h"

namespace big
{
	void weapon_menu::render_menu()
	{
		ImGui::BeginGroup();

		ImGui::Checkbox("Infinite Frostcraft", &g_settings->weapon.inf_frostcraft);

		ImGui::Checkbox("Max Sharpness", &g_settings->player.max_sharpness);

		ImGui::Checkbox("Maxed Stat Set", &g_settings->player.maxed_stat_set);

		ImGui::Checkbox("Maxed Armor Effect", &g_settings->player.maxed_armor_effect);
		
		ImGui::EndGroup();

		ImGui::BeginGroup();

		ImGui::Checkbox("Frostcraft Recharge", &g_settings->weapon.frostcraft_recharge);
		ImGui::InputFloat("Frostcraft Drawn", &g_settings->weapon.frostcraft_drawn, 1.f, 10.f);
		ImGui::InputFloat("Frostcraft Sheathed", &g_settings->weapon.frostcraft_heat, 1.f, 10.f);

		ImGui::EndGroup();

		if (ImGui::CollapsingHeader("Weapon Buffs"))
		{
			ImGui::BeginGroup();
			ImGui::Checkbox("Instantly Charged GS", &g_settings->weapon.inst_chrgs_gs);
			ImGui::EndGroup();
		}
	}
}