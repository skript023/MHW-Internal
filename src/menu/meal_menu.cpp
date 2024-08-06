#include "meal_menu.h"
#include <meal/meal_service.hpp>

namespace big
{
	void meal_menu::render_menu()
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
}