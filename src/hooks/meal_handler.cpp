#include "hooking.hpp"
#include <services/meal/meal_service.hpp>

namespace big
{
	int __fastcall hooks::meal_handler(__int64 a1, MealStats* a2)
	{
		if (a1)
		{
			meal_service::init(a2);
		}

		return base_hook::get_original<hooks::meal_handler>()(a1, a2);
	}
}