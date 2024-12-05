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

		return base_hook::get_original<hooks::player_position>()(a1);
	}
}