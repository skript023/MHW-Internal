#include "hooking.hpp"
#include <services/character/character_service.hpp>

namespace big
{
	int __fastcall hooks::player_position(__int64 a1)
	{
		if (a1)
		{
			auto rax = *(PlayerPosition**)(a1 + 0xDB0);

			g_character_service->init(rax);
		}

		return g_hooking->m_player_position_hook.get_original<decltype(&hooks::player_position)>()(a1);
	}
}