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
	void character_service::set_name(std::string_view name)
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
}
