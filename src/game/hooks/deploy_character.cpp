#include "game/hooks/hooks.hpp"
#include "core/hooking/hook_manager.hpp"

#include "game/mhw/character/character_service.hpp"

namespace big
{
	void __fastcall game_hooks::deploy_character(Character a1, uint32_t a2)
	{
		if (&a1)
		{
			character_service::init(a1);
		}

		return base_hook::get_original<game_hooks::deploy_character>()(a1, a2);
	}
}
