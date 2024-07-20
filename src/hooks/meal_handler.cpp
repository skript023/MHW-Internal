#include "hooking.hpp"

namespace big
{
	int __fastcall hooks::meal_handler(__int64 a1, MealStats* a2)
	{

		return g_hooking->m_meal_handler_hook.get_original<decltype(&hooks::meal_handler)>()(a1, a2);
	}
}