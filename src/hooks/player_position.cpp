#include "hooking.hpp"

#include <services/character/character_service.hpp>
#include <services/party/party_service.hpp>
#include <mhw/fwddec.hpp>

namespace big
{
	int __fastcall hooks::player_position(__int64 a1)
	{
		if (a1)
		{
			auto* hunter = reinterpret_cast<sHunter*>(a1);
			auto* pos_comp = hunter->m_position_component;

			if (character_service::get_player_entity() == 0 || character_service::get_player_entity() == static_cast<uintptr_t>(a1))
			{
				character_service::init(pos_comp);
				character_service::set_player_entity(static_cast<uintptr_t>(a1));
			}

			if (pos_comp)
			{
				party_service::register_player_entity(static_cast<uintptr_t>(a1), pos_comp->m_position);
			}
		}

		return base_hook::get_original<hooks::player_position>()(a1);
	}
}
