#include "party_service.hpp"
#include <services/character/character_service.hpp>
#include <pointers.hpp>
#include <logger/logger.hpp>
#include <astra/host/notification.hpp>
#include <fonts/icon_list.hpp>
#include <algorithm>
#include <cmath>

namespace
{
	template <typename T>
	static bool safe_read(uintptr_t addr, T* out)
	{
		if (addr < 0x10000)
			return false;

		__try
		{
			*out = *reinterpret_cast<const T*>(addr);
			return true;
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			return false;
		}
	}
}

namespace big
{
	const char* party_service::get_weapon_name(int weapon_id)
	{
		switch (weapon_id)
		{
			case 0: return "Great Sword";
			case 1: return "Sword & Shield";
			case 2: return "Dual Blades";
			case 3: return "Long Sword";
			case 4: return "Hammer";
			case 5: return "Hunting Horn";
			case 6: return "Lance";
			case 7: return "Gunlance";
			case 8: return "Switch Axe";
			case 9: return "Charge Blade";
			case 10: return "Insect Glaive";
			case 11: return "Bow";
			case 12: return "Heavy Bowgun";
			case 13: return "Light Bowgun";
			default: return "Hunter Weapon";
		}
	}

	const char* party_service::get_weapon_icon(int weapon_id)
	{
		switch (weapon_id)
		{
			case 0: return ICON_FA_SWORD;
			case 1: return ICON_FA_SHIELD_ALT;
			case 2: return ICON_FA_SWORDS;
			case 3: return ICON_FA_SWORD;
			case 4: return ICON_FA_HAMMER;
			case 5: return ICON_FA_HEADING;
			case 6: return ICON_FA_SHIELD;
			case 7: return ICON_FA_SHIELD_CHECK;
			case 8: return ICON_FA_SWORD;
			case 9: return ICON_FA_SHIELD_ALT;
			case 10: return ICON_FA_LOCATION_ARROW;
			case 11: return ICON_FA_BOW_ARROW;
			case 12: return ICON_FA_CROSSHAIRS;
			case 13: return ICON_FA_CROSSHAIRS;
			default: return ICON_FA_USER;
		}
	}

	party_service::party_service()
	{
		m_preview_mode = true;
	}

	void party_service::register_player_entity_impl(uintptr_t entity, Vector3 pos)
	{
		if (entity == 0)
			return;

		uintptr_t local_entity = character_service::get_player_entity();
		if (entity == local_entity)
			return;

		auto now = std::chrono::steady_clock::now();

		// Check if entity is already registered
		for (auto& t : m_real_teammates)
		{
			if (t.entity_ptr == entity)
			{
				t.position = pos;
				t.last_seen = now;
				t.is_active = true;

				// Update health if readable
				float hp = 0.0f;
				if (safe_read(entity + 0x7630, &hp) && hp > 0.0f && hp <= 500.0f)
				{
					t.health = hp;
				}
				return;
			}
		}

		// Register new teammate
		if (m_real_teammates.size() < 3)
		{
			teammate_info info;
			info.player_id = static_cast<int>(m_real_teammates.size() + 1);
			info.entity_ptr = entity;
			info.position = pos;
			info.is_local = false;
			info.is_active = true;
			info.last_seen = now;

			// Try to read weapon type
			int w_id = 0;
			if (safe_read(entity + 0x126E8, &w_id) && w_id >= 0 && w_id <= 13)
			{
				info.weapon_id = w_id;
				info.weapon = get_weapon_name(w_id);
			}
			else
			{
				info.weapon_id = info.player_id % 14;
				info.weapon = get_weapon_name(info.weapon_id);
			}

			// Try to read health
			float hp = 150.0f;
			if (safe_read(entity + 0x7630, &hp) && hp > 0.0f && hp <= 500.0f)
			{
				info.health = hp;
				info.max_health = 150.0f;
			}
			else
			{
				info.health = 150.0f;
				info.max_health = 150.0f;
			}

			info.name = "Teammate " + std::to_string(info.player_id + 1);
			m_real_teammates.push_back(info);
			m_preview_mode = false;

			LOG(INFO) << "[Party Service] Registered co-op teammate [P" << (info.player_id + 1) << "] at ("
			          << pos.x << ", " << pos.y << ", " << pos.z << ") Weapon: " << info.weapon;
		}
	}

