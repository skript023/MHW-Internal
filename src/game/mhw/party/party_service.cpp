#include "party_service.hpp"
#include "game/mhw/character/character_service.hpp"
#include "game/pointers.hpp"
#include "core/logging/logger.hpp"
#include <astra/host/notification.hpp>
#include "core/ui/fonts/icon_list.hpp"
#include <algorithm>
#include <cmath>

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
		m_preview_mode = false;
	}

	void party_service::register_player_entity_impl(uintptr_t entity, Vector3 pos)
	{
		if (entity < 0x10000)
			return;

		uintptr_t local_entity = character_service::get_player_entity();
		if (entity == local_entity)
			return;

		const auto* hunter = reinterpret_cast<const sHunter*>(entity);
		auto now = std::chrono::steady_clock::now();

		// Check if entity is already registered
		for (auto& t : m_real_teammates)
		{
			if (t.entity_ptr == entity)
			{
				t.position = pos;
				t.last_seen = now;
				t.is_active = true;

				float hp = hunter->m_health;
				if (hp >= 0.0f && hp <= 500.0f)
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

			int w_id = hunter->m_weapon_id;
			if (w_id >= 0 && w_id <= 13)
			{
				info.weapon_id = w_id;
				info.weapon = get_weapon_name(w_id);
			}
			else
			{
				info.weapon_id = info.player_id % 14;
				info.weapon = get_weapon_name(info.weapon_id);
			}

			float hp = hunter->m_health;
			if (hp >= 0.0f && hp <= 500.0f)
			{
				info.health = hp;
			}
			else
			{
				info.health = 150.0f;
			}
			info.max_health = 150.0f;

			info.name = "Teammate [P" + std::to_string(info.player_id + 1) + "]";
			m_real_teammates.push_back(info);

			LOG(INFO) << "[Party Service] Registered co-op teammate [P" << (info.player_id + 1) << "] at ("
			          << pos.x << ", " << pos.y << ", " << pos.z << ") Weapon: " << info.weapon;
		}
	}

	void party_service::update_impl()
	{
		auto now = std::chrono::steady_clock::now();
		Vector3 player_pos = character_service::get_position();

		// If preview mode is explicitly enabled by settings for debugging, maintain simulated co-op squad
		if (m_preview_mode)
		{
			Vector3 player_fwd = character_service::get_forward_pos();
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

			// Teammate 1: Front-Right (~35m)
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

			// Teammate 2: Front-Left (~65m)
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

			// Teammate 3: Behind (~90m)
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

			return;
		}

		// Real In-World Hunter Scan via sPlayerData
		std::vector<teammate_info> active_teammates;
		uintptr_t local_hunter = character_service::get_player_entity();

		if (g_pointers && g_pointers->m_player_data && *g_pointers->m_player_data)
		{
			sPlayerData* p_data = *g_pointers->m_player_data;

			// Cache session hunter names from sSessionData if available
			std::vector<std::string> session_names;
			if (g_pointers->m_session_data && *g_pointers->m_session_data)
			{
				sSessionData* s_session = *g_pointers->m_session_data;
				if (s_session->m_member_array && s_session->m_member_count > 0)
				{
					uint32_t limit = std::min(s_session->m_member_count, 16u);
					for (uint32_t s = 0; s < limit; ++s)
					{
						sSessionEntry* entry = s_session->m_member_array[s];
						if (entry && entry->m_hunter_info)
						{
							sHunterInfo* p_info = entry->m_hunter_info;
							if (p_info->m_name[0] != '\0')
							{
								session_names.emplace_back(p_info->m_name);
							}
							else if (p_info->m_name_short[0] != '\0')
							{
								session_names.emplace_back(p_info->m_name_short);
							}
							else
							{
								session_names.emplace_back("");
							}
						}
					}
				}
			}

			// Check active party slots 0..3
			for (int slot = 0; slot < 4; ++slot)
			{
				sHunter* hunter = p_data->m_slots[slot].m_hunter;
				if (!hunter || !hunter->m_position_component)
					continue;

				uintptr_t hunter_addr = reinterpret_cast<uintptr_t>(hunter);

				// Determine if this is local player
				if (local_hunter != 0)
				{
					if (hunter_addr == local_hunter)
						continue;
				}
				else
				{
					// Detect local player by position proximity if not yet cached
					float ldx = hunter->m_position_component->m_position.x - player_pos.x;
					float ldy = hunter->m_position_component->m_position.y - player_pos.y;
					float ldz = hunter->m_position_component->m_position.z - player_pos.z;
					if (std::sqrt(ldx * ldx + ldy * ldy + ldz * ldz) < 1.0f)
					{
						character_service::set_player_entity(hunter_addr);
						local_hunter = hunter_addr;
						continue;
					}
				}

				Vector3 pos = hunter->m_position_component->m_position;
				if (std::isnan(pos.x) || std::isnan(pos.y) || std::isnan(pos.z))
					continue;

				float hp = hunter->m_health;
				float max_hp = hunter->m_max_health;
				if (max_hp <= 1.0f || max_hp > 500.0f) max_hp = 150.0f;
				if (hp < 0.0f) hp = 0.0f;
				if (hp > max_hp) hp = max_hp;

				int w_id = hunter->m_weapon_id;
				if (w_id < 0 || w_id > 13) w_id = 0;

				teammate_info t;
				t.player_id = slot;
				t.entity_ptr = hunter_addr;
				t.position = pos;
				t.health = hp;
				t.max_health = max_hp;
				t.weapon_id = w_id;
				t.weapon = get_weapon_name(w_id);
				t.is_local = false;
				t.is_active = true;
				t.last_seen = now;

				float dx = pos.x - player_pos.x;
				float dy = pos.y - player_pos.y;
				float dz = pos.z - player_pos.z;
				t.distance = std::sqrt(dx * dx + dy * dy + dz * dz) * 0.03048f;
				if (t.distance < 0.1f) t.distance = 0.1f;

				if (slot < static_cast<int>(session_names.size()) && !session_names[slot].empty())
				{
					t.name = session_names[slot];
				}
				else
				{
					t.name = "Teammate [P" + std::to_string(slot + 1) + "]";
				}

				active_teammates.push_back(std::move(t));
			}
		}

		m_real_teammates = std::move(active_teammates);
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
		// Safe lateral offset to avoid character overlapping
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
