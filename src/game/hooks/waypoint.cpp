#include "game/hooks/hooks.hpp"
#include "core/hooking/hook_manager.hpp"

#include "game/mhw/character/character_service.hpp"

namespace big
{
	int __fastcall game_hooks::waypoint(Waypoint* a1)
	{
		if (a1)
		{
			character_service::init(a1);
		}

		return base_hook::get_original<game_hooks::waypoint>()(a1);
	}
}
