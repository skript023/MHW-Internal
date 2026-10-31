#include "overlay.h"

#include "game/mhw/character/character_service.hpp"
#include "game/mhw/monster/monster_service.hpp"
#include "game/mhw/world/world_service.hpp"
#include "game/mhw/damage/damage_service.hpp"
#include "game/mhw/drop/drop_service.hpp"
#include "game/mhw/party/party_service.hpp"
#include "game/mhw/camera/camera_service.hpp"
#include <astra/host/canvas.hpp>
#include "game/settings.hpp"
#include "core/ui/fonts/icon_list.hpp"
#include <imgui.h>
#include <vector>
#include <format>
#include <algorithm>
#include <cmath>

namespace big
{
	namespace mhw_gui
	{
		// ==========================================
		// MONSTER HUNTER: WORLD CANONICAL PALETTE
		// ==========================================
		constexpr ImU32 COLOR_BG_DARK           = 0xF00D0F13; // Deep Weathered Dark Slate (0.94 alpha)
		constexpr ImU32 COLOR_BG_CHANNEL        = 0xF808090C; // Recessed Metallic Track (0.97 alpha)
		constexpr ImU32 COLOR_BORDER_BRONZE     = 0xD09C7A48; // Hunter Antique Brass / Guild Gold
		constexpr ImU32 COLOR_BORDER_DARK       = 0xD028241D; // Etched Inner Border
		constexpr ImU32 COLOR_CORNER_GOLD       = 0xF0D4AF37; // Corner Filigree Accent
		constexpr ImU32 COLOR_TEXT_GOLD         = 0xFF7ED0F2; // MHW Title Gold (ABGR: #F2D07E)
		constexpr ImU32 COLOR_TEXT_IVORY        = 0xFFE2EAF0; // Warm Parchment Ivory
		constexpr ImU32 COLOR_TEXT_MUTED        = 0xFFA09B90; // Weathered Stone Gray
		constexpr ImU32 COLOR_SHADOW            = 0xD0000000; // Text & Element Drop Shadow

		// MHW Gauge Fill Colors (Top / Bottom gradients)
		// Health Gauges
		constexpr ImU32 HP_GREEN_TOP            = 0xFF4AE06E; // Vibrant MHW Hunter Green
		constexpr ImU32 HP_GREEN_BOT            = 0xFF229E3E;
		constexpr ImU32 HP_WARN_TOP             = 0xFF35B0F8; // MHW Caution Amber
		constexpr ImU32 HP_WARN_BOT             = 0xFF1878D0;
		constexpr ImU32 HP_CRIT_TOP             = 0xFF3E4CE8; // MHW Critical Crimson
		constexpr ImU32 HP_CRIT_BOT             = 0xFF1E28A0;
		constexpr ImU32 HP_DEAD_TOP             = 0xFF585C66; // Muted Slate Gray
		constexpr ImU32 HP_DEAD_BOT             = 0xFF383C44;

		// Combat & Specialized Gauges
		constexpr ImU32 ENRAGE_TOP              = 0xFF2088FF; // Fiery Agitator Vermilion
		constexpr ImU32 ENRAGE_BOT              = 0xFF1030D0;
		constexpr ImU32 TAIL_TOP                = 0xFFF09838; // MHW Severable Steel Blue
		constexpr ImU32 TAIL_BOT                = 0xFFB85818;
		constexpr ImU32 PART_TOP                = 0xFF70C840; // MHW Breakable Jade
		constexpr ImU32 PART_BOT                = 0xFF408820;

		// Specialized Tools / Mantles
		constexpr ImU32 MANTLE_ACT_TOP          = 0xFF42E088; // Emerald Mantle Glow
		constexpr ImU32 MANTLE_ACT_BOT          = 0xFF1E9848;
		constexpr ImU32 MANTLE_CD_TOP           = 0xFF2888F8; // Cooldown Amber
		constexpr ImU32 MANTLE_CD_BOT           = 0xFF1448D0;
		constexpr ImU32 MANTLE_RDY_TOP          = 0xFF40D0F8; // Polished Ready Gold
		constexpr ImU32 MANTLE_RDY_BOT          = 0xFF2088C8;

		// Party Contribution Gauges
		constexpr ImU32 DPS_CHAMP_TOP           = 0xFF40D0F8; // Radiant Guild Champion Gold
		constexpr ImU32 DPS_CHAMP_BOT           = 0xFF1890C8;
		constexpr ImU32 DPS_TEAM_TOP            = 0xFFF09838; // Teammate Guild Blue
		constexpr ImU32 DPS_TEAM_BOT            = 0xFFB85818;
		constexpr ImU32 DPS_SELF_TOP            = 0xFF4AE06E; // Local Player Hunter Green
		constexpr ImU32 DPS_SELF_BOT            = 0xFF229E3E;

		// Shiny Materials
		constexpr ImU32 SHINY_GOLD_TOP          = 0xFF30D8FF; // Guiding Lands Sacred Amber
		constexpr ImU32 SHINY_GOLD_BOT          = 0xFF1088C8;
		constexpr ImU32 SHINY_CYAN_TOP          = 0xFFFFF000; // Slinger Turquoise Cyan
		constexpr ImU32 SHINY_CYAN_BOT          = 0xFFD09000;
		constexpr ImU32 SHINY_PURP_TOP          = 0xFFF0B8FF; // Diamond Crystal Lilac
		constexpr ImU32 SHINY_PURP_BOT          = 0xFFB068D0;

		// ==========================================
		// MHW UI DECORATION HELPERS
		// ==========================================
		inline void draw_corner_brackets(ImVec2 min, ImVec2 max, ImU32 col = COLOR_CORNER_GOLD, float len = 8.0f, float thick = 1.5f)
		{
			auto* dl = ImGui::GetWindowDrawList();
			// Top-Left
			dl->AddLine(min, ImVec2(min.x + len, min.y), col, thick);
			dl->AddLine(min, ImVec2(min.x, min.y + len), col, thick);
			// Top-Right
			dl->AddLine(ImVec2(max.x, min.y), ImVec2(max.x - len, min.y), col, thick);
			dl->AddLine(ImVec2(max.x, min.y), ImVec2(max.x, min.y + len), col, thick);
			// Bottom-Left
			dl->AddLine(ImVec2(min.x, max.y), ImVec2(min.x + len, max.y), col, thick);
			dl->AddLine(ImVec2(min.x, max.y), ImVec2(min.x, max.y - len), col, thick);
			// Bottom-Right
			dl->AddLine(max, ImVec2(max.x - len, max.y), col, thick);
			dl->AddLine(max, ImVec2(max.x, max.y - len), col, thick);
		}

		inline void draw_ornate_divider(float y_offset = 2.0f, ImU32 line_col = 0x80987545)
		{
			auto* dl = ImGui::GetWindowDrawList();
			float x1 = ImGui::GetCursorScreenPos().x;
			float x2 = x1 + ImGui::GetContentRegionAvail().x;
			float y = ImGui::GetCursorScreenPos().y + y_offset;
			float cx = (x1 + x2) * 0.5f;

			// Double gradient lines fading toward center diamond
			dl->AddRectFilledMultiColor(ImVec2(x1, y - 0.5f), ImVec2(cx - 8.0f, y + 0.5f), 0x00987545, line_col, line_col, 0x00987545);
			dl->AddRectFilledMultiColor(ImVec2(cx + 8.0f, y - 0.5f), ImVec2(x2, y + 0.5f), line_col, 0x00987545, 0x00987545, line_col);

			// Center diamond emblem
			constexpr float r = 3.5f;
			dl->AddQuadFilled(
				ImVec2(cx, y - r),
				ImVec2(cx + r, y),
				ImVec2(cx, y + r),
				ImVec2(cx - r, y),
				COLOR_CORNER_GOLD
			);

			ImGui::Dummy(ImVec2(0.0f, y_offset * 2.0f + 2.0f));
		}

