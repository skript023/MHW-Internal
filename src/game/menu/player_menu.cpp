#include "game/features/ui.hpp"
#include "game/features/features.hpp"
#include "player_menu.h"

#include "imgui.h"
#include "core/threading/script.hpp"
#include "game/mhw/mhw.hpp"

float g_atk_addr = 30.f;
int g_def_addr = 5000;
float g_def_rate_addr = 0.5f;
float g_affinity_addr = 150.f;

namespace big
{
	void player_menu::render_menu()
	{
		ImGui::BeginGroup();

		features::checkbox("Invulnerable", features::_invulnerable);

		features::checkbox("Inf Stamina", features::_inf_stamina);

		features::checkbox("Inf Mounting", features::_inf_mount);

		features::checkbox("Inf Gathering", features::_inf_gathering);

		features::checkbox("No Palico Gadget CD", features::_palico_gadget_cd);

		ImGui::EndGroup();

		ImGui::SameLine(200.f);

		ImGui::BeginGroup();

		features::checkbox("Part Breaker", features::_part_breaker);

		features::checkbox("Easy Part Breaker", features::_easy_part_breaker);

		features::checkbox("Reset Mounting", features::_mount_reset);

		features::checkbox("Inf Item Use", features::_inf_item);

		features::checkbox("Reveal Monster", features::_reveal_monster);

		ImGui::EndGroup();

		ImGui::SameLine(400.f);

		ImGui::BeginGroup();

		features::checkbox("Inf Lucky Voucher", features::_inf_lucky_voucher);

		features::checkbox("Cannon Always Loaded", features::_cannon_always_loaded);

		features::checkbox("Inf Cannon Fire", features::_inf_cannon_fire);

		features::checkbox("Have All Pets", features::_unlock_all_pet);

		ImGui::EndGroup();

		ImGui::Separator();

		if (ImGui::CollapsingHeader("Stats"))
		{
			features::input_float("Critical Boost", features::_critical_boost, 1.f, 1000.f);
			ImGui::InputFloat("Atk +", &g_atk_addr, 1.f, 1000.f);
			ImGui::InputFloat("dmg receive percent", &g_def_rate_addr, 1.f, 1000.f);
			ImGui::InputInt("Def +", &g_def_addr, 1, 1000);
			ImGui::InputFloat("Affinity +", &g_affinity_addr, 1.f, 1000.f);
		}

		if (ImGui::CollapsingHeader("Player EXP"))
		{
			features::input_float("EXP Multiplier", features::_research_exp, 1.f, 10.f);
			features::input_int("MR EXP Mult", features::_mr_ex_mult, 1, 1000);
			features::input_int("HR EXP Mult", features::_hr_ex_mult, 1, 1000);
			features::input_int("Palico EXP Multiplier", features::_palico_exp_mult, 1, 1000);
		}
	}

}