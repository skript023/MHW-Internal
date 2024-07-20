#pragma once
#include "class/fwddec.hpp"

namespace big
{
	class meal_service
	{
	public:
		explicit meal_service();
		~meal_service() noexcept;

		void init(MealStats* instance);
		MealStats* get() { m_meal_stat == nullptr ? nullptr : m_meal_stat; };
	private:
		MealStats* m_meal_stat = nullptr;
	};

	inline meal_service* g_meal_service{};
}