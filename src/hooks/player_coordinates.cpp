#include "hooking.hpp"
#include <services/character/character_service.hpp>

namespace big
{
	int __fastcall hooks::player_coordinates(PlayerCoordinates* a1, __int64 a2)
	{
		if (a1)
		{

		}

		return g_hooking->m_player_coordinates_hook.get_original<decltype(&hooks::player_coordinates)>()(a1, a2);
	}
}