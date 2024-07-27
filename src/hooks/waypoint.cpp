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

		return g_hooking->m_waypoint_hook.get_original<decltype(&hooks::waypoint)>()(a1);
	}
}