#include "game/hooks/hooks.hpp"
#include "game/settings.hpp"
#include "core/hooking/hook_manager.hpp"

#include "game/mhw/monster/monster_service.hpp"

namespace big
{
	__int64 __fastcall game_hooks::monster_part_breaking(__int64 a1, unsigned int a2)
	{
		if (a1)
		{
			monster_service::register_parts_manager(static_cast<uintptr_t>(a1), a2);
		}

		if (g_settings->player.easy_part_breaker)
		{
			__int64 result = a1;

			if (a2 < *(DWORD*)(a1 + 16))
			{
				result = *(uintptr_t*)(a1 + 32);

				*(DWORD*)(*(uintptr_t*)(result + 8i64 * a2) + 16i64) = 0;
			}

			return result;
		}

		return base_hook::get_original<game_hooks::monster_part_breaking>()(a1, a2);
	}
}
