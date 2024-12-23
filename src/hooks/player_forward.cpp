#include "hooking.hpp"
#include <services/character/character_service.hpp>

namespace big
{
	int __fastcall hooks::player_forward(PlayerForward* a1, __int64 a2)
	{
		if (a1)
		{
			g_character_service->init(a1);
		}

		return base_hook::get_original<hooks::player_forward>()(a1, a2);
	}
}