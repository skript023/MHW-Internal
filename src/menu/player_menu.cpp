#include "imgui.h"
#include "script.hpp"
#include "player_menu.h"
#include "settings.hpp"
#include "utility/mhw.hpp"

namespace big
{
	void player_menu::render_menu()
	{
		ImGui::BeginGroup();

		ImGui::Checkbox("Invulnerable", &g_settings->player.invulnerable);

		ImGui::Checkbox("Inf Stamina", &g_settings->player.inf_stamina);

		ImGui::Checkbox("Inf Mounting", &g_settings->player.inf_mount);

		ImGui::Checkbox("Inf Gathering", &g_settings->player.inf_gathering);

		ImGui::EndGroup();

		ImGui::SameLine(200.f);

		ImGui::BeginGroup();

		ImGui::Checkbox("Part Breaker", &g_settings->player.part_breaker);

		ImGui::Checkbox("Easy Part Breaker", &g_settings->player.easy_part_breaker);

		ImGui::Checkbox("Reset Mounting", &g_settings->player.mount_reset);

		ImGui::EndGroup();

		ImGui::Separator();

		ImGui::BeginGroup();

		ImGui::InputFloat("Critical Boost", &g_settings->player.critical_boost, 1.f, 1000.f);
		ImGui::InputInt("MR EXP Mult", &g_settings->player.mr_ex_mult, 1, 1000);
		ImGui::InputInt("HR EXP Mult", &g_settings->player.hr_ex_mult, 1, 1000);

		ImGui::EndGroup();

		ImGui::EndTabItem();
	}
    
}