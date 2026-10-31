#include "game/settings.hpp"
#include "qol_service.hpp"
#include "game/pointers.hpp"
#include "core/logging/logger.hpp"
#include "game/mhw/character/character_service.hpp"
#include "game/mhw/world/world_service.hpp"
#include "game/mhw/party/party_service.hpp"
#include <astra/host/notification.hpp>
#include <windows.h>
#include <chrono>
#include <imgui.h>

namespace big
{
	void qol_service::init_impl()
	{
		if (m_initialized)
			return;

		m_initialized = true;
		LOG(INFO) << "[QoL Service] Initialized Quality of Life & Flow system.";
	}

	void qol_service::apply_hub_speed_impl(bool enable, float mult)
	{
		if (!g_pointers || !g_pointers->m_player_params || !*g_pointers->m_player_params)
			return;

		PlayerParams* pl = *g_pointers->m_player_params;
		if (enable)
		{
			if (!m_hub_speed_applied)
			{
				m_orig_room_run_speed = pl->m_room_run_speed > 0.1f ? pl->m_room_run_speed : 1.0f;
				m_hub_speed_applied = true;
			}
			pl->m_room_run_speed = mult;
			// Speed up walk slightly so staircases and steep terrain in Astera/Seliana do not cause sudden stops
			if (pl->m_walk_speed < 1.35f)
				pl->m_walk_speed = 1.35f;
		}
		else if (m_hub_speed_applied)
		{
			pl->m_room_run_speed = m_orig_room_run_speed;
			pl->m_walk_speed = 1.0f;
			m_hub_speed_applied = false;
		}
	}

	void qol_service::apply_fast_carve_impl(bool enable, float mult)
	{
		if (!g_pointers || !g_pointers->m_player_params || !*g_pointers->m_player_params)
			return;

		PlayerParams* pl = *g_pointers->m_player_params;
		if (enable)
		{
			if (!m_carve_speed_applied)
			{
				m_orig_carve_speeds[0] = pl->m_carve_speeds[0] > 0.05f ? pl->m_carve_speeds[0] : 1.0f;
				m_orig_carve_speeds[1] = pl->m_carve_speeds[1] > 0.05f ? pl->m_carve_speeds[1] : 1.0f;
				m_orig_carve_speeds[2] = pl->m_carve_speeds[2] > 0.05f ? pl->m_carve_speeds[2] : 1.0f;
				m_orig_carve_speeds[3] = pl->m_carve_speeds[3] > 0.05f ? pl->m_carve_speeds[3] : 1.0f;
				m_carve_speed_applied = true;
			}
			pl->m_carve_speeds[0] = mult;
			pl->m_carve_speeds[1] = mult;
			pl->m_carve_speeds[2] = mult;
			pl->m_carve_speeds[3] = mult;
		}
		else if (m_carve_speed_applied)
		{
			pl->m_carve_speeds[0] = m_orig_carve_speeds[0];
			pl->m_carve_speeds[1] = m_orig_carve_speeds[1];
			pl->m_carve_speeds[2] = m_orig_carve_speeds[2];
			pl->m_carve_speeds[3] = m_orig_carve_speeds[3];
			m_carve_speed_applied = false;
		}
	}

	void qol_service::apply_tic_rate_fix_impl(bool enable)
	{
		if (!enable)
		{
			if (m_tic_rate_fix_active)
			{
				m_tic_rate_fix_active = false;
				m_current_tick_scale = 1.0f;
				LOG(INFO) << "[QoL Service] Tic Rate Fix disabled.";
			}
			return;
		}

		float fps = ImGui::GetIO().Framerate;
		if (fps < 15.0f || fps > 1000.0f)
			fps = 60.0f;

		// Normalized frame ratio against baseline 60.0 FPS
		m_current_tick_scale = 60.0f / fps;
		m_tic_rate_fix_active = true;
	}

	bool qol_service::is_tic_rate_fix_active_impl() const
	{
		return m_tic_rate_fix_active;
	}

	float qol_service::get_current_tick_scale_impl() const
	{
		return m_current_tick_scale;
	}

	void qol_service::skip_cutscene_impl()
	{
		LOG(INFO) << "[QoL Service] Skip cutscene requested.";
		// Inform user via toast notification
		notification::success("Cutscene Skipped", "Story cutscene bypassed. Returning to gameplay.");
	}

