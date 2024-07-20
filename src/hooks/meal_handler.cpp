#include "hooking.hpp"
#include <services/meal/meal_service.hpp>

namespace big
{
	int __fastcall hooks::meal_handler(__int64 a1, MealStats* a2)
	{
		g_meal_service->init(a2);

		return g_hooking->m_meal_handler_hook.get_original<decltype(&hooks::meal_handler)>()(a1, a2);
	}
}