#pragma once
#include "mhw/fwddec.hpp"
#include "meal_data.hpp"

namespace big
{
	class meal_service
	{
	public:
		explicit meal_service();
		~meal_service() noexcept;

		void init(MealStats* instance);
		MealStats* instance() { return m_meal_stat == nullptr ? nullptr : m_meal_stat; };
		std::string get_buff(uint32_t skill);
		std::string get_buff_category();
	private:
		MealStats* m_meal_stat = nullptr;
	};

	inline meal_service* g_meal_service{};
}