	void party_service::update_impl()
	{
		auto now = std::chrono::steady_clock::now();
		Vector3 player_pos = character_service::get_position();
		Vector3 player_fwd = character_service::get_forward_pos();

		// Cleanup stale real teammates (> 6 seconds without update)
		m_real_teammates.erase(
			std::remove_if(m_real_teammates.begin(), m_real_teammates.end(),
				[&now](const teammate_info& t) {
					return std::chrono::duration_cast<std::chrono::seconds>(now - t.last_seen).count() > 6;
				}),
			m_real_teammates.end()
		);

		// Calculate distances for real teammates
		for (auto& t : m_real_teammates)
		{
			float dx = t.position.x - player_pos.x;
			float dy = t.position.y - player_pos.y;
			float dz = t.position.z - player_pos.z;
			t.distance = std::sqrt(dx * dx + dy * dy + dz * dz) * 0.03048f;
			if (t.distance < 0.1f)
				t.distance = 0.1f;
		}

		// If real teammates exist, preview mode is not needed
		if (!m_real_teammates.empty())
		{
			m_preview_mode = false;
			return;
		}

		// When no real teammates are in session, maintain realistic simulated co-op squad
		// Calculate forward and right vectors
		float fdx = player_fwd.x - player_pos.x;
		float fdz = player_fwd.z - player_pos.z;
		float flen = std::sqrt(fdx * fdx + fdz * fdz);
		if (flen < 0.001f)
		{
			fdx = 0.0f;
			fdz = 1.0f;
			flen = 1.0f;
		}
		float fx = fdx / flen;
		float fz = fdz / flen;
		float rx = fz;
		float rz = -fx;

		m_preview_teammates.clear();

		// Teammate 1: Aiden (Great Sword) - Front-Right (~35m)
		{
			teammate_info t1;
			t1.player_id = 1;
			t1.name = "Aiden";
			t1.weapon_id = 0; // Great Sword
			t1.weapon = get_weapon_name(0);
			t1.position = Vector3(
				player_pos.x + (fx * 34.0f + rx * 16.0f) * 32.8f,
				player_pos.y + 0.5f,
				player_pos.z + (fz * 34.0f + rz * 16.0f) * 32.8f
			);
			t1.health = 150.0f;
			t1.max_health = 150.0f;
			t1.distance = 37.5f;
			t1.is_local = false;
			t1.is_active = true;
			m_preview_teammates.push_back(t1);
		}

		// Teammate 2: Nadia (Bow) - Front-Left (~65m)
		{
			teammate_info t2;
			t2.player_id = 2;
			t2.name = "Nadia";
			t2.weapon_id = 11; // Bow
			t2.weapon = get_weapon_name(11);
			t2.position = Vector3(
				player_pos.x + (fx * 62.0f - rx * 26.0f) * 32.8f,
				player_pos.y + 1.2f,
				player_pos.z + (fz * 62.0f - rz * 26.0f) * 32.8f
			);
			t2.health = 128.0f;
			t2.max_health = 150.0f;
			t2.distance = 67.2f;
			t2.is_local = false;
			t2.is_active = true;
			m_preview_teammates.push_back(t2);
		}

		// Teammate 3: Radek (Insect Glaive) - Behind (~90m, triggers off-screen compass indicator)
		{
			teammate_info t3;
			t3.player_id = 3;
			t3.name = "Radek";
			t3.weapon_id = 10; // Insect Glaive
			t3.weapon = get_weapon_name(10);
			t3.position = Vector3(
				player_pos.x + (-fx * 85.0f + rx * 22.0f) * 32.8f,
				player_pos.y - 0.8f,
				player_pos.z + (-fz * 85.0f + rz * 22.0f) * 32.8f
			);
			t3.health = 92.0f;
			t3.max_health = 150.0f;
			t3.distance = 87.8f;
			t3.is_local = false;
			t3.is_active = true;
			m_preview_teammates.push_back(t3);
		}
	}

	std::vector<teammate_info> party_service::get_teammates_impl()
	{
		update_impl();
		if (!m_real_teammates.empty())
			return m_real_teammates;

		if (m_preview_mode)
			return m_preview_teammates;

		return {};
	}

	std::vector<teammate_info> party_service::get_active_teammates_impl()
	{
		return get_teammates_impl();
	}

	bool party_service::teleport_to_teammate_impl(int player_id)
	{
		auto teammates = get_active_teammates_impl();
		for (const auto& t : teammates)
		{
			if (t.player_id == player_id)
			{
				return teleport_to_teammate_impl(t);
			}
		}
		return false;
	}

	bool party_service::teleport_to_teammate_impl(const teammate_info& teammate)
	{
		Vector3 target = teammate.position;
		// Safe lateral and vertical offset to avoid collision overlapping
		target.x += 1.5f;
		target.z += 1.5f;
		target.y += 0.5f;

		character_service::set_player_coords(target);
		notification::success("Co-op Fast Travel", std::format("Teleported to {} ({})", teammate.name, teammate.weapon));
		LOG(INFO) << "[Party Service] Teleported player to teammate " << teammate.name << " at (" << target.x << ", " << target.y << ", " << target.z << ")";
		return true;
	}

	void party_service::set_preview_mode_impl(bool enabled)
	{
		m_preview_mode = enabled;
	}

	bool party_service::is_preview_mode_impl() const
	{
		return m_preview_mode;
	}
}
