#include <gui.hpp>
#include "overlay.h"
#include <world/world_service.hpp>
#include <monster/monster_service.hpp>
#include <character/character_service.hpp>

namespace big
{
	void overlay::render_menu()
	{
		ImGui::SetNextWindowPos(ImVec2(10.0f, 10.0f), ImGuiCond_FirstUseEver, ImVec2(0.0f, 0.0f));
		ImGui::SetNextWindowBgAlpha(0.5f);

		auto window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
		
		if (!g_gui.m_opened) window_flags |= ImGuiWindowFlags_NoMouseInputs;

		if (ImGui::Begin("overlay", nullptr, window_flags))
		{
			ImGui::Text("Overlay Info");

			ImGui::SeparatorText("Character Info");

			ImGui::Text("Name : %s", g_character_service->get_name());
			ImGui::Text("Zeni : %d", g_character_service->get_zeni());
			ImGui::Text("HR EXP : %d", g_character_service->get_high_rank_experience());
			ImGui::Text("HR Level : %d", g_character_service->get_high_rank_level());
			ImGui::Text("MR EXP : %d", g_character_service->get_master_rank_experience());
			ImGui::Text("MR Level : %d", g_character_service->get_master_rank_level());
			ImGui::Text("Research Points : %d", g_character_service->get_research_points());
			ImGui::Text("Time Playing (ms) : %d", g_character_service->get_time_playing());

			ImGui::SeparatorText("World Info");

			ImGui::Text("Time of Days : %f", g_world_service->get_time_of_date());
			ImGui::Text("Weather : %s", g_world_service->get_weather());

			ImGui::SeparatorText("Monster Info");

			for (int i = 0; i < 3; i++)
			{
				if (g_monster_service->get_entry(i))
				{
					ImGui::SeparatorText(std::format("Monster {}", g_monster_service->get_monster_name(i)).c_str());
					ImGui::Text("Monster Health %.0f/%.0f", g_monster_service->get_health(i), g_monster_service->get_max_health(i));
					ImGui::Text("Monster Timer %f", g_monster_service->get_timer(i));
					ImGui::Text("Monster Max Timer %f", g_monster_service->get_max_timer(i));
					ImGui::Text("Monster Size %.0f", g_monster_service->get_size(i));
				}
			}
		}
		ImGui::End();
	}
}