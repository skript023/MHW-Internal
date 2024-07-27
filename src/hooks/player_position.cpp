#include "hooking.hpp"
#include <services/character/character_service.hpp>

namespace big
{
	int __fastcall hooks::player_position(PlayerPosition* a1)
	{
		if (a1)
		{
			g_character_service->init(a1);
		}

		return g_hooking->m_player_position_hook.get_original<decltype(&hooks::player_position)>()(a1);
	}
}