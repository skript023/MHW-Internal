#include "overlay.h"

#include <character/character_service.hpp>
#include <monster/monster_service.hpp>
#include <world/world_service.hpp>
#include <astra/host/canvas.hpp>
#include <menu_settings.hpp>
#include <imgui.h>
#include <vector>
#include <format>
#include <algorithm>

namespace big
{
	void overlay::render_menu()
	{
		if (!g_settings.window.overlay)
			return;

		std::vector<astra::overlay_row> hunter_rows;
		if (g_character_service)
		{
			hunter_rows.push_back({"Name", g_character_service->get_name()});
			hunter_rows.push_back({"Zeni", std::to_string(g_character_service->get_zeni())});
			hunter_rows.push_back({"HR", std::format("Lvl {} ({} EXP)", g_character_service->get_high_rank_level(), g_character_service->get_high_rank_experience())});
			hunter_rows.push_back({"MR", std::format("Lvl {} ({} EXP)", g_character_service->get_master_rank_level(), g_character_service->get_master_rank_experience())});
			hunter_rows.push_back({"Research", std::to_string(g_character_service->get_research_points())});
		}
		if (g_world_service)
		{
			hunter_rows.push_back({"Weather", g_world_service->get_weather()});
			hunter_rows.push_back({"Time", std::format("{:.2f}", g_world_service->get_time_of_date())});
		}
		if (!hunter_rows.empty())
		{
			canvas::draw_overlay("Hunter Info", hunter_rows);
		}

		if (g_monster_service)
		{
			struct monster_display_info
			{
				int index;
				std::string name;
				float health;
				float max_health;
				float size;
				float timer;
				Vector3 position;
				float distance;
			};

			std::vector<monster_display_info> active_monsters;
			Vector3 player_pos = g_character_service ? g_character_service->get_position() : Vector3();

			for (int i = 0; i < 3; i++)
			{
				if (g_monster_service->get_entry(i))
				{
					float max_hp = g_monster_service->get_max_health(i);
					if (max_hp > 0.0f)
					{
						std::string name = g_monster_service->get_monster_name(i);
						if (name.empty() || name.find("Monster Not Found") != std::string::npos)
							name = std::format("Monster {}", i + 1);

						float hp = g_monster_service->get_health(i);
						float sz = g_monster_service->get_size(i);
						float tm = g_monster_service->get_timer(i);
						Vector3 pos = g_monster_service->get_position(i);
						float dist = player_pos.distance(pos);

						active_monsters.push_back({i, std::move(name), hp, max_hp, sz, tm, pos, dist});
					}
				}
			}

			bool is_menu_open = canvas::is_opened();
			if (active_monsters.empty() && !is_menu_open)
				return;

			// Position default: Top-Center of screen
			const auto* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(ImVec2(viewport->WorkPos.x + viewport->WorkSize.x * 0.5f, viewport->WorkPos.y + 24.0f), ImGuiCond_FirstUseEver, ImVec2(0.5f, 0.0f));
			ImGui::SetNextWindowSizeConstraints(ImVec2(380.0f, 0.0f), ImVec2(560.0f, FLT_MAX));

			ImGuiWindowFlags flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse;
			if (!is_menu_open)
			{
				flags |= ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove;
			}

			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, is_menu_open ? 1.0f : 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(12.0f, 10.0f));
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 6.0f));
			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.06f, 0.07f, 0.09f, is_menu_open ? 0.90f : 0.78f));
			ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.35f, 0.40f, 0.50f, 0.50f));

			if (ImGui::Begin("Monster Health Overlay###MonsterHealthOverlay", nullptr, flags))
			{
				if (active_monsters.empty())
				{
					ImGui::TextColored(ImVec4(0.6f, 0.8f, 1.0f, 1.0f), "[Preview] Monster Health Overlay");
					ImGui::Separator();
					ImGui::TextUnformatted("Target: Rathalos (Preview)");
					std::string dist_str = "Dist: 35.0 m";
					float dist_width = ImGui::CalcTextSize(dist_str.c_str()).x;
					ImGui::SameLine(ImGui::GetWindowWidth() - dist_width - 16.0f);
					ImGui::TextColored(ImVec4(0.4f, 0.85f, 1.0f, 1.0f), "%s", dist_str.c_str());

					ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.20f, 0.82f, 0.35f, 1.0f));
					ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.15f, 0.16f, 0.18f, 0.9f));
					ImGui::ProgressBar(1.0f, ImVec2(-1.0f, 18.0f), "15000 / 15000 (100.0%)");
					ImGui::PopStyleColor(2);

					ImGui::TextColored(ImVec4(0.68f, 0.72f, 0.78f, 1.0f), "Size: 1750.0  |  Pos: [120.5, 45.2, -80.0]");
				}
				else
				{
					for (size_t idx = 0; idx < active_monsters.size(); idx++)
					{
						const auto& m = active_monsters[idx];
						if (idx > 0)
						{
							ImGui::Spacing();
							ImGui::Separator();
							ImGui::Spacing();
						}

						// Title line: Name & Distance
						ImGui::TextColored(ImVec4(1.0f, 0.9f, 0.5f, 1.0f), "%s", m.name.c_str());

						std::string dist_str = std::format("{:.1f} m", m.distance);
						float dist_width = ImGui::CalcTextSize(dist_str.c_str()).x;
						ImGui::SameLine(ImGui::GetWindowWidth() - dist_width - 16.0f);
						ImGui::TextColored(ImVec4(0.4f, 0.85f, 1.0f, 1.0f), "%s", dist_str.c_str());

						// Health bar
						float fraction = m.max_health > 0.0f ? std::clamp(m.health / m.max_health, 0.0f, 1.0f) : 0.0f;
						std::string hp_text = std::format("{:.0f} / {:.0f} ({:.1f}%)", m.health, m.max_health, fraction * 100.0f);

						ImVec4 bar_color;
						if (m.health <= 0.0f)
						{
							bar_color = ImVec4(0.5f, 0.5f, 0.5f, 0.8f);
							hp_text = "DEAD / CAPTURED";
						}
						else if (fraction > 0.50f)
						{
							bar_color = ImVec4(0.20f, 0.82f, 0.35f, 1.0f); // Green
						}
						else if (fraction > 0.25f)
						{
							bar_color = ImVec4(0.95f, 0.75f, 0.15f, 1.0f); // Amber / Yellow
						}
						else
						{
							bar_color = ImVec4(0.92f, 0.22f, 0.22f, 1.0f); // Red
						}

						ImGui::PushStyleColor(ImGuiCol_PlotHistogram, bar_color);
						ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.15f, 0.16f, 0.18f, 0.9f));
						ImGui::ProgressBar(fraction, ImVec2(-1.0f, 18.0f), hp_text.c_str());
						ImGui::PopStyleColor(2);

						// Meta line: Size, Timer, Coordinates
						std::string meta_line;
						if (m.size > 0.0f)
							meta_line += std::format("Size: {:.1f}  ", m.size);
						if (m.timer > 0.0f)
							meta_line += std::format("|  Timer: {:.1f}s  ", m.timer);
						meta_line += std::format("|  Pos: [{:.1f}, {:.1f}, {:.1f}]", m.position.x, m.position.y, m.position.z);

						ImGui::TextColored(ImVec4(0.68f, 0.72f, 0.78f, 1.0f), "%s", meta_line.c_str());
					}
				}
			}
			ImGui::End();
			ImGui::PopStyleColor(2);
			ImGui::PopStyleVar(4);
		}
	}
}
