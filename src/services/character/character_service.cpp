#include "character_service.hpp"

#include <cstring>
#include <vector>

namespace big
{
	void character_service::init_impl(Character chara)
	{
		m_character = chara;
	}

	void character_service::init_impl(PlayerPosition* coords)
	{
		m_position = coords;
	}

	void character_service::init_impl(Waypoint* wp)
	{
		m_waypoint = wp;
	}

	void character_service::init_impl(PlayerForward* fwd)
	{
		m_forward = fwd;
	}

	void character_service::set_player_entity_impl(uintptr_t entity)
	{
		m_player_entity = entity;
	}

	uintptr_t character_service::get_player_entity_impl() const
	{
		return m_player_entity;
	}

	std::string character_service::get_name_impl() const
	{
		return m_character.m_name;
	}

	uint32_t character_service::get_zeni_impl() const
	{
		return m_character.m_zeni;
	}

	uint32_t character_service::get_high_rank_level_impl() const
	{
		return m_character.m_high_rank_level;
	}

	uint32_t character_service::get_research_points_impl() const
	{
		return m_character.m_research_points;
	}

	uint32_t character_service::get_high_rank_experience_impl() const
	{
		return m_character.m_high_rank_experience;
	}

	uint32_t character_service::get_time_playing_impl() const
	{
		return m_character.m_time_playing;
	}

	uint32_t character_service::get_master_rank_level_impl() const
	{
		return m_character.m_master_rank_level;
	}

	uint32_t character_service::get_master_rank_experience_impl() const
	{
		return m_character.m_master_rank_experience;
	}

	Vector3 character_service::get_position_impl() const
	{
		return m_position == nullptr ? Vector3(0.f, 0.f, 0.f) : m_position->m_position;
	}

	Vector3 character_service::get_forward_pos_impl() const
	{
		return m_forward == nullptr ? Vector3(0.f, 0.f, 0.f) : m_forward->m_forward_pos;
	}

	Vector3 character_service::get_waypoint_pos_impl() const
	{
		return m_waypoint == nullptr ? Vector3(0.f, 0.f, 0.f) : m_waypoint->m_waypoint;
	}

	std::vector<hunter_buff_data> character_service::get_active_buffs_impl() const
	{
		std::vector<hunter_buff_data> buffs;
		if (m_player_entity == 0)
			return buffs;

		const auto* hunter = reinterpret_cast<const sHunter*>(m_player_entity);
		const auto* buffs_ptr = hunter->m_buffs_ptr;
		if (!buffs_ptr)
			return buffs;

		auto check_buff = [&](float val, const char* name, float max_dur, bool is_buff, bool is_perm, uint32_t color) {
			if (val > 0.0f)
			{
				buffs.push_back({name, val, max_dur, is_buff, is_perm, color});
			}
		};

		check_buff(buffs_ptr->m_might_seed, "Might Seed", 180.0f, true, false, 0xFFFF8C00);
		check_buff(buffs_ptr->m_demon_powder, "Demon Powder", 180.0f, true, false, 0xFFFF4500);
		check_buff(buffs_ptr->m_demondrug, "Demondrug", 1.0f, true, true, 0xFFDC143C);
		check_buff(buffs_ptr->m_adamant_seed, "Adamant Seed", 180.0f, true, false, 0xFF1E90FF);
		check_buff(buffs_ptr->m_hardshell_powder, "Hardshell Powder", 180.0f, true, false, 0xFF4682B4);
		check_buff(buffs_ptr->m_armorskin, "Armorskin", 1.0f, true, true, 0xFF4169E1);
		check_buff(buffs_ptr->m_dash_juice, "Dash Juice", 240.0f, true, false, 0xFF32CD32);
		check_buff(buffs_ptr->m_cool_drink, "Cool Drink", 600.0f, true, false, 0xFF00CED1);
		check_buff(buffs_ptr->m_hot_drink, "Hot Drink", 600.0f, true, false, 0xFFFF6347);

		return buffs;
	}

	std::vector<mantle_slot_data> character_service::get_mantles_impl() const
	{
		std::vector<mantle_slot_data> mantles;
		if (m_player_entity == 0)
			return mantles;

		const auto* hunter = reinterpret_cast<const sHunter*>(m_player_entity);

		// Mantle Slot 1
		float dur0 = hunter->m_mantle_duration[0];
		float cd0 = hunter->m_mantle_cooldown[0];
		mantles.push_back({0, "Specialized Tool 1", dur0, 120.0f, cd0, 150.0f, dur0 > 0.0f, cd0 > 0.0f});

		// Mantle Slot 2
		float dur1 = hunter->m_mantle_duration[1];
		float cd1 = hunter->m_mantle_cooldown[1];
		mantles.push_back({1, "Specialized Tool 2", dur1, 120.0f, cd1, 150.0f, dur1 > 0.0f, cd1 > 0.0f});

		return mantles;
	}

	void character_service::set_name_impl(std::string_view const& name)
	{
		strncpy(m_character.m_name, name.data(), sizeof(m_character.m_name) - 1);
		m_character.m_name[sizeof(m_character.m_name) - 1] = '\0';
	}

	void character_service::set_zeni_impl(uint32_t zeni)
	{
		m_character.m_zeni = zeni;
	}

	void character_service::set_high_rank_level_impl(uint32_t level)
	{
		m_character.m_high_rank_level = level;
	}

	void character_service::set_research_points_impl(uint32_t pts)
	{
		m_character.m_research_points = pts;
	}

	void character_service::set_high_rank_experience_impl(uint32_t exp)
	{
		m_character.m_high_rank_experience = exp;
	}

	void character_service::set_time_playing_impl(uint32_t time)
	{
		m_character.m_time_playing = time;
	}

	void character_service::set_master_rank_level_impl(uint32_t level)
	{
		m_character.m_master_rank_level = level;
	}

	void character_service::set_master_rank_experience_impl(uint32_t exp)
	{
		m_character.m_master_rank_experience = exp;
	}

	void character_service::set_player_coords_impl(Vector3 vec3)
	{
		if (m_position)
		{
			m_position->m_position = vec3;
			m_position->m_psycal_pos = vec3;
		}
	}

	void character_service::set_player_to_waypoint_impl()
	{
		if (m_position && m_waypoint)
		{
			m_position->m_position = m_waypoint->m_waypoint;
			m_position->m_psycal_pos = m_waypoint->m_waypoint;
		}
	}

	void character_service::set_player_forward_impl()
	{
		if (m_position && m_forward)
		{
			m_position->m_position = m_forward->m_forward_pos;
			m_position->m_psycal_pos = m_forward->m_forward_pos;
		}
	}
}
