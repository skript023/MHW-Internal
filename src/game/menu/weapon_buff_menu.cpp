#include "game/features/ui.hpp"
#include "game/features/features.hpp"
#include <imgui.h>
#include "weapon_buff_menu.h"

namespace big
{
	void weapon_buff_menu::render_menu()
	{
		ImGui::BeginGroup();
		features::checkbox("Instantly Charged GS", features::_inst_chrgs_gs);
		features::checkbox("Inf Spirit Gauge", features::_inf_spirit_gauge);
		features::checkbox("Inf Demon Mode", features::_inf_demon_mode);
		ImGui::EndGroup();

		ImGui::SameLine(200.f);
		ImGui::BeginGroup();
		features::checkbox("Fully Charged Switch Axe", features::_full_charged_switch_axe);
		features::checkbox("Inf GL Ammo/Wyrmstake", features::_inf_gl_ammo);
		features::checkbox("No GL Cooldown", features::_no_gl_cooldown);
		ImGui::EndGroup();
	}
}