#include "weapon_buff_menu.h"

namespace big
{
	void weapon_buff_menu::render_menu()
	{
		ImGui::BeginGroup();
		ImGui::Checkbox("Instantly Charged GS", &g_settings->weapon.inst_chrgs_gs);
		ImGui::Checkbox("Inf Spirit Gauge", &g_settings->weapon.inf_spirit_gauge);
		ImGui::Checkbox("Inf Demon Mode", &g_settings->weapon.inf_demon_mode);
		ImGui::EndGroup();

		ImGui::SameLine(200.f);
		ImGui::BeginGroup();
		ImGui::Checkbox("Fully Charged Switch Axe", &g_settings->weapon.full_charged_switch_axe);
		ImGui::Checkbox("Inf GL Ammo/Wyrmstake", &g_settings->weapon.inf_gl_ammo);
		ImGui::Checkbox("No GL Cooldown", &g_settings->weapon.no_gl_cooldown);
		ImGui::EndGroup();
	}
}