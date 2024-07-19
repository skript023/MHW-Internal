#include "imgui.h"
#include "script.hpp"
#include "player_menu.h"
#include "settings.hpp"
#include "utility/mhw.hpp"

float g_atk_addr = 30.f;
float g_def_addr = 0.5f;
float g_affinity_addr = 150.f;

namespace big
{
	void player_menu::render_menu()
	{
		ImGui::BeginGroup();

		ImGui::Checkbox("Invulnerable", &g_settings->player.invulnerable);

		ImGui::Checkbox("Inf Stamina", &g_settings->player.inf_stamina);

		ImGui::Checkbox("Inf Mounting", &g_settings->player.inf_mount);

		ImGui::Checkbox("Inf Gathering", &g_settings->player.inf_gathering);

		ImGui::Checkbox("No Palico Gadget CD", &g_settings->player.palico_gadget_cd);

		ImGui::EndGroup();

		ImGui::SameLine(200.f);

		ImGui::BeginGroup();

		ImGui::Checkbox("Part Breaker", &g_settings->player.part_breaker);

		ImGui::Checkbox("Easy Part Breaker", &g_settings->player.easy_part_breaker);

		ImGui::Checkbox("Reset Mounting", &g_settings->player.mount_reset);

		ImGui::Checkbox("Inf Item Use", &g_settings->player.inf_item);

		ImGui::EndGroup();

		ImGui::SameLine(400.f);

		ImGui::BeginGroup();

		ImGui::Checkbox("Inf Lucky Voucher", &g_settings->player.inf_lucky_voucher);

		ImGui::Checkbox("Cannon Always Loaded", &g_settings->player.cannon_always_loaded);

		ImGui::Checkbox("Inf Cannon Fire", &g_settings->player.inf_cannon_fire);

		ImGui::Checkbox("Have All Pets", &g_settings->player.unlock_all_pet);

		ImGui::EndGroup();

		ImGui::Separator();

		if (ImGui::CollapsingHeader("Stats"))
		{
			ImGui::InputFloat("Critical Boost", &g_settings->player.critical_boost, 1.f, 1000.f);
			ImGui::InputFloat("Atk +", &g_atk_addr, 1.f, 1000.f);
			ImGui::InputFloat("dmg receive percent", &g_def_addr, 1.f, 1000.f);
			ImGui::InputInt("Def +", &*(int*)(&g_def_addr + 4), 1, 1000);
			ImGui::InputFloat("Affinity +", &g_affinity_addr, 1.f, 1000.f);
		}

		if (ImGui::CollapsingHeader("Player EXP"))
		{
			ImGui::InputFloat("EXP Multiplier", &g_settings->player.research_exp, 1.f, 10.f);
			ImGui::InputInt("MR EXP Mult", &g_settings->player.mr_ex_mult, 1, 1000);
			ImGui::InputInt("HR EXP Mult", &g_settings->player.hr_ex_mult, 1, 1000);
		}

		if (ImGui::CollapsingHeader("Monster"))
		{
			for (int i = 0; i < 3; i++)
			{
				ImGui::SeparatorText(std::format("Monster {}", i + 1).c_str());

				if (auto entry = mhw::monster_stats(i))
				{
					ImGui::Text("Monster Size %.0f", entry->m_size);
					if (auto wt = entry->m_monster_world_stats)
					{
						ImGui::Text("Monster ID %i", wt->m_monster_id);
						ImGui::Text("Monster Timer %f", wt->m_timer);
						ImGui::Text("Monster Max Timer %f", wt->m_max_timer);
					}
					if (auto st = entry->m_monster_stats)
					{
						ImGui::Text("Monster Health %.0f", st->m_health);
						ImGui::Text("Monster Max %.0f", st->m_max);
					}
				}
			}
		}

		ImGui::EndTabItem();
	}
    
}