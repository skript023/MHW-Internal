#include "overlay.h"

#include <character/character_service.hpp>
#include <monster/monster_service.hpp>
#include <world/world_service.hpp>
#include <services/damage/damage_service.hpp>
#include <services/drop/drop_service.hpp>
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

		bool is_menu_open = canvas::is_opened();
		const auto* viewport = ImGui::GetMainViewport();

		ImGuiWindowFlags unfocused_flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse;
		if (!is_menu_open)
		{
			unfocused_flags |= ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove;
		}

		// ==========================================
		// 1. HUNTER INFO OVERLAY
		// ==========================================
		if (g_settings.window.overlay_hunter)
		{
			std::vector<astra::overlay_row> hunter_rows;
			if (true)
			{
				hunter_rows.push_back({"Name", character_service::get_name()});
				hunter_rows.push_back({"Zeni", std::to_string(character_service::get_zeni())});
				hunter_rows.push_back({"HR", std::format("Lvl {} ({} EXP)", character_service::get_high_rank_level(), character_service::get_high_rank_experience())});
				hunter_rows.push_back({"MR", std::format("Lvl {} ({} EXP)", character_service::get_master_rank_level(), character_service::get_master_rank_experience())});
				hunter_rows.push_back({"Research", std::to_string(character_service::get_research_points())});

				auto pos = character_service::get_position();
				hunter_rows.push_back({"Position", std::format("{:.1f}, {:.1f}, {:.1f}", pos.x, pos.y, pos.z)});

				auto wp = character_service::get_waypoint_pos();
				if (wp.x != 0.f || wp.y != 0.f || wp.z != 0.f)
				{
					hunter_rows.push_back({"Waypoint Dist", std::format("{:.1f} m", pos.distance(wp))});
				}
			}
			if (true)
			{
				hunter_rows.push_back({"Weather", world_service::get_weather()});
				float tod = world_service::get_time_of_date();
				std::string day_phase = (tod >= 6.f && tod < 18.f) ? "Day" : (tod >= 18.f && tod < 20.f) ? "Dusk" : "Night";
				hunter_rows.push_back({"Time", std::format("{:.2f} ({})", tod, day_phase)});
			}
			if (!hunter_rows.empty())
			{
				canvas::draw_overlay("Hunter Info", hunter_rows);
			}
		}

		// ==========================================
		// 2. MONSTER HEALTH, PARTS & AILMENTS OVERLAY
		// ==========================================
		if (g_settings.window.overlay_monster)
		{
			struct monster_display_info
			{
				int index;
				std::string name;
				float health;
				float max_health;
				float size;
				float timer;
				float max_timer;
				Vector3 position;
				float distance;
				std::vector<monster_part_info> parts;
				std::vector<monster_ailment_info> ailments;
			};

			std::vector<monster_display_info> active_monsters;
			Vector3 player_pos = character_service::get_position();

			for (int i = 0; i < 3; i++)
			{
				if (monster_service::get_entry(i))
				{
					float max_hp = monster_service::get_max_health(i);
					if (max_hp > 0.0f)
					{
						std::string name = monster_service::get_monster_name(i);
						if (name.empty() || name.find("Monster Not Found") != std::string::npos)
							name = std::format("Monster {}", i + 1);

						float hp = monster_service::get_health(i);
						float sz = monster_service::get_size(i);
						float tm = monster_service::get_timer(i);
						float max_tm = monster_service::get_max_timer(i);
						Vector3 pos = monster_service::get_position(i);
						float dist = player_pos.distance(pos);
						auto parts = monster_service::get_parts(i);
						auto ailments = monster_service::get_ailments(i);

						active_monsters.push_back({i, std::move(name), hp, max_hp, sz, tm, max_tm, pos, dist, std::move(parts), std::move(ailments)});
					}
				}
			}

			if (!active_monsters.empty() || is_menu_open)
			{
				// Position: Top-Center
				ImGui::SetNextWindowPos(ImVec2(viewport->WorkPos.x + viewport->WorkSize.x * 0.5f, viewport->WorkPos.y + 24.0f), ImGuiCond_FirstUseEver, ImVec2(0.5f, 0.0f));
				ImGui::SetNextWindowSizeConstraints(ImVec2(440.0f, 0.0f), ImVec2(620.0f, FLT_MAX));

				ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.0f);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, is_menu_open ? 1.0f : 0.0f);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(12.0f, 10.0f));
				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 5.0f));
				ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.06f, 0.07f, 0.09f, is_menu_open ? 0.90f : 0.82f));
				ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.35f, 0.40f, 0.50f, 0.50f));

				if (ImGui::Begin("Monster Health Overlay###MonsterHealthOverlay", nullptr, unfocused_flags))
				{
					if (active_monsters.empty())
					{
						// Preview Card for positioning and visual adjustment
						ImGui::TextColored(ImVec4(0.6f, 0.8f, 1.0f, 1.0f), "[Preview] Monster Health & Combat Widget");
						ImGui::Separator();

						ImGui::TextColored(ImVec4(1.0f, 0.90f, 0.50f, 1.0f), "Rathalos");
						if (g_settings.window.overlay_crown)
						{
							ImGui::SameLine();
							ImGui::TextColored(ImVec4(1.0f, 0.84f, 0.0f, 1.0f), "[Giant Gold 👑] (124.5%)");
						}

						std::string dist_str = "28.5 m [MID]";
						float dist_width = ImGui::CalcTextSize(dist_str.c_str()).x;
						ImGui::SameLine(ImGui::GetWindowWidth() - dist_width - 16.0f);
						ImGui::TextColored(ImVec4(1.0f, 0.85f, 0.35f, 1.0f), "%s", dist_str.c_str());

						// Main HP Bar
						ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.20f, 0.82f, 0.35f, 1.0f));
						ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.15f, 0.16f, 0.18f, 0.9f));
						ImGui::ProgressBar(0.85f, ImVec2(-1.0f, 18.0f), "12,750 / 15,000 (85.0%)");
						ImGui::PopStyleColor(2);

						// Enrage Bar
						if (g_settings.window.overlay_enrage_bar)
						{
							ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(1.0f, 0.45f, 0.12f, 1.0f));
							ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.25f, 0.10f, 0.08f, 0.85f));
							ImGui::ProgressBar(0.72f, ImVec2(-1.0f, 12.0f), "Enrage / Agitator: 65.0s / 90.0s (72%)");
							ImGui::PopStyleColor(2);
						}

						// Severable Tail & Body Parts Preview
						if (g_settings.window.overlay_monster_parts)
						{
							ImGui::Spacing();
							ImGui::TextColored(ImVec4(0.8f, 0.85f, 0.95f, 1.0f), "Body Parts & Severable Tail:");

							// Tail Bar Highlight
							ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.35f, 0.75f, 1.00f, 1.0f));
							ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.12f, 0.18f, 0.25f, 0.85f));
							ImGui::ProgressBar(0.62f, ImVec2(-1.0f, 13.0f), "Tail [Severable Cut]: 930 / 1,500 HP (62.0%) ✂️");
							ImGui::PopStyleColor(2);

							// Sample Sub-Parts
							ImGui::Columns(2, "PreviewPartsColumns", false);
							ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.30f, 0.70f, 0.50f, 0.9f));
							ImGui::ProgressBar(0.90f, ImVec2(-1.0f, 10.0f), "Head: 1,080 / 1,200");
							ImGui::ProgressBar(0.75f, ImVec2(-1.0f, 10.0f), "Left Wing: 675 / 900");
							ImGui::NextColumn();
							ImGui::ProgressBar(0.85f, ImVec2(-1.0f, 10.0f), "Body: 2,550 / 3,000");
							ImGui::ProgressBar(0.80f, ImVec2(-1.0f, 10.0f), "Right Wing: 720 / 900");
							ImGui::PopStyleColor();
							ImGui::Columns(1);
						}

						// Status Buildup (Ailments) Preview
						if (g_settings.window.overlay_monster_ailments)
						{
							ImGui::Spacing();
							ImGui::TextColored(ImVec4(0.8f, 0.85f, 0.95f, 1.0f), "Status Ailments Buildup:");
							ImGui::Columns(3, "PreviewAilmentColumns", false);

							ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.73f, 0.33f, 0.83f, 1.0f));
							ImGui::ProgressBar(0.75f, ImVec2(-1.0f, 10.0f), "Poison: 112/150");
							ImGui::PopStyleColor();

							ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(1.00f, 0.84f, 0.00f, 1.0f));
							ImGui::ProgressBar(0.40f, ImVec2(-1.0f, 10.0f), "Para: 72/180");
							ImGui::PopStyleColor();

							ImGui::NextColumn();
							ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.53f, 0.81f, 0.92f, 1.0f));
							ImGui::ProgressBar(0.30f, ImVec2(-1.0f, 10.0f), "Sleep: 39/130");
							ImGui::PopStyleColor();

							ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(1.00f, 0.27f, 0.00f, 1.0f));
							ImGui::ProgressBar(0.85f, ImVec2(-1.0f, 10.0f), "Blast: 119/140");
							ImGui::PopStyleColor();

							ImGui::NextColumn();
							ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(1.00f, 0.65f, 0.00f, 1.0f));
							ImGui::ProgressBar(0.55f, ImVec2(-1.0f, 10.0f), "Stun: 88/160");
							ImGui::PopStyleColor();

							ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.13f, 0.70f, 0.67f, 1.0f));
							ImGui::ProgressBar(0.45f, ImVec2(-1.0f, 10.0f), "Exhaust: 90/200");
							ImGui::PopStyleColor();

							ImGui::Columns(1);
						}

						ImGui::Spacing();
						ImGui::TextColored(ImVec4(0.68f, 0.72f, 0.78f, 1.0f), "Pos: [120.5, 45.2, -80.0]");
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

							// Title: Monster Name + Crown Badge
							ImGui::TextColored(ImVec4(1.0f, 0.90f, 0.50f, 1.0f), "%s", m.name.c_str());

							if (g_settings.window.overlay_crown && m.size > 0.01f)
							{
								ImGui::SameLine();
								if (m.size <= 0.90f)
								{
									ImGui::TextColored(ImVec4(1.0f, 0.84f, 0.0f, 1.0f), "[Mini Crown 👑] (%.1f%%)", m.size * 100.0f);
								}
								else if (m.size >= 1.23f)
								{
									ImGui::TextColored(ImVec4(1.0f, 0.84f, 0.0f, 1.0f), "[Giant Gold 👑] (%.1f%%)", m.size * 100.0f);
								}
								else if (m.size >= 1.15f)
								{
									ImGui::TextColored(ImVec4(0.85f, 0.88f, 0.95f, 1.0f), "[Silver Crown 🥈] (%.1f%%)", m.size * 100.0f);
								}
								else
								{
									ImGui::TextColored(ImVec4(0.65f, 0.68f, 0.72f, 1.0f), "(%.1f%%)", m.size * 100.0f);
								}
							}

							// Distance badge
							std::string dist_str;
							ImVec4 dist_color;
							if (m.distance < 25.0f)
							{
								dist_str = std::format("{:.1f} m [NEAR]", m.distance);
								dist_color = ImVec4(1.00f, 0.35f, 0.35f, 1.0f);
							}
							else if (m.distance <= 60.0f)
							{
								dist_str = std::format("{:.1f} m [MID]", m.distance);
								dist_color = ImVec4(1.00f, 0.85f, 0.35f, 1.0f);
							}
							else
							{
								dist_str = std::format("{:.1f} m", m.distance);
								dist_color = ImVec4(0.40f, 0.85f, 1.0f, 1.0f);
							}

							float dist_width = ImGui::CalcTextSize(dist_str.c_str()).x;
							ImGui::SameLine(ImGui::GetWindowWidth() - dist_width - 16.0f);
							ImGui::TextColored(dist_color, "%s", dist_str.c_str());

							// Main HP Bar
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
								bar_color = ImVec4(0.20f, 0.82f, 0.35f, 1.0f);
							}
							else if (fraction > 0.25f)
							{
								bar_color = ImVec4(0.95f, 0.75f, 0.15f, 1.0f);
							}
							else
							{
								bar_color = ImVec4(0.92f, 0.22f, 0.22f, 1.0f);
							}

							ImGui::PushStyleColor(ImGuiCol_PlotHistogram, bar_color);
							ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.15f, 0.16f, 0.18f, 0.9f));
							ImGui::ProgressBar(fraction, ImVec2(-1.0f, 18.0f), hp_text.c_str());
							ImGui::PopStyleColor(2);

							// Enrage Bar
							if (g_settings.window.overlay_enrage_bar && m.timer > 0.0f)
							{
								float timer_frac = (m.max_timer > 0.0f) ? std::clamp(m.timer / m.max_timer, 0.0f, 1.0f) : 1.0f;
								std::string enrage_label = (m.max_timer > 0.0f)
									? std::format("Enrage / Agitator: {:.1f}s / {:.1f}s ({:.0f}%)", m.timer, m.max_timer, timer_frac * 100.0f)
									: std::format("Enrage / Agitator: {:.1f}s", m.timer);

								ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(1.0f, 0.45f, 0.12f, 1.0f));
								ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.25f, 0.10f, 0.08f, 0.85f));
								ImGui::ProgressBar(timer_frac, ImVec2(-1.0f, 12.0f), enrage_label.c_str());
								ImGui::PopStyleColor(2);
							}

							// Sub-fitur 1: Monster Part Health & Severable Tail Bar
							if (g_settings.window.overlay_monster_parts && !m.parts.empty())
							{
								ImGui::Spacing();
								ImGui::TextColored(ImVec4(0.78f, 0.82f, 0.90f, 1.0f), "Body Parts & Severable Tail:");

								// Severable Tail check
								const monster_part_info* tail_part = nullptr;
								for (const auto& p : m.parts)
								{
									if (p.is_severable || p.name.find("Tail") != std::string::npos)
									{
										tail_part = &p;
										break;
									}
								}

								if (tail_part)
								{
									if (tail_part->is_severed)
									{
										ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.3f, 0.8f, 0.3f, 0.9f));
										ImGui::ProgressBar(1.0f, ImVec2(-1.0f, 13.0f), "Tail [SEVERED ✂️]");
										ImGui::PopStyleColor();
									}
									else
									{
										float tail_frac = tail_part->max_health > 0.0f ? std::clamp(tail_part->health / tail_part->max_health, 0.0f, 1.0f) : 0.0f;
										std::string tail_str = std::format("Tail Cut Remaining: {:.0f} / {:.0f} ({:.1f}%) ✂️", tail_part->health, tail_part->max_health, tail_frac * 100.0f);

										ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.25f, 0.65f, 0.95f, 1.0f));
										ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.12f, 0.16f, 0.22f, 0.85f));
										ImGui::ProgressBar(tail_frac, ImVec2(-1.0f, 13.0f), tail_str.c_str());
										ImGui::PopStyleColor(2);
									}
								}

								// Other parts (grid layout)
								ImGui::Columns(2, "PartsColumns", false);
								for (size_t p_idx = 0; p_idx < m.parts.size(); p_idx++)
								{
									const auto& p = m.parts[p_idx];
									if (&p == tail_part)
										continue;

									float p_frac = p.max_health > 0.0f ? std::clamp(p.health / p.max_health, 0.0f, 1.0f) : 0.0f;
									std::string p_str = std::format("{}: {:.0f}/{:.0f}", p.name, p.health, p.max_health);

									ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.25f, 0.70f, 0.50f, 0.85f));
									ImGui::ProgressBar(p_frac, ImVec2(-1.0f, 10.0f), p_str.c_str());
									ImGui::PopStyleColor();

									if (p_idx % 2 == 1)
										ImGui::NextColumn();
								}
								ImGui::Columns(1);
							}

							// Sub-fitur 2: Monster Status Buildup (Ailments Tracking)
							if (g_settings.window.overlay_monster_ailments && !m.ailments.empty())
							{
								ImGui::Spacing();
								ImGui::TextColored(ImVec4(0.78f, 0.82f, 0.90f, 1.0f), "Status Ailments Buildup:");
								ImGui::Columns(3, "AilmentColumns", false);

								for (size_t a_idx = 0; a_idx < m.ailments.size(); a_idx++)
								{
									const auto& a = m.ailments[a_idx];
									float a_frac = a.max_buildup > 0.0f ? std::clamp(a.buildup / a.max_buildup, 0.0f, 1.0f) : 0.0f;
									std::string a_str = a.is_active
										? std::format("{}: [ACTIVE] {:.0f}s", a.name, a.duration)
										: std::format("{}: {:.0f}/{:.0f}", a.name, a.buildup, a.max_buildup);

									ImVec4 color = ImGui::ColorConvertU32ToFloat4(a.color);
									ImGui::PushStyleColor(ImGuiCol_PlotHistogram, color);
									ImGui::ProgressBar(a.is_active ? 1.0f : a_frac, ImVec2(-1.0f, 10.0f), a_str.c_str());
									ImGui::PopStyleColor();

									if (a_idx % 3 == 2)
										ImGui::NextColumn();
								}
								ImGui::Columns(1);
							}

							// Meta line
							std::string meta_line = std::format("Pos: [{:.1f}, {:.1f}, {:.1f}]", m.position.x, m.position.y, m.position.z);
							ImGui::TextColored(ImVec4(0.68f, 0.72f, 0.78f, 1.0f), "%s", meta_line.c_str());

							if (g_settings.player.easy_part_breaker)
							{
								ImGui::SameLine();
								ImGui::TextColored(ImVec4(0.30f, 1.00f, 0.40f, 1.0f), "[1-Hit Part Breaker]");
							}
							else if (g_settings.player.part_breaker)
							{
								ImGui::SameLine();
								ImGui::TextColored(ImVec4(0.30f, 0.80f, 1.00f, 1.0f), "[Part Breaker]");
							}
						}
					}
				}
				ImGui::End();
				ImGui::PopStyleColor(2);
				ImGui::PopStyleVar(4);
			}
		}

		// ==========================================
		// 3. PARTY DPS / DAMAGE METER OVERLAY
		// ==========================================
		if (g_settings.window.overlay_party_dps)
		{
			auto party = damage_service::get_party_damage();
			uint32_t total_team_dmg = damage_service::get_total_party_damage();
			float hunt_time = damage_service::get_hunt_time();

			if (!party.empty() || is_menu_open)
			{
				// Position: Top-Right
				ImGui::SetNextWindowPos(ImVec2(viewport->WorkPos.x + viewport->WorkSize.x - 20.0f, viewport->WorkPos.y + 24.0f), ImGuiCond_FirstUseEver, ImVec2(1.0f, 0.0f));
				ImGui::SetNextWindowSizeConstraints(ImVec2(340.0f, 0.0f), ImVec2(480.0f, FLT_MAX));

				ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.0f);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, is_menu_open ? 1.0f : 0.0f);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(12.0f, 10.0f));
				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 5.0f));
				ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.06f, 0.07f, 0.09f, is_menu_open ? 0.90f : 0.80f));
				ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.35f, 0.40f, 0.50f, 0.50f));

				if (ImGui::Begin("Party Damage Meter###PartyDamageMeter", nullptr, unfocused_flags))
				{
					int minutes = static_cast<int>(hunt_time) / 60;
					int seconds = static_cast<int>(hunt_time) % 60;

					ImGui::TextColored(ImVec4(1.0f, 0.84f, 0.0f, 1.0f), "⚔️ Party Damage Meter");
					ImGui::SameLine();
					ImGui::TextColored(ImVec4(0.7f, 0.75f, 0.85f, 1.0f), "[⏱️ %02d:%02d]", minutes, seconds);

					if (is_menu_open)
					{
						ImGui::SameLine(ImGui::GetWindowWidth() - 75.0f);
						if (ImGui::SmallButton("Reset"))
						{
							damage_service::reset();
						}
					}

					ImGui::Separator();

					if (party.empty() || total_team_dmg == 0)
					{
						// Preview team rows
						struct preview_player { const char* rank; const char* name; const char* weapon; uint32_t dmg; float dps; float pct; int carts; };
						const preview_player preview_team[] = {
							{"🥇", "Hunter (You)", "Great Sword", 14500, 241.6f, 58.0f, 0},
							{"🥈", "Ace Lancer", "Lance", 6200, 103.3f, 24.8f, 0},
							{"🥉", "Bow Master", "Bow", 4300, 71.6f, 17.2f, 1}
						};

						for (const auto& p : preview_team)
						{
							ImGui::TextColored(ImVec4(0.9f, 0.9f, 0.95f, 1.0f), "%s %s (%s)", p.rank, p.name, p.weapon);
							ImGui::SameLine(ImGui::GetWindowWidth() - 95.0f);
							ImGui::TextColored(ImVec4(1.0f, 0.85f, 0.35f, 1.0f), "%u (%5.1f/s)", p.dmg, p.dps);

							float pct_frac = p.pct / 100.0f;
							std::string pct_label = std::format("{:.1f}% [Carts: {}]", p.pct, p.carts);

							ImGui::PushStyleColor(ImGuiCol_PlotHistogram, (p.pct > 50.0f) ? ImVec4(0.95f, 0.35f, 0.20f, 1.0f) : ImVec4(0.20f, 0.65f, 0.95f, 1.0f));
							ImGui::ProgressBar(pct_frac, ImVec2(-1.0f, 12.0f), pct_label.c_str());
							ImGui::PopStyleColor();
						}
					}
					else
					{
						for (size_t p_idx = 0; p_idx < party.size(); p_idx++)
						{
							const auto& p = party[p_idx];
							const char* rank_icon = (p_idx == 0) ? "🥇" : (p_idx == 1) ? "🥈" : (p_idx == 2) ? "🥉" : "4️⃣";

							ImGui::TextColored(p.is_local ? ImVec4(0.3f, 1.0f, 0.5f, 1.0f) : ImVec4(0.9f, 0.9f, 0.95f, 1.0f),
								"%s %s", rank_icon, p.name.c_str());

							std::string stat_str = std::format("{} ({:.0f}/s)", p.total_damage, p.dps);
							float stat_width = ImGui::CalcTextSize(stat_str.c_str()).x;
							ImGui::SameLine(ImGui::GetWindowWidth() - stat_width - 16.0f);
							ImGui::TextColored(ImVec4(1.0f, 0.85f, 0.35f, 1.0f), "%s", stat_str.c_str());

							float pct_frac = std::clamp(p.damage_percent / 100.0f, 0.0f, 1.0f);
							std::string pct_label = std::format("{:.1f}% [Carts: {}]", p.damage_percent, p.cart_count);

							ImVec4 bar_col = p.is_local ? ImVec4(0.20f, 0.80f, 0.40f, 1.0f) : ImVec4(0.25f, 0.65f, 0.95f, 1.0f);
							ImGui::PushStyleColor(ImGuiCol_PlotHistogram, bar_col);
							ImGui::ProgressBar(pct_frac, ImVec2(-1.0f, 12.0f), pct_label.c_str());
							ImGui::PopStyleColor();
						}
					}

					ImGui::Separator();
					float total_dps = damage_service::get_total_party_dps();
					ImGui::TextColored(ImVec4(0.7f, 0.75f, 0.8f, 1.0f), "Team Total: %u dmg | Avg DPS: %.1f/s", total_team_dmg, total_dps);
				}
				ImGui::End();
				ImGui::PopStyleColor(2);
				ImGui::PopStyleVar(4);
			}
		}

		// ==========================================
		// 4. DROPPED MATERIALS & SHINY ESP
		// ==========================================
		if (g_settings.window.overlay_dropped_materials)
		{
			auto drops = drop_service::get_active_drops();

			if (!drops.empty() || is_menu_open)
			{
				// Position: Bottom-Left
				ImGui::SetNextWindowPos(ImVec2(viewport->WorkPos.x + 20.0f, viewport->WorkPos.y + viewport->WorkSize.y - 24.0f), ImGuiCond_FirstUseEver, ImVec2(0.0f, 1.0f));
				ImGui::SetNextWindowSizeConstraints(ImVec2(320.0f, 0.0f), ImVec2(440.0f, 320.0f));

				ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.0f);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, is_menu_open ? 1.0f : 0.0f);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(12.0f, 10.0f));
				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 5.0f));
				ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.06f, 0.07f, 0.09f, is_menu_open ? 0.90f : 0.80f));
				ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.35f, 0.40f, 0.50f, 0.50f));

				if (ImGui::Begin("Dropped Materials ESP###DroppedMaterialsESP", nullptr, unfocused_flags))
				{
					int total_drops = drop_service::get_total_active_drops();
					float closest_dist = drop_service::get_closest_drop_distance();

					ImGui::TextColored(ImVec4(0.4f, 0.9f, 1.0f, 1.0f), "💎 Dropped Shiny Materials");
					ImGui::SameLine();
					ImGui::TextColored(ImVec4(1.0f, 0.85f, 0.35f, 1.0f), "[Count: %d]", total_drops > 0 ? total_drops : 2);
					ImGui::Separator();

					if (drops.empty())
					{
						// Preview drops
						struct preview_drop { const char* name; float dist; const char* dir; float time; bool gl; };
						const preview_drop preview_list[] = {
							{"Guiding Dragonbone", 14.2f, "NW", 28.5f, true},
							{"Monster Slinger Ammo", 22.0f, "E", 12.0f, false}
						};

						for (const auto& d : preview_list)
						{
							ImGui::TextColored(d.gl ? ImVec4(1.0f, 0.84f, 0.0f, 1.0f) : ImVec4(0.9f, 0.95f, 1.0f, 1.0f),
								"◈ %s", d.name);

							std::string dist_str = std::format("{:.1f}m [{}]", d.dist, d.dir);
							float dist_width = ImGui::CalcTextSize(dist_str.c_str()).x;
							ImGui::SameLine(ImGui::GetWindowWidth() - dist_width - 16.0f);
							ImGui::TextColored(ImVec4(0.35f, 0.85f, 1.0f, 1.0f), "%s", dist_str.c_str());

							float frac = d.time / 38.0f;
							std::string time_str = std::format("Despawn in {:.1f}s", d.time);

							ImVec4 bar_col = (d.time > 15.0f) ? ImVec4(0.2f, 0.8f, 0.4f, 1.0f) : ImVec4(0.95f, 0.3f, 0.2f, 1.0f);
							ImGui::PushStyleColor(ImGuiCol_PlotHistogram, bar_col);
							ImGui::ProgressBar(frac, ImVec2(-1.0f, 9.0f), time_str.c_str());
							ImGui::PopStyleColor();
						}
					}
					else
					{
						for (const auto& d : drops)
						{
							ImGui::TextColored(d.is_guiding_lands ? ImVec4(1.0f, 0.84f, 0.0f, 1.0f) : ImVec4(0.9f, 0.95f, 1.0f, 1.0f),
								"◈ %s", d.name.c_str());

							std::string dist_str = std::format("{:.1f}m [{}]", d.distance, d.direction.c_str());
							float dist_width = ImGui::CalcTextSize(dist_str.c_str()).x;
							ImGui::SameLine(ImGui::GetWindowWidth() - dist_width - 16.0f);
							ImGui::TextColored(ImVec4(0.35f, 0.85f, 1.0f, 1.0f), "%s", dist_str.c_str());

							float frac = std::clamp(d.time_remaining / d.max_time, 0.0f, 1.0f);
							std::string time_str = std::format("Despawn in {:.1f}s", d.time_remaining);

							ImVec4 bar_col;
							if (d.time_remaining > 20.0f)
								bar_col = ImVec4(0.2f, 0.8f, 0.4f, 1.0f);
							else if (d.time_remaining > 10.0f)
								bar_col = ImVec4(0.95f, 0.75f, 0.15f, 1.0f);
							else
								bar_col = ImVec4(0.95f, 0.25f, 0.25f, 1.0f);

							ImGui::PushStyleColor(ImGuiCol_PlotHistogram, bar_col);
							ImGui::ProgressBar(frac, ImVec2(-1.0f, 9.0f), time_str.c_str());
							ImGui::PopStyleColor();
						}
					}

					if (closest_dist > 0.0f && closest_dist < 9990.0f)
					{
						ImGui::Separator();
						ImGui::TextColored(ImVec4(0.7f, 0.75f, 0.8f, 1.0f), "Nearest Shiny: %.1f m", closest_dist);
					}
				}
				ImGui::End();
				ImGui::PopStyleColor(2);
				ImGui::PopStyleVar(4);
			}
		}

		// ==========================================
		// 5. HUNTER BUFF & MANTLE TIMERS OVERLAY
		// ==========================================
		if (g_settings.window.overlay_hunter_buffs)
		{
			auto buffs = character_service::get_active_buffs();
			auto mantles = character_service::get_mantles();

			if (!buffs.empty() || !mantles.empty() || is_menu_open)
			{
				// Position: Bottom-Right
				ImGui::SetNextWindowPos(ImVec2(viewport->WorkPos.x + viewport->WorkSize.x - 20.0f, viewport->WorkPos.y + viewport->WorkSize.y - 24.0f), ImGuiCond_FirstUseEver, ImVec2(1.0f, 1.0f));
				ImGui::SetNextWindowSizeConstraints(ImVec2(320.0f, 0.0f), ImVec2(440.0f, 360.0f));

				ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.0f);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, is_menu_open ? 1.0f : 0.0f);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(12.0f, 10.0f));
				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 5.0f));
				ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.06f, 0.07f, 0.09f, is_menu_open ? 0.90f : 0.80f));
				ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.35f, 0.40f, 0.50f, 0.50f));

				if (ImGui::Begin("Hunter Buff & Mantle Timers###HunterBuffTimers", nullptr, unfocused_flags))
				{
					ImGui::TextColored(ImVec4(0.4f, 1.0f, 0.7f, 1.0f), "🛡️ Hunter Buffs & Specialized Tools");
					ImGui::Separator();

					// Mantle Timers
					ImGui::TextColored(ImVec4(0.85f, 0.9f, 0.95f, 1.0f), "Specialized Tools (Mantles):");

					if (mantles.empty())
					{
						// Preview mantles
						struct preview_mantle { const char* name; float dur; float max_dur; float cd; float max_cd; bool act; bool on_cd; };
						const preview_mantle p_mantles[] = {
							{"Temporal Mantle", 52.4f, 120.0f, 0.0f, 150.0f, true, false},
							{"Rocksteady Mantle", 0.0f, 90.0f, 65.0f, 180.0f, false, true}
						};

						for (const auto& m : p_mantles)
						{
							if (m.act)
							{
								float frac = m.dur / m.max_dur;
								std::string label = std::format("{}: [ACTIVE] {:.1f}s", m.name, m.dur);
								ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.2f, 0.8f, 0.6f, 1.0f));
								ImGui::ProgressBar(frac, ImVec2(-1.0f, 13.0f), label.c_str());
								ImGui::PopStyleColor();
							}
							else if (m.on_cd)
							{
								float frac = m.cd / m.max_cd;
								std::string label = std::format("{}: [COOLDOWN] {:.1f}s", m.name, m.cd);
								ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.95f, 0.45f, 0.15f, 1.0f));
								ImGui::ProgressBar(frac, ImVec2(-1.0f, 13.0f), label.c_str());
								ImGui::PopStyleColor();
							}
						}
					}
					else
					{
						for (const auto& m : mantles)
						{
							if (m.is_active)
							{
								float frac = (m.max_duration > 0.0f) ? std::clamp(m.duration / m.max_duration, 0.0f, 1.0f) : 1.0f;
								std::string label = std::format("{}: [ACTIVE] {:.1f}s", m.name, m.duration);
								ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.2f, 0.8f, 0.6f, 1.0f));
								ImGui::ProgressBar(frac, ImVec2(-1.0f, 13.0f), label.c_str());
								ImGui::PopStyleColor();
							}
							else if (m.is_on_cooldown)
							{
								float frac = (m.max_cooldown > 0.0f) ? std::clamp(m.cooldown / m.max_cooldown, 0.0f, 1.0f) : 1.0f;
								std::string label = std::format("{}: [COOLDOWN] {:.1f}s", m.name, m.cooldown);
								ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.95f, 0.45f, 0.15f, 1.0f));
								ImGui::ProgressBar(frac, ImVec2(-1.0f, 13.0f), label.c_str());
								ImGui::PopStyleColor();
							}
							else
							{
								std::string label = std::format("{}: READY ✅", m.name);
								ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.2f, 0.6f, 0.3f, 0.8f));
								ImGui::ProgressBar(1.0f, ImVec2(-1.0f, 13.0f), label.c_str());
								ImGui::PopStyleColor();
							}
						}
					}

					// Hunter Active Buff Timers
					ImGui::Spacing();
					ImGui::TextColored(ImVec4(0.85f, 0.9f, 0.95f, 1.0f), "Consumable & Hunting Buffs:");

					if (buffs.empty())
					{
						// Preview buffs
						struct preview_buff { const char* name; float time; float max_t; bool perm; uint32_t col; };
						const preview_buff p_buffs[] = {
							{"Mega Demondrug", 1.0f, 1.0f, true, 0xFFDC143C},
							{"Might Seed", 115.0f, 180.0f, false, 0xFFFF8C00},
							{"Demon Powder", 82.0f, 180.0f, false, 0xFFFF4500},
							{"Dash Juice", 175.0f, 240.0f, false, 0xFF32CD32}
						};

						for (const auto& b : p_buffs)
						{
							ImVec4 col = ImGui::ColorConvertU32ToFloat4(b.col);
							float frac = b.perm ? 1.0f : (b.time / b.max_t);
							std::string label = b.perm ? std::format("{}: ACTIVE ⚔️", b.name) : std::format("{}: {:.0f}s", b.name, b.time);

							ImGui::PushStyleColor(ImGuiCol_PlotHistogram, col);
							ImGui::ProgressBar(frac, ImVec2(-1.0f, 11.0f), label.c_str());
							ImGui::PopStyleColor();
						}
					}
					else
					{
						for (const auto& b : buffs)
						{
							ImVec4 col = ImGui::ColorConvertU32ToFloat4(b.color);
							float frac = b.is_permanent ? 1.0f : (b.max_timer > 0.0f ? std::clamp(b.timer / b.max_timer, 0.0f, 1.0f) : 1.0f);
							std::string label = b.is_permanent
								? std::format("{}: ACTIVE ⚔️", b.name)
								: std::format("{}: {:.0f}s", b.name, b.timer);

							ImGui::PushStyleColor(ImGuiCol_PlotHistogram, col);
							ImGui::ProgressBar(frac, ImVec2(-1.0f, 11.0f), label.c_str());
							ImGui::PopStyleColor();
						}
					}
				}
				ImGui::End();
				ImGui::PopStyleColor(2);
				ImGui::PopStyleVar(4);
			}
		}
	}
}
