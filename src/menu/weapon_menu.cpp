#include "weapon_menu.h"
#include "features/weapon.hpp"
#include <services/equipment/armour_service.hpp>
#include <services/equipment/weapon_service.hpp>

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

		if (ImGui::CollapsingHeader("Weapon Buffs"))
		{
			ImGui::BeginGroup();
			ImGui::Checkbox("Instantly Charged GS", &g_settings->weapon.inst_chrgs_gs);
			ImGui::EndGroup();
		}

		if (ImGui::CollapsingHeader("Equipment"))
		{
			auto equipment = g_armour_service->get();
			auto melee = g_weapon_service->get_melee();
			auto ranged = g_weapon_service->get_ranged();
			
			/*ImGui::Text("Equipment Base %X", equipment);
			ImGui::Text("Melee Weapon Base %X", melee);
			ImGui::Text("Ranged Weapon Base %X", equipment);*/

			if (melee)
			{
				if (ImGui::TreeNode("Melee Weapon"))
				{
					ImGui::InputScalar("Weapon ID", ImGuiDataType_U16, &melee->m_weapon_id);
					ImGui::InputScalar("Attack", ImGuiDataType_U16, &melee->m_attack);
					ImGui::InputScalar("Defense", ImGuiDataType_U16, &melee->m_defense);
					ImGui::InputScalar("Affinity", ImGuiDataType_U8, &melee->m_affinity);

					ImGui::InputScalar("Rarity (-1)", ImGuiDataType_U8, &melee->m_rarity);
					ImGui::InputScalar("Sharpness Quality", ImGuiDataType_U8, &melee->m_sharpness_quality);
					ImGui::InputScalar("Sharpness Amount", ImGuiDataType_U8, &melee->m_sharpness_amount);

					ImGui::Text("Skill");
					if (ImGui::BeginCombo("##skill", g_weapon_service->get_melee_skill().c_str()))
					{
						for (auto& skill : g_weapon_service->get_skill_data())
						{
							if (ImGui::Selectable(skill.second.c_str(), melee->m_element_type == skill.first))
							{
								melee->m_skill = skill.first;
							}
							if (melee->m_skill == skill.first)
								ImGui::SetItemDefaultFocus();
						}

						ImGui::EndCombo();
					}

					if (ImGui::TreeNode("Elemental Effect"))
					{
						ImGui::Text("Elemental Type");
						if (ImGui::BeginCombo("##elementaltype", g_weapon_service->get_melee_elemental().c_str()))
						{
							for (auto& element : g_weapon_service->get_element_data())
							{
								if (ImGui::Selectable(element.second.c_str(), melee->m_element_type == element.first))
								{
									melee->m_element_type = element.first;
								}
								if (melee->m_element_type == element.first)
									ImGui::SetItemDefaultFocus();
							}

							ImGui::EndCombo();
						}
						ImGui::InputScalar("Elemental Attack", ImGuiDataType_U16, &melee->m_elemental_atk);

						ImGui::Text("Hidden Elemental Type");
						if (ImGui::BeginCombo("##hiddenelementaltype", g_weapon_service->get_melee_hidden_elemental().c_str()))
						{
							for (auto& element : g_weapon_service->get_element_data())
							{
								if (ImGui::Selectable(element.second.c_str(), melee->m_hidden_element_type == element.first))
								{
									melee->m_hidden_element_type = element.first;
								}
								if (melee->m_element_type == element.first)
									ImGui::SetItemDefaultFocus();
							}

							ImGui::EndCombo();
						}
						ImGui::InputScalar("Hidden Elemental Attack", ImGuiDataType_U16, &melee->m_hidden_element_atk);

						ImGui::TreePop();
					}

					ImGui::TreePop();
				}
			}

			if (equipment)
			{
				if (ImGui::TreeNode("Armour"))
				{
					ImGui::InputScalar("ID For Layered", ImGuiDataType_S16, &equipment->m_id_layered);
					ImGui::InputScalar("ID For ED", ImGuiDataType_S16, &equipment->m_id_ed);
					ImGui::InputScalar("Defense", ImGuiDataType_S16, &equipment->m_defense);

					if (ImGui::TreeNode("# Of Active Slots"))
					{
						ImGui::InputScalar("Slot 1 level", ImGuiDataType_S8, &equipment->m_slot_a_level);
						ImGui::InputScalar("Slot 2 level", ImGuiDataType_S8, &equipment->m_slot_b_level);
						ImGui::InputScalar("Slot 3 level", ImGuiDataType_S8, &equipment->m_slot_c_level);

						ImGui::TreePop();
					}

					ImGui::InputScalar("Fire Res", ImGuiDataType_S16, &equipment->m_fire_res);
					ImGui::InputScalar("Water Res", ImGuiDataType_S16, &equipment->m_water_res);
					ImGui::InputScalar("Thunder Res", ImGuiDataType_S16, &equipment->m_thunder_res);
					ImGui::InputScalar("Ice Res", ImGuiDataType_S16, &equipment->m_ice_res);
					ImGui::InputScalar("Dragon Res", ImGuiDataType_S16, &equipment->m_dragon_res);

					if (ImGui::TreeNode("Equipment Effect"))
					{
						ImGui::InputScalar("Set Bonus", ImGuiDataType_S8, &equipment->m_set_bonus);
						ImGui::InputScalar("Set Bonus Level", ImGuiDataType_S8, &equipment->m_bonus_level);

						ImGui::InputScalar("Skill 1", ImGuiDataType_S8, &equipment->m_skill_a);
						ImGui::InputScalar("Skill 1 Level", ImGuiDataType_S8, &equipment->m_slot_a_level);

						ImGui::InputScalar("Skill 2", ImGuiDataType_S8, &equipment->m_skill_b);
						ImGui::InputScalar("Skill 2 Level", ImGuiDataType_S8, &equipment->m_slot_b_level);

						ImGui::InputScalar("Skill 3", ImGuiDataType_S8, &equipment->m_skill_c);
						ImGui::InputScalar("Skill 3 Level", ImGuiDataType_S8, &equipment->m_slot_c_level);

						ImGui::TreePop();
					}
					ImGui::TreePop();
				}
			}
		}
	}
}