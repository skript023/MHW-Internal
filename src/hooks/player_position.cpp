#include "hooking.hpp"

#include <services/character/character_service.hpp>

namespace big
{
	int __fastcall hooks::player_position(__int64 a1)
	{
		if (a1)
		{
			auto rax = *(PlayerPosition**)(a1 + 0xDB0);

			character_service::init(rax);
			character_service::set_player_entity(static_cast<uintptr_t>(a1));
		}

		return base_hook::get_original<hooks::player_position>()(a1);
	}
}