		inline void draw_gauge_bar(
			float fraction,
			float width,
			float height,
			ImU32 col_top,
			ImU32 col_bot,
			const char* label = nullptr,
			ImU32 text_col = COLOR_TEXT_IVORY,
			bool show_pip = true
		)
		{
			fraction = std::clamp(fraction, 0.0f, 1.0f);
			if (width <= 0.0f)
				width = ImGui::GetContentRegionAvail().x;

			ImVec2 p_min = ImGui::GetCursorScreenPos();
			ImVec2 p_max = ImVec2(p_min.x + width, p_min.y + height);
			auto* dl = ImGui::GetWindowDrawList();

			// 1. Recessed dark metallic track
			dl->AddRectFilled(p_min, p_max, COLOR_BG_CHANNEL, 0.0f);
			dl->AddRect(p_min, p_max, COLOR_BORDER_DARK, 0.0f, 0, 1.0f);

			// 2. Filled Bar with two-tone vertical gradient
			float fill_w = width * fraction;
			if (fill_w > 1.0f)
			{
				ImVec2 fill_max = ImVec2(p_min.x + fill_w, p_max.y);
				dl->AddRectFilledMultiColor(p_min, fill_max, col_top, col_top, col_bot, col_bot);

				// 1px metallic sheen along top inner edge
				dl->AddLine(
					ImVec2(p_min.x + 1.0f, p_min.y + 1.0f),
					ImVec2(fill_max.x - 1.0f, p_min.y + 1.0f),
					0x60FFFFFF, 1.0f
				);

				// 3. Leading edge bright indicator pip (MHW canonical gauge threshold)
				if (show_pip && fraction < 0.999f && fill_w >= 2.0f)
				{
					dl->AddLine(
						ImVec2(p_min.x + fill_w - 0.5f, p_min.y),
						ImVec2(p_min.x + fill_w - 0.5f, p_max.y),
						0xFFFFF8E0, 1.5f
					);
				}
			}

			// 4. Outer etched bronze frame
			dl->AddRect(p_min, p_max, COLOR_BORDER_BRONZE, 0.0f, 0, 1.0f);

			// 5. Label overlay with dark drop shadow for perfect visibility
			if (label && label[0] != '\0')
			{
				ImVec2 text_sz = ImGui::CalcTextSize(label);
				ImVec2 text_pos(
					p_min.x + (width - text_sz.x) * 0.5f,
					p_min.y + (height - text_sz.y) * 0.5f - 0.5f
				);

				dl->AddText(ImVec2(text_pos.x + 1.0f, text_pos.y + 1.0f), COLOR_SHADOW, label);
				dl->AddText(text_pos, text_col, label);
			}

			ImGui::Dummy(ImVec2(width, height));
		}
	}

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

