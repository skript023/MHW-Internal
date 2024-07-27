#include "hooking.hpp"
#include <services/character/character_service.hpp>

namespace big
{
	int __fastcall hooks::player_forward(PlayerForward* a1, __int64 a2)
	{
		if (a1)
		{
			g_character_service->init(a1);
			LOG(INFO) << "Player forward coords initialized. " << a1;
		}

		return g_hooking->m_player_forward_hook.get_original<decltype(&hooks::player_forward)>()(a1, a2);
	}
}