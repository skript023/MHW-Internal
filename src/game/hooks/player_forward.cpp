#include "game/hooks/hooks.hpp"
#include "core/hooking/hook_manager.hpp"

#include "game/mhw/character/character_service.hpp"

namespace big
{
	int __fastcall game_hooks::player_forward(PlayerForward* a1, __int64 a2)
	{
		if (a1)
		{
			character_service::init(a1);
		}

		return base_hook::get_original<game_hooks::player_forward>()(a1, a2);
	}
}
