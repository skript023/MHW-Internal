#include "imgui.h"
#include "script.hpp"
#include "player_menu.h"
#include "settings.hpp"
#include "utility/mhw.hpp"
#include <services/character/character_service.hpp>
#include <services/meal/meal_service.hpp>

float g_atk_addr = 30.f;
int g_def_addr = 1;
float g_def_rate_addr = 0.5f;
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

		ImGui::Checkbox("Reveal Monster", &g_settings->player.reveal_monster);

		ImGui::EndGroup();

		ImGui::SameLine(400.f);

		ImGui::BeginGroup();

		ImGui::Checkbox("Inf Lucky Voucher", &g_settings->player.inf_lucky_voucher);

		ImGui::Checkbox("Cannon Always Loaded", &g_settings->player.cannon_always_loaded);

		ImGui::Checkbox("Inf Cannon Fire", &g_settings->player.inf_cannon_fire);

		ImGui::Checkbox("Have All Pets", &g_settings->player.unlock_all_pet);

		ImGui::EndGroup();

		ImGui::BeginGroup();

		if (ImGui::Button("Teleport Waypoint"))
		{
			g_character_service->set_player_to_waypoint();
		}

		ImGui::SameLine(200.f);

		if (ImGui::Button("Teleport Forward"))
		{
			g_character_service->set_player_forward();
		}

		ImGui::EndGroup();

		ImGui::Separator();

		if (ImGui::CollapsingHeader("Character Info"))
		{
			ImGui::Text("Zeni : %d", g_character_service->get_zeni());
			ImGui::Text("HR EXP : %d", g_character_service->get_high_rank_experience());
			ImGui::Text("HR Level : %d", g_character_service->get_high_rank_level());
			ImGui::Text("MR EXP : %d", g_character_service->get_master_rank_experience());
			ImGui::Text("MR Level : %d", g_character_service->get_master_rank_level());
			ImGui::Text("Research Points : %d", g_character_service->get_research_points());
			ImGui::Text("Time Playing (ms) : %d", g_character_service->get_time_playing());
		}

		if (ImGui::CollapsingHeader("Stats"))
		{
			ImGui::InputFloat("Critical Boost", &g_settings->player.critical_boost, 1.f, 1000.f);
			ImGui::InputFloat("Atk +", &g_atk_addr, 1.f, 1000.f);
			ImGui::InputFloat("dmg receive percent", &g_def_rate_addr, 1.f, 1000.f);
			ImGui::InputInt("Def +", &g_def_addr, 1, 1000);
			ImGui::InputFloat("Affinity +", &g_affinity_addr, 1.f, 1000.f);
		}

		if (ImGui::CollapsingHeader("Player EXP"))
		{
			ImGui::InputFloat("EXP Multiplier", &g_settings->player.research_exp, 1.f, 10.f);
			ImGui::InputInt("MR EXP Mult", &g_settings->player.mr_ex_mult, 1, 1000);
			ImGui::InputInt("HR EXP Mult", &g_settings->player.hr_ex_mult, 1, 1000);
			ImGui::InputInt("Palico EXP Multiplier", &g_settings->player.palico_exp_mult, 1, 1000);
		}

		if (ImGui::CollapsingHeader("Meal Editor"))
		{
			if (auto meal = g_meal_service->instance())
			{
				ImGui::Text("Meal Base: %X", meal);

				ImGui::Text("Meal Buff 1");
				if (ImGui::BeginCombo("##buff", g_meal_service->get_buff(meal->m_skill_a).c_str()))
				{
					for (auto& buff : meal_data::meal_buff)
					{
						if (ImGui::Selectable(buff.second.c_str(), meal->m_skill_a == buff.first))
						{
							meal->m_skill_a = buff.first;
						}
						if (meal->m_skill_a == buff.first)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}
				
				ImGui::Text("Meal Buff 2");
				if (ImGui::BeginCombo("##buff2", g_meal_service->get_buff(meal->m_skill_b).c_str()))
				{
					for (auto& buff : meal_data::meal_buff)
					{
						if (ImGui::Selectable(buff.second.c_str(), meal->m_skill_b == buff.first))
						{
							meal->m_skill_b = buff.first;
						}
						if (meal->m_skill_b == buff.first)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				ImGui::Text("Meal Buff 3");
				if (ImGui::BeginCombo("##buff3", g_meal_service->get_buff(meal->m_skill_c).c_str()))
				{
					for (auto& buff : meal_data::meal_buff)
					{
						if (ImGui::Selectable(buff.second.c_str(), meal->m_skill_c == buff.first))
						{
							meal->m_skill_c = buff.first;
						}
						if (meal->m_skill_c == buff.first)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				ImGui::SeparatorText("Stat Buff");

				ImGui::SliderInt("Attack Up", &meal->m_attack_effect, 1, 1000);
				ImGui::SliderInt("Defense Up", &meal->m_defense_effect, 1, 1000);
				ImGui::SliderInt("Health Up", &meal->m_health_effect, 1, 1000);
				ImGui::SliderInt("Stamina Up", &meal->m_stamina_effect, 1, 1000);
				ImGui::SliderInt("Palico Health Up", &meal->m_palico_health, 1, 1000);

				ImGui::SeparatorText("Misc");
				if (ImGui::TreeNode("Ingredient Reference"))
				{
					ImGui::SliderInt("Fresh Ingredient Activation Chance", &meal->m_fresh_ingredients, 1, 1000);

					for (int i = 0; i < IM_ARRAYSIZE(meal->m_ingredient_slots); i++)
					{
						ImGui::SliderInt(std::format("Ingredient Slot {}", i).c_str(), &meal->m_ingredient_slots[i], 1, 1000);
					}

					ImGui::TreePop();
				}
			}
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
	}
    
}