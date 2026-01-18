#include "meal_service.hpp"

namespace big
{
	void meal_service::init_impl(MealStats* instance)
	{
		m_meal_stat = instance;
	}
	std::string_view meal_service::get_buff_impl(uint32_t skill)
	{
		if (m_meal_stat)
		{
			for (const auto& [id, name] : meal_data::meal_buff)
			{
				if (skill == id)
				{
					return name;
				}
			}
		}

		return "No buff found";
	}
	std::string_view meal_service::get_buff_category_impl()
	{
		if (m_meal_stat)
		{
			for (const auto& [id, name] : meal_data::skill_category)
			{
				if (m_meal_stat->m_skill_category == id)
				{
					return name;
				}
			}
		}

		return "No buff category found";
	}
}