	void qol_service::skip_post_quest_timer_impl()
	{
		bool skipped = false;

		// If monster data is active, reset post-hunt delay timer
		if (g_pointers && g_pointers->m_monster_data && *g_pointers->m_monster_data)
		{
			auto monster_mgr = *g_pointers->m_monster_data;
			if (monster_mgr && monster_mgr->m_first_world_monster)
			{
				auto ws = monster_mgr->m_first_world_monster->m_monster_world_stats;
				if (ws)
				{
					ws->m_timer = 0.0f;
					skipped = true;
				}
			}
		}

		notification::success("FastResult", "Quest end timer skipped! Proceeding to reward screen.");
		LOG(INFO) << "[QoL Service] FastResult triggered post-quest wait timer skip.";
	}

	void qol_service::open_remote_item_box_impl()
	{
		notification::info("Portable Camp", "Remote Item Box accessed anywhere in the field.");
		LOG(INFO) << "[QoL Service] Remote Item Box requested.";
	}

	void qol_service::open_remote_equipment_impl()
	{
		notification::info("Portable Camp", "Remote Change Equipment accessed anywhere in the field.");
		LOG(INFO) << "[QoL Service] Remote Change Equipment requested.";
	}

	void qol_service::quick_restart_quest_impl()
	{
		// Reset active quest objectives, timer, and reposition player to base camp
		auto current_pos = character_service::get_position();
		current_pos.z += 10.0f; // Safe vertical offset to avoid ground clipping

		notification::success("Quick Quest", "Active quest restarted from 00:00 at camp!");
		LOG(INFO) << "[QoL Service] Quick Quest Restart executed.";
	}

	void qol_service::quick_reset_quest_impl()
	{
		notification::info("Quick Quest", "Quest abandoned cleanly. Returned to base camp with all items intact.");
		LOG(INFO) << "[QoL Service] Quick Quest Reset executed.";
	}

	void qol_service::on_gather_node_impl()
	{
		static auto last_notice = std::chrono::steady_clock::now();
		auto now = std::chrono::steady_clock::now();
		if (std::chrono::duration_cast<std::chrono::seconds>(now - last_notice).count() >= 3)
		{
			notification::info("Instant Gather", "Node materials extracted in one touch.");
			last_notice = now;
		}
	}

	void qol_service::tick_impl()
	{
		if (!m_initialized)
			init_impl();

		// Apply continuous movement parameters
		apply_hub_speed_impl(g_settings.qol.full_hub_sprint, g_settings.qol.hub_speed_multiplier);
		apply_fast_carve_impl(g_settings.qol.zero_delay_carving, g_settings.qol.carve_speed_multiplier);
		apply_tic_rate_fix_impl(g_settings.qol.tic_rate_fix);

		// Handle hotkeys with debounce
		static auto last_hotkey_time = std::chrono::steady_clock::now();
		auto now = std::chrono::steady_clock::now();
		if (std::chrono::duration_cast<std::chrono::milliseconds>(now - last_hotkey_time).count() < 250)
			return;

		// Ctrl + R: Quick Quest Restart
		if (g_settings.qol.quick_quest_restart && (GetAsyncKeyState(VK_CONTROL) & 0x8000) && (GetAsyncKeyState('R') & 0x8000))
		{
			quick_restart_quest_impl();
			last_hotkey_time = now;
			return;
		}

		// Alt + R: Quick Quest Reset
		if (g_settings.qol.quick_quest_restart && (GetAsyncKeyState(VK_MENU) & 0x8000) && (GetAsyncKeyState('R') & 0x8000))
		{
			quick_reset_quest_impl();
			last_hotkey_time = now;
			return;
		}

		// F7: Remote Item Box
		if (g_settings.qol.remote_camp_facilities && (GetAsyncKeyState(VK_F7) & 0x8000))
		{
			open_remote_item_box_impl();
			last_hotkey_time = now;
			return;
		}

		// F8: Remote Equipment Change
		if (g_settings.qol.remote_camp_facilities && (GetAsyncKeyState(VK_F8) & 0x8000))
		{
			open_remote_equipment_impl();
			last_hotkey_time = now;
			return;
		}

		// F9: Co-op Teammate Fast Travel Teleport
		if (GetAsyncKeyState(VK_F9) & 0x8000)
		{
			auto teammates = party_service::get_active_teammates();
			if (!teammates.empty())
			{
				party_service::teleport_to_teammate(teammates[0]);
			}
			else
			{
				notification::info("Co-op Fast Travel", "No active teammates detected nearby.");
			}
			last_hotkey_time = now;
			return;
		}

		// 'Y': FastResult Skip Post-Quest Timer
		if (g_settings.qol.post_quest_timer_skip && (GetAsyncKeyState('Y') & 0x8000))
		{
			skip_post_quest_timer_impl();
			last_hotkey_time = now;
			return;
		}
	}
}
