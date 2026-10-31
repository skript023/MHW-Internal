#include "game/hooks/hooks.hpp"
#include "game/settings.hpp"
#include "core/hooking/hook_manager.hpp"

#include "core/threading/script_mgr.hpp"

namespace big
{
	int __fastcall game_hooks::material_handler(uint32_t a1, int a2)
	{
		if (g_settings->crafting.ignore_material)
			return 99;

		return base_hook::get_original<material_handler>()(a1, a2);
	}
}