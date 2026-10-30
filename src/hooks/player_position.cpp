#include "hooking.hpp"

#include <services/character/character_service.hpp>
#include <services/party/party_service.hpp>

namespace big
{
	int __fastcall hooks::player_position(__int64 a1)
	{
		if (a1)
		{
			auto rax = *(PlayerPosition**)(a1 + 0xDB0);

			if (character_service::get_player_entity() == 0 || character_service::get_player_entity() == static_cast<uintptr_t>(a1))
			{
				character_service::init(rax);
				character_service::set_player_entity(static_cast<uintptr_t>(a1));
			}

			if (rax)
			{
				party_service::register_player_entity(static_cast<uintptr_t>(a1), rax->m_position);
			}
		}

		return base_hook::get_original<hooks::player_position>()(a1);
	}
}
