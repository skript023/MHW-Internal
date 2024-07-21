#include "meal_service.hpp"

namespace big
{
	meal_service::meal_service()
	{
		g_meal_service = this;
	}
	meal_service::~meal_service() noexcept
	{
		g_meal_service = nullptr;
	}
	void meal_service::init(MealStats* instance)
	{
		m_meal_stat = instance;
	}
	std::string meal_service::get_buff(uint32_t skill)
	{
		if (m_meal_stat)
		{
			if (auto it = meal_data::meal_buff.find(skill); it != meal_data::meal_buff.end())
			{
				return it->second;
			}
		}

		return "No buff found";
	}
	std::string meal_service::get_buff_category()
	{
		if (m_meal_stat)
		{
			if (auto it = meal_data::skill_category.find(m_meal_stat->m_skill_category); it != meal_data::skill_category.end())
			{
				return it->second;
			}
		}

		return "No buff category found";
	}
}