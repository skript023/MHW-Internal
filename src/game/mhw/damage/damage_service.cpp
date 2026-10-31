#include "damage_service.hpp"

#include "game/mhw/character/character_service.hpp"
#include "game/mhw/monster/monster_service.hpp"
#include "game/pointers.hpp"
#include <algorithm>
#include <chrono>

namespace big
{
	static auto s_start_time = std::chrono::steady_clock::now();

	damage_service::damage_service()
	{
		party_damage_entry local_p;
		local_p.player_id = 0;
		local_p.name = "Hunter (You)";
		local_p.weapon = "Great Sword";
		local_p.total_damage = 0;
		local_p.dps = 0.0f;
		local_p.damage_percent = 100.0f;
		local_p.cart_count = 0;
		local_p.is_local = true;
		m_players.push_back(local_p);
	}

	void damage_service::update_impl()
	{
		std::string name = character_service::get_name();
		if (!name.empty() && !m_players.empty())
		{
			m_players[0].name = name + " (You)";
		}

		for (int i = 0; i < 3; i++)
		{
			if (monster_service::get_entry(i))
			{
				float cur_hp = monster_service::get_health(i);
				float max_hp = monster_service::get_max_health(i);

				if (max_hp > 0.0f)
				{
					if (!m_initialized_hp || m_last_monster_hp[i] <= 0.0f)
					{
						m_last_monster_hp[i] = cur_hp;
						m_initialized_hp = true;
					}
					else if (cur_hp < m_last_monster_hp[i])
					{
						float delta = m_last_monster_hp[i] - cur_hp;
						if (delta > 0.0f && delta < 50000.0f)
						{
							record_damage_impl(0, static_cast<uint32_t>(delta));
						}
						m_last_monster_hp[i] = cur_hp;
					}
					else if (cur_hp > m_last_monster_hp[i])
					{
						m_last_monster_hp[i] = cur_hp;
					}
				}
			}
		}

		auto now = std::chrono::steady_clock::now();
		m_hunt_time = std::chrono::duration<float>(now - s_start_time).count();

		uint32_t total_party_dmg = get_total_party_damage_impl();
		for (auto& p : m_players)
		{
			p.dps = (m_hunt_time > 1.0f) ? (static_cast<float>(p.total_damage) / m_hunt_time) : static_cast<float>(p.total_damage);
			p.damage_percent = (total_party_dmg > 0)
				? (static_cast<float>(p.total_damage) / static_cast<float>(total_party_dmg) * 100.0f)
				: 0.0f;
		}
	}

	void damage_service::record_damage_impl(int player_id, uint32_t damage)
	{
		for (auto& p : m_players)
		{
			if (p.player_id == player_id)
			{
				p.total_damage += damage;
				return;
			}
		}

		party_damage_entry entry;
		entry.player_id = player_id;
		entry.name = "Teammate " + std::to_string(player_id + 1);
		entry.total_damage = damage;
		m_players.push_back(entry);
	}

	void damage_service::record_cart_impl(int player_id)
	{
		for (auto& p : m_players)
		{
			if (p.player_id == player_id)
			{
				p.cart_count++;
				return;
			}
		}
	}

	void damage_service::reset_impl()
	{
		s_start_time = std::chrono::steady_clock::now();
		m_hunt_time = 0.0f;
		for (auto& p : m_players)
		{
			p.total_damage = 0;
			p.dps = 0.0f;
			p.damage_percent = 0.0f;
			p.cart_count = 0;
		}
		for (int i = 0; i < 3; i++)
		{
			m_last_monster_hp[i] = 0.0f;
		}
		m_initialized_hp = false;
	}

	std::vector<party_damage_entry> damage_service::get_party_damage_impl()
	{
		update_impl();
		return m_players;
	}

	float damage_service::get_hunt_time_impl() const
	{
		return m_hunt_time;
	}

	uint32_t damage_service::get_total_party_damage_impl() const
	{
		uint32_t total = 0;
		for (const auto& p : m_players)
		{
			total += p.total_damage;
		}
		return total;
	}

	float damage_service::get_total_party_dps_impl() const
	{
		return (m_hunt_time > 1.0f) ? (static_cast<float>(get_total_party_damage_impl()) / m_hunt_time) : 0.0f;
	}
}
