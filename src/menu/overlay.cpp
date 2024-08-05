#include "overlay.h"
#include "monster/monster_service.hpp"
#include <gui.hpp>

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

			ImGui::Separator();
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