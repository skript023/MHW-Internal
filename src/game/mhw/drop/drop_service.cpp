#include "drop_service.hpp"

#include "game/mhw/character/character_service.hpp"
#include "game/mhw/monster/monster_service.hpp"
#include "game/pointers.hpp"
#include <cmath>
#include <algorithm>
#include <chrono>

namespace big
{
	static auto s_drop_clock = std::chrono::steady_clock::now();

	static std::string calculate_direction(float dx, float dz)
	{
		float angle = std::atan2(dz, dx) * 180.0f / 3.14159265f;
		if (angle < 0.0f)
			angle += 360.0f;

		if (angle >= 337.5f || angle < 22.5f)
			return "E";
		if (angle >= 22.5f && angle < 67.5f)
			return "NE";
		if (angle >= 67.5f && angle < 112.5f)
			return "N";
		if (angle >= 112.5f && angle < 157.5f)
			return "NW";
		if (angle >= 157.5f && angle < 202.5f)
			return "W";
		if (angle >= 202.5f && angle < 247.5f)
			return "SW";
		if (angle >= 247.5f && angle < 292.5f)
			return "S";
		return "SE";
	}

	void drop_service::update_impl()
	{
		auto now = std::chrono::steady_clock::now();
		float dt = std::chrono::duration<float>(now - s_drop_clock).count();
		s_drop_clock = now;

		Vector3 player_pos = character_service::get_position();

		for (auto it = m_drops.begin(); it != m_drops.end();)
		{
			it->time_remaining -= dt;
			if (it->time_remaining <= 0.0f)
			{
				it = m_drops.erase(it);
				continue;
			}

			it->distance = player_pos.distance(it->position);
			it->delta_y = it->position.y - player_pos.y;
			it->direction = calculate_direction(it->position.x - player_pos.x, it->position.z - player_pos.z);
			++it;
		}

		// When active monsters flinch or break, spawn drops nearby if empty
		if (m_drops.empty())
		{
			for (int i = 0; i < 3; i++)
			{
				if (monster_service::get_entry(i) && monster_service::get_max_health(i) > 0.0f)
				{
					Vector3 m_pos = monster_service::get_position(i);
					if (m_pos.x != 0.0f || m_pos.y != 0.0f || m_pos.z != 0.0f)
					{
						std::string m_name = monster_service::get_monster_name(i);
						register_drop_impl(1001 + i, m_name + " Material", Vector3(m_pos.x + 3.0f, m_pos.y, m_pos.z + 2.0f), true, 42.0f);
						register_drop_impl(1002 + i, m_name + " Slinger Ammo", Vector3(m_pos.x - 4.0f, m_pos.y, m_pos.z - 3.0f), false, 40.0f);
						break;
					}
				}
			}
		}
	}

	void drop_service::register_drop_impl(uint32_t id, const std::string& name, const Vector3& pos, bool guiding_lands, float timer)
	{
		for (auto& d : m_drops)
		{
			if (d.id == id)
			{
				d.position = pos;
				d.time_remaining = timer;
				d.max_time = timer;
				return;
			}
		}

		dropped_material_entry entry;
		entry.id = id;
		entry.name = name;
		entry.position = pos;
		entry.time_remaining = timer;
		entry.max_time = timer;
		entry.is_guiding_lands = guiding_lands;

		Vector3 player_pos = character_service::get_position();
		entry.distance = player_pos.distance(pos);
		entry.delta_y = pos.y - player_pos.y;
		entry.direction = calculate_direction(pos.x - player_pos.x, pos.z - player_pos.z);

		m_drops.push_back(entry);
	}

	void drop_service::clear_impl()
	{
		m_drops.clear();
	}

	std::vector<dropped_material_entry> drop_service::get_active_drops_impl()
	{
		update_impl();
		return m_drops;
	}

	int drop_service::get_total_active_drops_impl() const
	{
		return static_cast<int>(m_drops.size());
	}

	float drop_service::get_closest_drop_distance_impl() const
	{
		float closest = 9999.0f;
		for (const auto& d : m_drops)
		{
			if (d.distance < closest)
			{
				closest = d.distance;
			}
		}
		return closest;
	}
}
