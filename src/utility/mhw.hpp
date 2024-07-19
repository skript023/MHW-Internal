#pragma once
#include <pointers.hpp>

namespace big::mhw
{
	inline MonsterEntry* monster_stats(int index)
	{
		if (auto data = *g_pointers->m_monster_data)
		{
			switch (index)
			{
			case 0:
				if (auto world_stat = data->m_first_world_monster)
				{
					return world_stat;
				}
				break;
			case 1:
				if (auto world_stat = data->m_second_world_monster)
				{
					return world_stat;
				}
				break;
			case 2:
				if (auto world_stat = data->m_third_world_monster)
				{
					return world_stat;
				}
				break;
			}
		}

		return nullptr;
	}
}