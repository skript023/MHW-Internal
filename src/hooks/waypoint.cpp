#include "hooking.hpp"
#include <services/character/character_service.hpp>

namespace big
{
	int __fastcall hooks::waypoint(Waypoint* a1)
	{
		if (a1)
		{
			g_character_service->init(a1);
		}

		return base_hook::get_original<hooks::waypoint>()(a1);
	}
}