		// Push MHW Global HUD Style (Sharp geometric edges, weathered dark slate, antique brass borders)
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(14.0f, 10.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 5.0f));
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.045f, 0.048f, 0.054f, is_menu_open ? 0.95f : 0.88f));
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.68f, 0.55f, 0.35f, 0.70f));

		// ==========================================
		// 1. HUNTER INFO OVERLAY (MHW GUILD CARD)
		// ==========================================
		if (g_settings.window.overlay_hunter)
		{
			ImGui::SetNextWindowPos(ImVec2(viewport->WorkPos.x + 20.0f, viewport->WorkPos.y + 24.0f), ImGuiCond_FirstUseEver, ImVec2(0.0f, 0.0f));
			ImGui::SetNextWindowSizeConstraints(ImVec2(280.0f, 0.0f), ImVec2(380.0f, FLT_MAX));

			if (ImGui::Begin("Guild Card: Hunter Info###HunterGuildCard", nullptr, unfocused_flags))
			{
				mhw_gui::draw_corner_brackets(ImGui::GetWindowPos(), ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowWidth(), ImGui::GetWindowPos().y + ImGui::GetWindowHeight()));

				ImGui::TextColored(ImVec4(0.95f, 0.82f, 0.44f, 1.0f), ICON_FA_DIAMOND " GUILD CARD: HUNTER " ICON_FA_DIAMOND);
				mhw_gui::draw_ornate_divider();

				std::string hunter_name = character_service::get_name();
				if (hunter_name.empty())
					hunter_name = "Hunter";

				ImGui::TextColored(ImVec4(0.92f, 0.90f, 0.85f, 1.0f), "%s", hunter_name.c_str());
				ImGui::SameLine(ImGui::GetWindowWidth() - 95.0f);
				ImGui::TextColored(ImVec4(1.0f, 0.84f, 0.0f, 1.0f), "[HR %u]", character_service::get_high_rank_level());

				ImGui::TextColored(ImVec4(0.85f, 0.88f, 0.95f, 1.0f), "Master Rank: Lvl %u", character_service::get_master_rank_level());
				ImGui::TextColored(ImVec4(1.0f, 0.85f, 0.35f, 1.0f), "Funds: %u z", character_service::get_zeni());
				ImGui::TextColored(ImVec4(0.40f, 0.85f, 1.0f, 1.0f), "Research: %u pts", character_service::get_research_points());

				float tod = world_service::get_time_of_date();
				std::string day_phase = (tod >= 6.f && tod < 18.f) ? ("Day " ICON_FA_SUN) : (tod >= 18.f && tod < 20.f) ? ("Dusk " ICON_FA_CLOUD_SUN) : ("Night " ICON_FA_MOON);
				std::string weather = world_service::get_weather();
				ImGui::TextColored(ImVec4(0.75f, 0.78f, 0.82f, 1.0f), "Field: %s | %s", weather.c_str(), day_phase.c_str());

				auto pos = character_service::get_position();
				ImGui::TextColored(ImVec4(0.60f, 0.62f, 0.66f, 1.0f), "Coord: [%.1f, %.1f, %.1f]", pos.x, pos.y, pos.z);

				auto wp = character_service::get_waypoint_pos();
				if (wp.x != 0.f || wp.y != 0.f || wp.z != 0.f)
				{
					ImGui::TextColored(ImVec4(0.35f, 0.85f, 1.0f, 1.0f), ICON_FA_COMPASS " Waypoint: %.1f m", pos.distance(wp));
				}
			}
			ImGui::End();
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
				ImGui::SetNextWindowSizeConstraints(ImVec2(460.0f, 0.0f), ImVec2(640.0f, FLT_MAX));

				if (ImGui::Begin("Monster Health Overlay###MonsterHealthOverlay", nullptr, unfocused_flags))
				{
					mhw_gui::draw_corner_brackets(ImGui::GetWindowPos(), ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowWidth(), ImGui::GetWindowPos().y + ImGui::GetWindowHeight()));

					if (active_monsters.empty())
					{
						// Preview Card in Full MHW Styling
						ImGui::TextColored(ImVec4(0.95f, 0.82f, 0.44f, 1.0f), ICON_FA_DIAMOND " RATHALOS " ICON_FA_DIAMOND);
						if (g_settings.window.overlay_crown)
						{
							ImGui::SameLine();
							ImGui::TextColored(ImVec4(1.0f, 0.84f, 0.0f, 1.0f), "[" ICON_FA_CROWN " Giant Gold] (124.5%)");
						}

						std::string dist_str = "28.5 m [MID]";
						float dist_width = ImGui::CalcTextSize(dist_str.c_str()).x;
						ImGui::SameLine(ImGui::GetWindowWidth() - dist_width - 16.0f);
						ImGui::TextColored(ImVec4(1.0f, 0.85f, 0.35f, 1.0f), "%s", dist_str.c_str());

						mhw_gui::draw_ornate_divider();

						// Weakness HUD
						if (g_settings.window.overlay_monster_weakness)
						{
							ImGui::TextColored(ImVec4(0.84f, 0.75f, 0.58f, 1.0f), "Weakness:");
							ImGui::SameLine();
							ImGui::TextColored(ImVec4(1.0f, 0.35f, 0.25f, 1.0f), ICON_FA_FIRE " " ICON_FA_TIMES);
							ImGui::SameLine();
							ImGui::TextColored(ImVec4(0.25f, 0.65f, 1.0f, 1.0f), ICON_FA_TINT " " ICON_FA_STAR);
							ImGui::SameLine();
							ImGui::TextColored(ImVec4(1.0f, 0.85f, 0.2f, 1.0f), ICON_FA_BOLT " " ICON_FA_STAR ICON_FA_STAR);
							ImGui::SameLine();
							ImGui::TextColored(ImVec4(0.4f, 0.95f, 1.0f, 1.0f), ICON_FA_SNOWFLAKE " " ICON_FA_STAR);
							ImGui::SameLine();
							ImGui::TextColored(ImVec4(0.75f, 0.45f, 0.95f, 1.0f), ICON_FA_DRAGON " " ICON_FA_STAR ICON_FA_STAR ICON_FA_STAR);

							ImGui::TextColored(ImVec4(0.84f, 0.75f, 0.58f, 1.0f), "Status:");
							ImGui::SameLine();
							ImGui::TextColored(ImVec4(0.85f, 0.55f, 0.95f, 1.0f), ICON_FA_FLASK_POISON " " ICON_FA_STAR);
							ImGui::SameLine();
							ImGui::TextColored(ImVec4(0.55f, 0.85f, 1.0f, 1.0f), ICON_FA_BED " " ICON_FA_STAR ICON_FA_STAR);
							ImGui::SameLine();
							ImGui::TextColored(ImVec4(1.0f, 0.85f, 0.2f, 1.0f), ICON_FA_BOLT " " ICON_FA_STAR ICON_FA_STAR);
							ImGui::SameLine();
							ImGui::TextColored(ImVec4(1.0f, 0.45f, 0.2f, 1.0f), ICON_FA_BOMB " " ICON_FA_STAR);
							ImGui::SameLine();
							ImGui::TextColored(ImVec4(1.0f, 0.75f, 0.3f, 1.0f), ICON_FA_DIZZY " " ICON_FA_STAR ICON_FA_STAR);
							ImGui::Spacing();
						}

						// Main HP Gauge
						mhw_gui::draw_gauge_bar(0.85f, -1.0f, 18.0f, mhw_gui::HP_GREEN_TOP, mhw_gui::HP_GREEN_BOT, "12,750 / 15,000 (85.0%)");

						// Enrage Gauge
						if (g_settings.window.overlay_enrage_bar)
						{
							ImGui::Spacing();
							mhw_gui::draw_gauge_bar(0.72f, -1.0f, 12.0f, mhw_gui::ENRAGE_TOP, mhw_gui::ENRAGE_BOT, ICON_FA_FIRE " Enrage / Agitator: 65.0s / 90.0s (72%)");
						}

						// Severable Tail & Body Parts Preview
						if (g_settings.window.overlay_monster_parts)
						{
							ImGui::Spacing();
							ImGui::TextColored(ImVec4(0.84f, 0.75f, 0.58f, 1.0f), "Body Parts & Severable Tail:");

							// Tail Gauge
							mhw_gui::draw_gauge_bar(0.62f, -1.0f, 13.0f, mhw_gui::TAIL_TOP, mhw_gui::TAIL_BOT, ICON_FA_CUT " Tail [Severable Cut]: 930 / 1,500 HP (62.0%)");
							if (g_settings.window.overlay_hitzone_values)
								ImGui::TextColored(ImVec4(1.0f, 0.85f, 0.25f, 1.0f), ICON_FA_SWORD " 60 " ICON_FA_HAMMER " 50 " ICON_FA_BOW_ARROW " 45 " ICON_FA_DRAGON " 25 [" ICON_FA_CROSSHAIRS " WEX]");

							// Sample Sub-Parts
							ImGui::Columns(2, "PreviewPartsColumns", false);
							mhw_gui::draw_gauge_bar(0.90f, -1.0f, 10.0f, mhw_gui::PART_TOP, mhw_gui::PART_BOT, "Head: 1,080 / 1,200");
							if (g_settings.window.overlay_hitzone_values)
								ImGui::TextColored(ImVec4(1.0f, 0.85f, 0.25f, 1.0f), ICON_FA_SWORD " 65 " ICON_FA_HAMMER " 65 " ICON_FA_BOW_ARROW " 55 " ICON_FA_DRAGON " 30 [" ICON_FA_CROSSHAIRS " WEX]");
							mhw_gui::draw_gauge_bar(0.75f, -1.0f, 10.0f, mhw_gui::PART_TOP, mhw_gui::PART_BOT, "Left Wing: 675 / 900");
							if (g_settings.window.overlay_hitzone_values)
								ImGui::TextColored(ImVec4(1.0f, 0.85f, 0.25f, 1.0f), ICON_FA_SWORD " 45 " ICON_FA_HAMMER " 40 " ICON_FA_BOW_ARROW " 45 " ICON_FA_DRAGON " 20 [" ICON_FA_CROSSHAIRS " WEX]");

							ImGui::NextColumn();
							mhw_gui::draw_gauge_bar(0.85f, -1.0f, 10.0f, mhw_gui::PART_TOP, mhw_gui::PART_BOT, "Body: 2,550 / 3,000");
							if (g_settings.window.overlay_hitzone_values)
								ImGui::TextColored(ImVec4(0.65f, 0.70f, 0.78f, 1.0f), ICON_FA_SWORD " 25 " ICON_FA_HAMMER " 25 " ICON_FA_BOW_ARROW " 20 " ICON_FA_DRAGON " 15");
							mhw_gui::draw_gauge_bar(0.80f, -1.0f, 10.0f, mhw_gui::PART_TOP, mhw_gui::PART_BOT, "Right Wing: 720 / 900");
							if (g_settings.window.overlay_hitzone_values)
								ImGui::TextColored(ImVec4(1.0f, 0.85f, 0.25f, 1.0f), ICON_FA_SWORD " 45 " ICON_FA_HAMMER " 40 " ICON_FA_BOW_ARROW " 45 " ICON_FA_DRAGON " 20 [" ICON_FA_CROSSHAIRS " WEX]");
							ImGui::Columns(1);
						}

						// Status Buildup Preview
						if (g_settings.window.overlay_monster_ailments)
						{
							ImGui::Spacing();
							ImGui::TextColored(ImVec4(0.84f, 0.75f, 0.58f, 1.0f), "Status Ailments Buildup:");
							ImGui::Columns(3, "PreviewAilmentColumns", false);
							mhw_gui::draw_gauge_bar(0.75f, -1.0f, 10.0f, 0xFFBA55D3, 0xFF8A2BE2, ICON_FA_FLASK_POISON " 112/150");
							mhw_gui::draw_gauge_bar(0.40f, -1.0f, 10.0f, 0xFFFFD700, 0xFFB8860B, ICON_FA_BOLT " 72/180");
							ImGui::NextColumn();
							mhw_gui::draw_gauge_bar(0.30f, -1.0f, 10.0f, 0xFF87CEEB, 0xFF4682B4, ICON_FA_BED " 39/130");
							mhw_gui::draw_gauge_bar(0.85f, -1.0f, 10.0f, 0xFFFF4500, 0xFFB22222, ICON_FA_BOMB " 119/140");
							ImGui::NextColumn();
							mhw_gui::draw_gauge_bar(0.55f, -1.0f, 10.0f, 0xFFFFA500, 0xFFCC7000, ICON_FA_DIZZY " 88/160");
							mhw_gui::draw_gauge_bar(0.45f, -1.0f, 10.0f, 0xFF20B2AA, 0xFF008080, ICON_FA_HEARTBEAT " 90/200");
							ImGui::Columns(1);
						}

						ImGui::Spacing();
						ImGui::TextColored(ImVec4(0.60f, 0.62f, 0.66f, 1.0f), "Pos: [120.5, 45.2, -80.0]");
					}
					else
					{
						for (size_t idx = 0; idx < active_monsters.size(); idx++)
						{
							const auto& m = active_monsters[idx];
							if (idx > 0)
							{
								ImGui::Spacing();
								mhw_gui::draw_ornate_divider(4.0f);
								ImGui::Spacing();
							}

							// Header: Monster Name + Crown Badge
							ImGui::TextColored(ImVec4(0.95f, 0.82f, 0.44f, 1.0f), ICON_FA_DIAMOND " %s " ICON_FA_DIAMOND, m.name.c_str());

							if (g_settings.window.overlay_crown && m.size > 0.01f)
							{
								ImGui::SameLine();
								if (m.size <= 0.90f)
								{
									ImGui::TextColored(ImVec4(1.0f, 0.84f, 0.0f, 1.0f), "[" ICON_FA_CROWN " Mini Crown] (%.1f%%)", m.size * 100.0f);
								}
								else if (m.size >= 1.23f)
								{
									ImGui::TextColored(ImVec4(1.0f, 0.84f, 0.0f, 1.0f), "[" ICON_FA_CROWN " Giant Gold] (%.1f%%)", m.size * 100.0f);
								}
								else if (m.size >= 1.15f)
								{
									ImGui::TextColored(ImVec4(0.85f, 0.88f, 0.95f, 1.0f), "[" ICON_FA_MEDAL " Silver Crown] (%.1f%%)", m.size * 100.0f);
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

							mhw_gui::draw_ornate_divider();

							// Weakness HUD
							if (g_settings.window.overlay_monster_weakness)
							{
								auto profile = monster_service::get_combat_profile(m.index);
								auto format_stars = [](uint8_t count) -> const char* {
									switch (count) {
									case 3: return ICON_FA_STAR " " ICON_FA_STAR " " ICON_FA_STAR;
									case 2: return ICON_FA_STAR " " ICON_FA_STAR;
									case 1: return ICON_FA_STAR;
									default: return ICON_FA_TIMES;
									}
								};
								ImGui::TextColored(ImVec4(0.84f, 0.75f, 0.58f, 1.0f), "Weakness:");
								ImGui::SameLine();
								ImGui::TextColored(ImVec4(1.0f, 0.35f, 0.25f, 1.0f), ICON_FA_FIRE " %s", format_stars(profile.element.fire));
								ImGui::SameLine();
								ImGui::TextColored(ImVec4(0.25f, 0.65f, 1.0f, 1.0f), ICON_FA_TINT " %s", format_stars(profile.element.water));
								ImGui::SameLine();
								ImGui::TextColored(ImVec4(1.0f, 0.85f, 0.2f, 1.0f), ICON_FA_BOLT " %s", format_stars(profile.element.thunder));
								ImGui::SameLine();
								ImGui::TextColored(ImVec4(0.4f, 0.95f, 1.0f, 1.0f), ICON_FA_SNOWFLAKE " %s", format_stars(profile.element.ice));
								ImGui::SameLine();
								ImGui::TextColored(ImVec4(0.75f, 0.45f, 0.95f, 1.0f), ICON_FA_DRAGON " %s", format_stars(profile.element.dragon));

								ImGui::TextColored(ImVec4(0.84f, 0.75f, 0.58f, 1.0f), "Status:");
								ImGui::SameLine();
								ImGui::TextColored(ImVec4(0.85f, 0.55f, 0.95f, 1.0f), ICON_FA_FLASK_POISON " %s", format_stars(profile.status.poison));
								ImGui::SameLine();
								ImGui::TextColored(ImVec4(0.55f, 0.85f, 1.0f, 1.0f), ICON_FA_BED " %s", format_stars(profile.status.sleep));
								ImGui::SameLine();
								ImGui::TextColored(ImVec4(1.0f, 0.85f, 0.2f, 1.0f), ICON_FA_BOLT " %s", format_stars(profile.status.paralysis));
								ImGui::SameLine();
								ImGui::TextColored(ImVec4(1.0f, 0.45f, 0.2f, 1.0f), ICON_FA_BOMB " %s", format_stars(profile.status.blast));
								ImGui::SameLine();
								ImGui::TextColored(ImVec4(1.0f, 0.75f, 0.3f, 1.0f), ICON_FA_DIZZY " %s", format_stars(profile.status.stun));
								ImGui::Spacing();
							}

							// Main HP Gauge
							float fraction = m.max_health > 0.0f ? std::clamp(m.health / m.max_health, 0.0f, 1.0f) : 0.0f;
							std::string hp_text = std::format("{:.0f} / {:.0f} ({:.1f}%)", m.health, m.max_health, fraction * 100.0f);

							ImU32 hp_top, hp_bot;
							if (m.health <= 0.0f)
							{
								hp_top = mhw_gui::HP_DEAD_TOP;
								hp_bot = mhw_gui::HP_DEAD_BOT;
								hp_text = "DEAD / CAPTURED";
							}
							else if (fraction > 0.50f)
							{
								hp_top = mhw_gui::HP_GREEN_TOP;
								hp_bot = mhw_gui::HP_GREEN_BOT;
							}
							else if (fraction > 0.25f)
							{
								hp_top = mhw_gui::HP_WARN_TOP;
								hp_bot = mhw_gui::HP_WARN_BOT;
							}
							else
							{
								hp_top = mhw_gui::HP_CRIT_TOP;
								hp_bot = mhw_gui::HP_CRIT_BOT;
							}

							mhw_gui::draw_gauge_bar(fraction, -1.0f, 18.0f, hp_top, hp_bot, hp_text.c_str());

							// Enrage Gauge
							if (g_settings.window.overlay_enrage_bar && m.timer > 0.0f)
							{
								ImGui::Spacing();
								float timer_frac = (m.max_timer > 0.0f) ? std::clamp(m.timer / m.max_timer, 0.0f, 1.0f) : 1.0f;
								std::string enrage_label = (m.max_timer > 0.0f)
									? std::format(ICON_FA_FIRE " Enrage / Agitator: {:.1f}s / {:.1f}s ({:.0f}%)", m.timer, m.max_timer, timer_frac * 100.0f)
									: std::format(ICON_FA_FIRE " Enrage / Agitator: {:.1f}s", m.timer);

								mhw_gui::draw_gauge_bar(timer_frac, -1.0f, 12.0f, mhw_gui::ENRAGE_TOP, mhw_gui::ENRAGE_BOT, enrage_label.c_str());
							}

							// Sub-fitur 1: Monster Part Health & Severable Tail Bar
							if (g_settings.window.overlay_monster_parts && !m.parts.empty())
							{
								ImGui::Spacing();
								ImGui::TextColored(ImVec4(0.84f, 0.75f, 0.58f, 1.0f), "Body Parts & Severable Tail:");

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
										mhw_gui::draw_gauge_bar(1.0f, -1.0f, 13.0f, mhw_gui::PART_TOP, mhw_gui::PART_BOT, ICON_FA_CUT " Tail [SEVERED]");
									}
									else
									{
										float tail_frac = tail_part->max_health > 0.0f ? std::clamp(tail_part->health / tail_part->max_health, 0.0f, 1.0f) : 0.0f;
										std::string tail_str = std::format(ICON_FA_CUT " Tail Cut Remaining: {:.0f} / {:.0f} ({:.1f}%)", tail_part->health, tail_part->max_health, tail_frac * 100.0f);

										mhw_gui::draw_gauge_bar(tail_frac, -1.0f, 13.0f, mhw_gui::TAIL_TOP, mhw_gui::TAIL_BOT, tail_str.c_str());

										if (g_settings.window.overlay_hitzone_values && tail_part->slash_hzv > 0)
										{
											std::string hzv_str = std::format(ICON_FA_SWORD " {} " ICON_FA_HAMMER " {} " ICON_FA_BOW_ARROW " {} {}{}", tail_part->slash_hzv, tail_part->blunt_hzv, tail_part->shot_hzv, tail_part->elem_type.substr(0, 2), tail_part->elem_hzv);
											if (tail_part->is_weakpoint)
												ImGui::TextColored(ImVec4(1.0f, 0.85f, 0.25f, 1.0f), "%s [" ICON_FA_CROSSHAIRS " WEX]", hzv_str.c_str());
											else
												ImGui::TextColored(ImVec4(0.65f, 0.70f, 0.78f, 1.0f), "%s", hzv_str.c_str());
										}
									}
								}

								// Other parts (2 columns)
								ImGui::Columns(2, "PartsColumns", false);
								for (size_t p_idx = 0; p_idx < m.parts.size(); p_idx++)
								{
									const auto& p = m.parts[p_idx];
									if (&p == tail_part)
										continue;

									float p_frac = p.max_health > 0.0f ? std::clamp(p.health / p.max_health, 0.0f, 1.0f) : 0.0f;
									std::string p_str = std::format("{}: {:.0f}/{:.0f}", p.name, p.health, p.max_health);

									mhw_gui::draw_gauge_bar(p_frac, -1.0f, 10.0f, mhw_gui::PART_TOP, mhw_gui::PART_BOT, p_str.c_str());

									if (g_settings.window.overlay_hitzone_values && p.slash_hzv > 0)
									{
										std::string hzv_str = std::format(ICON_FA_SWORD " {} " ICON_FA_HAMMER " {} " ICON_FA_BOW_ARROW " {} {}{}", p.slash_hzv, p.blunt_hzv, p.shot_hzv, p.elem_type.substr(0, 2), p.elem_hzv);
										if (p.is_weakpoint)
											ImGui::TextColored(ImVec4(1.0f, 0.85f, 0.25f, 1.0f), "%s [" ICON_FA_CROSSHAIRS " WEX]", hzv_str.c_str());
										else
											ImGui::TextColored(ImVec4(0.65f, 0.70f, 0.78f, 1.0f), "%s", hzv_str.c_str());
									}

									if (p_idx % 2 == 1)
										ImGui::NextColumn();
								}
								ImGui::Columns(1);
							}

							// Sub-fitur 2: Monster Status Buildup (Ailments Tracking)
							if (g_settings.window.overlay_monster_ailments && !m.ailments.empty())
							{
								ImGui::Spacing();
								ImGui::TextColored(ImVec4(0.84f, 0.75f, 0.58f, 1.0f), "Status Ailments Buildup:");
								ImGui::Columns(3, "AilmentColumns", false);

								for (size_t a_idx = 0; a_idx < m.ailments.size(); a_idx++)
								{
									const auto& a = m.ailments[a_idx];
									float a_frac = a.max_buildup > 0.0f ? std::clamp(a.buildup / a.max_buildup, 0.0f, 1.0f) : 0.0f;
									
									const char* ail_icon = ICON_FA_DIAMOND;
									if (a.name.find("Poison") != std::string::npos) ail_icon = ICON_FA_FLASK_POISON;
									else if (a.name.find("Para") != std::string::npos) ail_icon = ICON_FA_BOLT;
									else if (a.name.find("Sleep") != std::string::npos) ail_icon = ICON_FA_BED;
									else if (a.name.find("Blast") != std::string::npos) ail_icon = ICON_FA_BOMB;
									else if (a.name.find("Stun") != std::string::npos) ail_icon = ICON_FA_DIZZY;
									else if (a.name.find("Exhaust") != std::string::npos) ail_icon = ICON_FA_HEARTBEAT;

									std::string a_str = a.is_active
										? std::format("{} [ACT] {:.0f}s", ail_icon, a.duration)
										: std::format("{} {:.0f}/{:.0f}", ail_icon, a.buildup, a.max_buildup);

									ImU32 col_top = a.color;
									ImU32 col_bot = (a.color & 0x00FFFFFF) | 0xA0000000;

									mhw_gui::draw_gauge_bar(a.is_active ? 1.0f : a_frac, -1.0f, 10.0f, col_top, col_bot, a_str.c_str());

									if (a_idx % 3 == 2)
										ImGui::NextColumn();
								}
								ImGui::Columns(1);
							}

							// Meta line
							std::string meta_line = std::format("Pos: [{:.1f}, {:.1f}, {:.1f}]", m.position.x, m.position.y, m.position.z);
							ImGui::TextColored(ImVec4(0.60f, 0.62f, 0.66f, 1.0f), "%s", meta_line.c_str());

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

				if (ImGui::Begin("Party Damage Meter###PartyDamageMeter", nullptr, unfocused_flags))
				{
					mhw_gui::draw_corner_brackets(ImGui::GetWindowPos(), ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowWidth(), ImGui::GetWindowPos().y + ImGui::GetWindowHeight()));

					int minutes = static_cast<int>(hunt_time) / 60;
					int seconds = static_cast<int>(hunt_time) % 60;

					ImGui::TextColored(ImVec4(0.95f, 0.82f, 0.44f, 1.0f), ICON_FA_DIAMOND " HUNT CONTRIBUTION " ICON_FA_DIAMOND);
					ImGui::SameLine();
					ImGui::TextColored(ImVec4(0.70f, 0.75f, 0.85f, 1.0f), "[" ICON_FA_CLOCK " %02d:%02d]", minutes, seconds);

					if (is_menu_open)
					{
						ImGui::SameLine(ImGui::GetWindowWidth() - 75.0f);
						if (ImGui::SmallButton("Reset"))
						{
							damage_service::reset();
						}
					}

					mhw_gui::draw_ornate_divider();

					if (party.empty() || total_team_dmg == 0)
					{
						// Preview team rows
						struct preview_player { const char* icon; const char* name; const char* weapon; uint32_t dmg; float dps; float pct; int carts; };
						const preview_player preview_team[] = {
							{ICON_FA_TROPHY, "Hunter (You)", "Great Sword", 14500, 241.6f, 58.0f, 0},
							{ICON_FA_MEDAL, "Ace Lancer", "Lance", 6200, 103.3f, 24.8f, 0},
							{ICON_FA_AWARD, "Bow Master", "Bow", 4300, 71.6f, 17.2f, 1}
						};

						for (size_t p_idx = 0; p_idx < 3; p_idx++)
						{
							const auto& p = preview_team[p_idx];
							ImGui::TextColored((p_idx == 0) ? ImVec4(1.0f, 0.85f, 0.35f, 1.0f) : ImVec4(0.92f, 0.90f, 0.85f, 1.0f),
								"%s %s (%s)", p.icon, p.name, p.weapon);
							ImGui::SameLine(ImGui::GetWindowWidth() - 110.0f);
							ImGui::TextColored(ImVec4(0.95f, 0.82f, 0.44f, 1.0f), "%u (%5.1f/s)", p.dmg, p.dps);

							float pct_frac = p.pct / 100.0f;
							std::string pct_label = std::format("{:.1f}% [" ICON_FA_SKULL " {}]", p.pct, p.carts);

							ImU32 dps_top = (p_idx == 0) ? mhw_gui::DPS_CHAMP_TOP : mhw_gui::DPS_TEAM_TOP;
							ImU32 dps_bot = (p_idx == 0) ? mhw_gui::DPS_CHAMP_BOT : mhw_gui::DPS_TEAM_BOT;

							mhw_gui::draw_gauge_bar(pct_frac, -1.0f, 12.0f, dps_top, dps_bot, pct_label.c_str());
						}
					}
					else
					{
						for (size_t p_idx = 0; p_idx < party.size(); p_idx++)
						{
							const auto& p = party[p_idx];
							const char* rank_icon = (p_idx == 0) ? ICON_FA_TROPHY : (p_idx == 1) ? ICON_FA_MEDAL : (p_idx == 2) ? ICON_FA_AWARD : ICON_FA_SHIELD_ALT;

							ImGui::TextColored(p.is_local ? ImVec4(0.3f, 1.0f, 0.5f, 1.0f) : (p_idx == 0 ? ImVec4(1.0f, 0.85f, 0.35f, 1.0f) : ImVec4(0.92f, 0.90f, 0.85f, 1.0f)),
								"%s %s", rank_icon, p.name.c_str());

							std::string stat_str = std::format("{} ({:.0f}/s)", p.total_damage, p.dps);
							float stat_width = ImGui::CalcTextSize(stat_str.c_str()).x;
							ImGui::SameLine(ImGui::GetWindowWidth() - stat_width - 16.0f);
							ImGui::TextColored(ImVec4(0.95f, 0.82f, 0.44f, 1.0f), "%s", stat_str.c_str());

							float pct_frac = std::clamp(p.damage_percent / 100.0f, 0.0f, 1.0f);
							std::string pct_label = std::format("{:.1f}% [" ICON_FA_SKULL " {}]", p.damage_percent, p.cart_count);

							ImU32 dps_top, dps_bot;
							if (p.is_local)
							{
								dps_top = mhw_gui::DPS_SELF_TOP;
								dps_bot = mhw_gui::DPS_SELF_BOT;
							}
							else if (p_idx == 0)
							{
								dps_top = mhw_gui::DPS_CHAMP_TOP;
								dps_bot = mhw_gui::DPS_CHAMP_BOT;
							}
							else
							{
								dps_top = mhw_gui::DPS_TEAM_TOP;
								dps_bot = mhw_gui::DPS_TEAM_BOT;
							}

							mhw_gui::draw_gauge_bar(pct_frac, -1.0f, 12.0f, dps_top, dps_bot, pct_label.c_str());
						}
					}

					mhw_gui::draw_ornate_divider(4.0f);
					float total_dps = damage_service::get_total_party_dps();
					ImGui::TextColored(ImVec4(0.70f, 0.75f, 0.80f, 1.0f), "Team Total: %u dmg | Avg DPS: %.1f/s", total_team_dmg, total_dps);
				}
				ImGui::End();
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

				if (ImGui::Begin("Dropped Materials ESP###DroppedMaterialsESP", nullptr, unfocused_flags))
				{
					mhw_gui::draw_corner_brackets(ImGui::GetWindowPos(), ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowWidth(), ImGui::GetWindowPos().y + ImGui::GetWindowHeight()));

					int total_drops = drop_service::get_total_active_drops();
					float closest_dist = drop_service::get_closest_drop_distance();

					ImGui::TextColored(ImVec4(0.95f, 0.82f, 0.44f, 1.0f), ICON_FA_DIAMOND " SCOUTFLY: SHINY DROPS " ICON_FA_DIAMOND);
					ImGui::SameLine();
					ImGui::TextColored(ImVec4(0.85f, 0.75f, 0.55f, 1.0f), "[Count: %d]", total_drops > 0 ? total_drops : 2);
					mhw_gui::draw_ornate_divider();

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
								ICON_FA_DIAMOND " %s", d.name);

							std::string dist_str = std::format("{:.1f}m [{}]", d.dist, d.dir);
							float dist_width = ImGui::CalcTextSize(dist_str.c_str()).x;
							ImGui::SameLine(ImGui::GetWindowWidth() - dist_width - 16.0f);
							ImGui::TextColored(ImVec4(0.35f, 0.85f, 1.0f, 1.0f), "%s", dist_str.c_str());

							float frac = d.time / 38.0f;
							std::string time_str = std::format("Despawn in {:.1f}s", d.time);

							ImU32 col_top = (d.time > 15.0f) ? mhw_gui::PART_TOP : mhw_gui::HP_CRIT_TOP;
							ImU32 col_bot = (d.time > 15.0f) ? mhw_gui::PART_BOT : mhw_gui::HP_CRIT_BOT;
							mhw_gui::draw_gauge_bar(frac, -1.0f, 9.0f, col_top, col_bot, time_str.c_str(), mhw_gui::COLOR_TEXT_IVORY, false);
						}
					}
					else
					{
						for (const auto& d : drops)
						{
							ImGui::TextColored(d.is_guiding_lands ? ImVec4(1.0f, 0.84f, 0.0f, 1.0f) : ImVec4(0.9f, 0.95f, 1.0f, 1.0f),
								ICON_FA_DIAMOND " %s", d.name.c_str());

							std::string dist_str = std::format("{:.1f}m [{}]", d.distance, d.direction.c_str());
							float dist_width = ImGui::CalcTextSize(dist_str.c_str()).x;
							ImGui::SameLine(ImGui::GetWindowWidth() - dist_width - 16.0f);
							ImGui::TextColored(ImVec4(0.35f, 0.85f, 1.0f, 1.0f), "%s", dist_str.c_str());

							float frac = std::clamp(d.time_remaining / d.max_time, 0.0f, 1.0f);
							std::string time_str = std::format("Despawn in {:.1f}s", d.time_remaining);

							ImU32 col_top, col_bot;
							if (d.time_remaining > 20.0f)
							{
								col_top = mhw_gui::PART_TOP;
								col_bot = mhw_gui::PART_BOT;
							}
							else if (d.time_remaining > 10.0f)
							{
								col_top = mhw_gui::HP_WARN_TOP;
								col_bot = mhw_gui::HP_WARN_BOT;
							}
							else
							{
								col_top = mhw_gui::HP_CRIT_TOP;
								col_bot = mhw_gui::HP_CRIT_BOT;
							}

							mhw_gui::draw_gauge_bar(frac, -1.0f, 9.0f, col_top, col_bot, time_str.c_str(), mhw_gui::COLOR_TEXT_IVORY, false);
						}
					}

					if (closest_dist > 0.0f && closest_dist < 9990.0f)
					{
						mhw_gui::draw_ornate_divider(4.0f);
						ImGui::TextColored(ImVec4(0.85f, 0.80f, 0.65f, 1.0f), "Nearest Shiny: %.1f m", closest_dist);
					}
				}
				ImGui::End();
			}
		}

		// ==========================================
		// 5. 3D LIGHT PILLARS FOR DROPPED ITEMS
		// ==========================================
		if (g_settings.window.light_pillars_shinies)
		{
			auto* bg_draw = ImGui::GetBackgroundDrawList();
			auto drops = drop_service::get_active_drops();
			Vector3 player_pos = character_service::get_position();
			Vector3 fwd_pos = character_service::get_forward_pos();

			float fx = fwd_pos.x - player_pos.x;
			float fz = fwd_pos.z - player_pos.z;
			float flen = std::sqrt(fx * fx + fz * fz);
			if (flen > 0.001f)
			{
				fx /= flen;
				fz /= flen;
			}
			else
			{
				fx = 0.0f;
				fz = 1.0f;
			}

			float rx = -fz;
			float rz = fx;

			float screen_w = viewport->WorkSize.x;
			float screen_h = viewport->WorkSize.y;
			float fov_scale = screen_w * 0.85f;

			auto now_time = static_cast<float>(ImGui::GetTime());

			if (drops.empty() && is_menu_open)
			{
				float sx = viewport->WorkPos.x + screen_w * 0.35f;
				float sy = viewport->WorkPos.y + screen_h * 0.65f;
				float beam_h = 240.0f;

				ImU32 col_core = 0xFFFFFFFF;
				ImU32 col_glow = 0xE000D7FF; // Vibrant Sacred Gold
				ImU32 col_top = 0x0000D7FF;

				float pulse = 8.0f + 3.0f * std::sin(now_time * 4.0f);
				bg_draw->AddCircle(ImVec2(sx, sy), pulse, col_glow, 24, 2.0f);
				bg_draw->AddCircle(ImVec2(sx, sy), pulse * 1.5f, (col_glow & 0x00FFFFFF) | 0x50000000, 24, 1.0f);
				bg_draw->AddCircleFilled(ImVec2(sx, sy), 5.0f, col_core, 16);

				bg_draw->AddRectFilledMultiColor(
					ImVec2(sx - 14.0f, sy - beam_h), ImVec2(sx + 14.0f, sy),
					col_top, col_top, col_glow, col_glow
				);

				bg_draw->AddRectFilledMultiColor(
					ImVec2(sx - 3.0f, sy - beam_h), ImVec2(sx + 3.0f, sy),
					0x40FFFFFF, 0x40FFFFFF, col_core, col_core
				);

				std::string preview_tag = ICON_FA_GEM " [Preview] Guiding Dragonbone (14.2m | 28.5s)";
				ImVec2 tag_sz = ImGui::CalcTextSize(preview_tag.c_str());
				ImVec2 tag_pos(sx - tag_sz.x * 0.5f, sy - beam_h - 22.0f);

				bg_draw->AddRectFilled(ImVec2(tag_pos.x - 6.0f, tag_pos.y - 3.0f), ImVec2(tag_pos.x + tag_sz.x + 6.0f, tag_pos.y + tag_sz.y + 3.0f), mhw_gui::COLOR_BG_DARK, 0.0f);
				bg_draw->AddRect(ImVec2(tag_pos.x - 6.0f, tag_pos.y - 3.0f), ImVec2(tag_pos.x + tag_sz.x + 6.0f, tag_pos.y + tag_sz.y + 3.0f), mhw_gui::COLOR_BORDER_BRONZE, 0.0f, 0, 1.0f);
				bg_draw->AddText(tag_pos, 0xFFFFE066, preview_tag.c_str());
			}
			else
			{
				for (const auto& d : drops)
				{
					Vector2 screen_bot;
					if (!camera_service::world_to_screen(d.position, screen_bot))
						continue;

					if (screen_bot.x < -100.0f || screen_bot.x > screen_w + 100.0f ||
					    screen_bot.y < -100.0f || screen_bot.y > screen_h + 100.0f)
						continue;

					Vector2 screen_top;
					float beam_h = 240.0f;
					if (camera_service::world_to_screen(d.position + Vector3(0.0f, 350.0f, 0.0f), screen_top) && screen_top.y < screen_bot.y)
					{
						beam_h = std::clamp(screen_bot.y - screen_top.y, 40.0f, screen_h * 0.85f);
					}

					float sx = screen_bot.x;
					float sy = screen_bot.y;
					float beam_half_w = std::clamp(beam_h * 0.055f, 4.0f, 22.0f);

					float dist = d.distance;
					if (dist <= 0.01f)
					{
						float dx = d.position.x - player_pos.x;
						float dy = d.position.y - player_pos.y;
						float dz = d.position.z - player_pos.z;
						dist = std::sqrt(dx * dx + dy * dy + dz * dz) * 0.03048f;
					}

					ImU32 col_core = 0xFFFFFFFF;
					ImU32 col_glow;
					ImU32 col_top;

					if (d.is_guiding_lands)
					{
						col_glow = 0xE000D7FF; // Sacred Gold
						col_top = 0x0000D7FF;
					}
					else if (d.name.find("Slinger") != std::string::npos || d.name.find("Ammo") != std::string::npos)
					{
						col_glow = 0xE0FFE500; // Scoutfly Turquoise Cyan
						col_top = 0x00FFE500;
					}
					else
					{
						col_glow = 0xE0FFB0E0; // Diamond Light Purple
						col_top = 0x00FFB0E0;
					}

					float pulse = 6.0f + 3.0f * std::sin(now_time * 5.0f + static_cast<float>(d.id));
					bg_draw->AddCircle(ImVec2(sx, sy), pulse, col_glow, 24, 2.0f);
					bg_draw->AddCircle(ImVec2(sx, sy), pulse * 1.5f, (col_glow & 0x00FFFFFF) | 0x50000000, 24, 1.0f);
					bg_draw->AddCircleFilled(ImVec2(sx, sy), 4.0f, col_core, 16);

					bg_draw->AddRectFilledMultiColor(
						ImVec2(sx - beam_half_w, sy - beam_h), ImVec2(sx + beam_half_w, sy),
						col_top, col_top, col_glow, col_glow
					);

					bg_draw->AddRectFilledMultiColor(
						ImVec2(sx - 2.5f, sy - beam_h), ImVec2(sx + 2.5f, sy),
						0x30FFFFFF, 0x30FFFFFF, col_core, col_core
					);

					std::string drop_tag = std::format(ICON_FA_GEM " {} ({:.1f}m | {:.0f}s)", d.name, dist, d.time_remaining);
					ImVec2 tag_sz = ImGui::CalcTextSize(drop_tag.c_str());
					ImVec2 tag_pos(sx - tag_sz.x * 0.5f, sy - beam_h - 22.0f);

					bg_draw->AddRectFilled(
						ImVec2(tag_pos.x - 6.0f, tag_pos.y - 3.0f),
						ImVec2(tag_pos.x + tag_sz.x + 6.0f, tag_pos.y + tag_sz.y + 3.0f),
						mhw_gui::COLOR_BG_DARK, 0.0f
					);
					bg_draw->AddRect(
						ImVec2(tag_pos.x - 6.0f, tag_pos.y - 3.0f),
						ImVec2(tag_pos.x + tag_sz.x + 6.0f, tag_pos.y + tag_sz.y + 3.0f),
						mhw_gui::COLOR_BORDER_BRONZE, 0.0f, 0, 1.0f
					);
					bg_draw->AddText(tag_pos, col_core, drop_tag.c_str());
				}
			}
		}

		// ==========================================
		// 6. HUNTER BUFF & MANTLE TIMERS OVERLAY
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

				if (ImGui::Begin("Hunter Buff & Mantle Timers###HunterBuffTimers", nullptr, unfocused_flags))
				{
					mhw_gui::draw_corner_brackets(ImGui::GetWindowPos(), ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowWidth(), ImGui::GetWindowPos().y + ImGui::GetWindowHeight()));

					ImGui::TextColored(ImVec4(0.95f, 0.82f, 0.44f, 1.0f), ICON_FA_DIAMOND " SPECIALIZED TOOLS & BUFFS " ICON_FA_DIAMOND);
					mhw_gui::draw_ornate_divider();

					// Mantle Timers
					ImGui::TextColored(ImVec4(0.84f, 0.75f, 0.58f, 1.0f), "Specialized Tools (Mantles):");

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
								std::string label = std::format(ICON_FA_SHIELD_ALT " {}: [ACTIVE] {:.1f}s", m.name, m.dur);
								mhw_gui::draw_gauge_bar(frac, -1.0f, 13.0f, mhw_gui::MANTLE_ACT_TOP, mhw_gui::MANTLE_ACT_BOT, label.c_str());
							}
							else if (m.on_cd)
							{
								float frac = m.cd / m.max_cd;
								std::string label = std::format(ICON_FA_CLOCK " {}: [COOLDOWN] {:.1f}s", m.name, m.cd);
								mhw_gui::draw_gauge_bar(frac, -1.0f, 13.0f, mhw_gui::MANTLE_CD_TOP, mhw_gui::MANTLE_CD_BOT, label.c_str());
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
								std::string label = std::format(ICON_FA_SHIELD_ALT " {}: [ACTIVE] {:.1f}s", m.name, m.duration);
								mhw_gui::draw_gauge_bar(frac, -1.0f, 13.0f, mhw_gui::MANTLE_ACT_TOP, mhw_gui::MANTLE_ACT_BOT, label.c_str());
							}
							else if (m.is_on_cooldown)
							{
								float frac = (m.max_cooldown > 0.0f) ? std::clamp(m.cooldown / m.max_cooldown, 0.0f, 1.0f) : 1.0f;
								std::string label = std::format(ICON_FA_CLOCK " {}: [COOLDOWN] {:.1f}s", m.name, m.cooldown);
								mhw_gui::draw_gauge_bar(frac, -1.0f, 13.0f, mhw_gui::MANTLE_CD_TOP, mhw_gui::MANTLE_CD_BOT, label.c_str());
							}
							else
							{
								std::string label = std::format(ICON_FA_SHIELD_ALT " {}: " ICON_FA_CHECK " READY", m.name);
								mhw_gui::draw_gauge_bar(1.0f, -1.0f, 13.0f, mhw_gui::MANTLE_RDY_TOP, mhw_gui::MANTLE_RDY_BOT, label.c_str());
							}
						}
					}

					// Hunter Active Buff Timers
					ImGui::Spacing();
					ImGui::TextColored(ImVec4(0.84f, 0.75f, 0.58f, 1.0f), "Consumable & Hunting Buffs:");

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
							float frac = b.perm ? 1.0f : (b.time / b.max_t);
							std::string label = b.perm ? std::format(ICON_FA_SWORDS " {}: ACTIVE", b.name) : std::format(ICON_FA_SWORD " {}: {:.0f}s", b.name, b.time);

							ImU32 col_top = b.col;
							ImU32 col_bot = (b.col & 0x00FFFFFF) | 0x90000000;
							mhw_gui::draw_gauge_bar(frac, -1.0f, 11.0f, col_top, col_bot, label.c_str());
						}
					}
					else
					{
						for (const auto& b : buffs)
						{
							float frac = b.is_permanent ? 1.0f : (b.max_timer > 0.0f ? std::clamp(b.timer / b.max_timer, 0.0f, 1.0f) : 1.0f);
							std::string label = b.is_permanent
								? std::format(ICON_FA_SWORDS " {}: ACTIVE", b.name)
								: std::format(ICON_FA_SWORD " {}: {:.0f}s", b.name, b.timer);

							ImU32 col_top = b.color;
							ImU32 col_bot = (b.color & 0x00FFFFFF) | 0x90000000;
							mhw_gui::draw_gauge_bar(frac, -1.0f, 11.0f, col_top, col_bot, label.c_str());
						}
					}
				}
				ImGui::End();
			}
		}

		// ==========================================
		// 7. TEAM PLAYER ESP & OFF-SCREEN COMPASS
		// ==========================================
		if (g_settings.window.overlay_team_esp)
		{
			auto teammates = party_service::get_active_teammates();
			if (!teammates.empty())
			{
				Vector3 player_pos = character_service::get_position();
				Vector3 cam_pos = camera_service::get_camera_pos();
				Vector3 cam_fwd = camera_service::get_camera_forward();
				Vector3 cam_right = camera_service::get_camera_right();

				float screen_w = viewport->WorkSize.x;
				float screen_h = viewport->WorkSize.y;
				auto* bg_draw = ImGui::GetBackgroundDrawList();
				float now_time = static_cast<float>(ImGui::GetTime());

				for (const auto& t : teammates)
				{
					Vector3 ppos = (cam_pos.x != 0.0f || cam_pos.z != 0.0f) ? cam_pos : player_pos;
					float dx = t.position.x - ppos.x;
					float dy = t.position.y - ppos.y;
					float dz = t.position.z - ppos.z;
					float dist = t.distance > 0.01f ? t.distance : (std::sqrt(dx * dx + dy * dy + dz * dz) * 0.03048f);

					float dot_f = dx * cam_fwd.x + dy * cam_fwd.y + dz * cam_fwd.z;
					float dot_r = dx * cam_right.x + dy * cam_right.y + dz * cam_right.z;

					Vector2 screen_feet, screen_head;
					bool in_front = camera_service::world_to_screen(t.position, screen_feet) &&
					                camera_service::world_to_screen(t.position + Vector3(0.0f, 180.0f, 0.0f), screen_head);

					bool is_on_screen = false;
					if (in_front)
					{
						if (screen_feet.x >= 110.0f && screen_feet.x <= screen_w - 110.0f &&
						    screen_head.y >= 70.0f && screen_feet.y <= screen_h - 40.0f)
						{
							is_on_screen = true;
						}
					}

					if (is_on_screen)
					{
						float sx = screen_head.x;
						float sy = screen_head.y;

						// 1. In-world 3D locator beacon at feet
						float pulse = 7.0f + 2.5f * std::sin(now_time * 4.0f + static_cast<float>(t.player_id));
						bg_draw->AddCircle(ImVec2(screen_feet.x, screen_feet.y), pulse, 0xE040D0F8, 24, 2.0f);
						bg_draw->AddCircleFilled(ImVec2(screen_feet.x, screen_feet.y), 3.5f, 0xFFFFFFFF, 16);
						bg_draw->AddLine(ImVec2(screen_feet.x, screen_feet.y), ImVec2(sx, sy), 0x809C7A48, 1.0f);

						// 2. Floating MHW Nameplate box
						ImVec2 b_min(sx - 105.0f, sy - 58.0f);
						ImVec2 b_max(sx + 105.0f, sy - 4.0f);

						bg_draw->AddRectFilled(b_min, b_max, mhw_gui::COLOR_BG_DARK, 0.0f);
						bg_draw->AddRect(b_min, b_max, mhw_gui::COLOR_BORDER_BRONZE, 0.0f, 0, 1.2f);
						mhw_gui::draw_corner_brackets(b_min, b_max, mhw_gui::COLOR_CORNER_GOLD, 6.0f);

						// Title & Distance
						const char* w_icon = party_service::get_weapon_icon(t.weapon_id);
						std::string title = std::format("{} [P{}] {}", w_icon, t.player_id + 1, t.name);
						bg_draw->AddText(ImVec2(b_min.x + 8.0f, b_min.y + 4.0f), mhw_gui::COLOR_TEXT_GOLD, title.c_str());

						std::string dist_str = std::format("{:.1f}m", dist);
						ImVec2 dist_sz = ImGui::CalcTextSize(dist_str.c_str());
						bg_draw->AddText(ImVec2(b_max.x - dist_sz.x - 8.0f, b_min.y + 4.0f), mhw_gui::COLOR_TEXT_IVORY, dist_str.c_str());

						// HP Gauge Bar
						float hp_ratio = (t.max_health > 0.0f) ? std::clamp(t.health / t.max_health, 0.0f, 1.0f) : 1.0f;
						ImVec2 bar_min(b_min.x + 8.0f, b_min.y + 22.0f);
						ImVec2 bar_max(b_max.x - 8.0f, b_min.y + 35.0f);

						bg_draw->AddRectFilled(bar_min, bar_max, mhw_gui::COLOR_BG_CHANNEL, 0.0f);
						float bar_fill_w = (bar_max.x - bar_min.x) * hp_ratio;
						ImU32 hp_top = (hp_ratio > 0.5f) ? mhw_gui::HP_GREEN_TOP : (hp_ratio > 0.25f) ? mhw_gui::HP_WARN_TOP : mhw_gui::HP_CRIT_TOP;
						ImU32 hp_bot = (hp_ratio > 0.5f) ? mhw_gui::HP_GREEN_BOT : (hp_ratio > 0.25f) ? mhw_gui::HP_WARN_BOT : mhw_gui::HP_CRIT_BOT;
						bg_draw->AddRectFilledMultiColor(bar_min, ImVec2(bar_min.x + bar_fill_w, bar_max.y), hp_top, hp_top, hp_bot, hp_bot);
						bg_draw->AddRect(bar_min, bar_max, mhw_gui::COLOR_BORDER_BRONZE, 0.0f, 0, 1.0f);

						std::string hp_str = std::format("HP {:.0f} / {:.0f}", t.health, t.max_health);
						ImVec2 hp_sz = ImGui::CalcTextSize(hp_str.c_str());
						bg_draw->AddText(ImVec2(bar_min.x + (bar_max.x - bar_min.x - hp_sz.x) * 0.5f, bar_min.y - 0.5f), 0xFFFFFFFF, hp_str.c_str());

						// Footer Weapon & Teleport hint
						bg_draw->AddText(ImVec2(b_min.x + 8.0f, b_min.y + 38.0f), mhw_gui::COLOR_TEXT_MUTED, t.weapon.c_str());
						bg_draw->AddText(ImVec2(b_max.x - 70.0f, b_min.y + 38.0f), mhw_gui::COLOR_CORNER_GOLD, "[Click / F9]");

						// Click to Teleport
						if (is_menu_open && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
						{
							ImVec2 mouse_pos = ImGui::GetMousePos();
							if (mouse_pos.x >= b_min.x && mouse_pos.x <= b_max.x && mouse_pos.y >= b_min.y && mouse_pos.y <= b_max.y)
							{
								party_service::teleport_to_teammate(t);
							}
						}
					}
					else
					{
						// Off-Screen Compass Indicator
						float cx = viewport->WorkPos.x + screen_w * 0.5f;
						float cy = viewport->WorkPos.y + screen_h * 0.5f;

						float dir_x = 0.0f;
						float dir_y = 0.0f;
						if (in_front)
						{
							dir_x = screen_head.x - cx;
							dir_y = screen_head.y - cy;
						}
						else
						{
							dir_x = dot_r;
							dir_y = -dot_f;
						}

						float dlen = std::sqrt(dir_x * dir_x + dir_y * dir_y);
						if (dlen > 0.001f)
						{
							dir_x /= dlen;
							dir_y /= dlen;
						}
						else
						{
							dir_x = 0.0f;
							dir_y = 1.0f;
						}

						float bound_x = screen_w * 0.5f - 50.0f;
						float bound_y = screen_h * 0.5f - 50.0f;
						float scale_x = (dir_x != 0.0f) ? std::abs(bound_x / dir_x) : 999999.0f;
						float scale_y = (dir_y != 0.0f) ? std::abs(bound_y / dir_y) : 999999.0f;
						float scale = std::min(scale_x, scale_y);

						float ex = cx + dir_x * scale;
						float ey = cy + dir_y * scale;

						// Draw directional chevron triangle pointing towards teammate
						ImVec2 tip(ex, ey);
						ImVec2 base1(ex - dir_x * 16.0f + dir_y * 10.0f, ey - dir_y * 16.0f - dir_x * 10.0f);
						ImVec2 base2(ex - dir_x * 16.0f - dir_y * 10.0f, ey - dir_y * 16.0f + dir_x * 10.0f);
						bg_draw->AddTriangleFilled(tip, base1, base2, mhw_gui::COLOR_CORNER_GOLD);
						bg_draw->AddTriangle(tip, base1, base2, mhw_gui::COLOR_BORDER_BRONZE, 1.5f);

						// Draw compact compass badge
						const char* w_icon = party_service::get_weapon_icon(t.weapon_id);
						std::string badge_str = std::format("{} [P{}] {} ({:.0f}m)", w_icon, t.player_id + 1, t.name, dist);
						ImVec2 bsz = ImGui::CalcTextSize(badge_str.c_str());
						ImVec2 bpos(ex - dir_x * 48.0f - bsz.x * 0.5f, ey - dir_y * 48.0f - bsz.y * 0.5f);

						bg_draw->AddRectFilled(ImVec2(bpos.x - 6.0f, bpos.y - 3.0f), ImVec2(bpos.x + bsz.x + 6.0f, bpos.y + bsz.y + 3.0f), mhw_gui::COLOR_BG_DARK, 0.0f);
						bg_draw->AddRect(ImVec2(bpos.x - 6.0f, bpos.y - 3.0f), ImVec2(bpos.x + bsz.x + 6.0f, bpos.y + bsz.y + 3.0f), mhw_gui::COLOR_BORDER_BRONZE, 0.0f, 0, 1.0f);
						bg_draw->AddText(bpos, mhw_gui::COLOR_TEXT_GOLD, badge_str.c_str());
					}
				}
			}
		}

		// Pop MHW Global HUD Style
		ImGui::PopStyleColor(2);
		ImGui::PopStyleVar(4);
	}
}
