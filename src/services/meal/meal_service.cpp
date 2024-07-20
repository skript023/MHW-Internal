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
}