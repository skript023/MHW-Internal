#include "character_service.hpp"

namespace big
{
	character_service::character_service()
	{
		g_character_service = this;
	}
	character_service::~character_service() noexcept
	{
		g_character_service = nullptr;
	}
	void character_service::init(Character chara)
	{
		m_character = chara;
	}
	void character_service::init(PlayerPosition* coords)
	{
		m_position = coords;
	}
	void character_service::init(Waypoint* wp)
	{
		m_waypoint = wp;
	}
	void character_service::init(PlayerForward* fwd)
	{
		m_forward = fwd;
	}
	std::string character_service::get_name() const
	{
		return m_character.m_name;
	}
	uint32_t character_service::get_zeni() const
	{
		return m_character.m_zeni;
	}
	uint32_t character_service::get_high_rank_level() const
	{
		return m_character.m_high_rank_level;
	}
	uint32_t character_service::get_research_points() const
	{
		return m_character.m_research_points;
	}
	uint32_t character_service::get_high_rank_experience() const
	{
		return m_character.m_high_rank_experience;
	}
	uint32_t character_service::get_time_playing() const
	{
		return m_character.m_time_playing;
	}
	uint32_t character_service::get_master_rank_level() const
	{
		return m_character.m_master_rank_level;
	}
	uint32_t character_service::get_master_rank_experience() const
	{
		return m_character.m_master_rank_experience;
	}
	Vector3 character_service::get_position() const
	{
		return m_position == nullptr ? Vector3(0.f, 0.f, 0.f) : m_position->m_position;
	}
	Vector3 character_service::get_waypoint_pos() const
	{
		return m_waypoint == nullptr ? Vector3(0.f, 0.f, 0.f) : m_waypoint->m_waypoint;
	}
	void character_service::set_name(std::string_view const& name)
	{
		strcpy(m_character.m_name, name.data());
	}
	void character_service::set_zeni(uint32_t zeni)
	{
		m_character.m_zeni = zeni;
	}
	void character_service::set_high_rank_level(uint32_t level)
	{
		m_character.m_high_rank_level = level;
	}
	void character_service::set_research_points(uint32_t pts)
	{
		m_character.m_research_points = pts;
	}
	void character_service::set_high_rank_experience(uint32_t exp)
	{
		m_character.m_high_rank_experience = exp;
	}
	void character_service::set_time_playing(uint32_t time)
	{
		m_character.m_time_playing = time;
	}
	void character_service::set_master_rank_level(uint32_t level)
	{
		m_character.m_master_rank_level = level;
	}
	void character_service::set_master_rank_experience(uint32_t exp)
	{
		m_character.m_master_rank_experience = exp;
	}
	void character_service::set_player_coords(Vector3 vec3)
	{
		if (m_position)
		{
			m_position->m_position = vec3;
			m_position->m_psycal_pos = vec3;
		}
	}
	void character_service::set_player_to_waypoint()
	{
		if (m_position && m_waypoint)
		{
			m_position->m_position = m_waypoint->m_waypoint;
			m_position->m_psycal_pos = m_waypoint->m_waypoint;
		}
	}
	void character_service::set_player_forward()
	{
		if (m_position && m_forward)
		{
			m_position->m_position = m_forward->m_forward_pos;
			m_position->m_psycal_pos = m_forward->m_forward_pos;
		}
	}
}
