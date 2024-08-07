#include "monster_service.hpp"
#include <pointers.hpp>

namespace big
{
	monster_service::monster_service()
	{
		g_monster_service = this;
	}
	monster_service::~monster_service() noexcept
	{
		g_monster_service = nullptr;
	}
	bool monster_service::get_entry(int index)
	{
		if (auto data = *g_pointers->m_monster_data)
		{
			switch (index)
			{
			case 0:
				if (auto world_stat = data->m_first_world_monster)
				{
					m_entry[0] = world_stat;

					return true;
				}
				break;
			case 1:
				if (auto world_stat = data->m_second_world_monster)
				{
					m_entry[1] = world_stat;

					return true;
				}
				break;
			case 2:
				if (auto world_stat = data->m_third_world_monster)
				{
					m_entry[2] = world_stat;

					return true;
				}
				break;
			}
		}

		return false;
	}
	float monster_service::get_health(int index)
	{
		if (auto stat = m_entry[index]->m_monster_stats)
		{
			return std::round(stat->m_health);
		}

		return 0.0f;
	}
	float monster_service::get_max_health(int index)
	{
		if (auto stat = m_entry[index]->m_monster_stats)
		{
			return std::round(stat->m_max);
		}

		return 0.0f;
	}
	std::string monster_service::get_monster_name(int index)
	{
		if (auto wt = m_entry[index]->m_monster_world_stats)
		{
			if (auto it = monster_data::monster_data.find(wt->m_monster_id); it != monster_data::monster_data.end())
			{
				return it->second;
			}
		}

		return std::format("Monster Not Found, ID: {}", index);
	}
	float monster_service::get_timer(int index)
	{
		if (auto wt = m_entry[index]->m_monster_world_stats)
		{
			return wt->m_timer;
		}

		return 0.0f;
	}
	float monster_service::get_max_timer(int index)
	{
		if (auto wt = m_entry[index]->m_monster_world_stats)
		{
			return wt->m_max_timer;
		}

		return 0.0f;
	}
	float monster_service::get_size(int index)
	{
		if (m_entry[index])
			return m_entry[index]->m_size;
		return 0.0f;
	}
}