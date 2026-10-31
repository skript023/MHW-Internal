#include "game/features/features.hpp"
#include "game/hooks/hooks.hpp"
#include "core/hooking/hook_manager.hpp"
#include "game/classes/fwddec.hpp"

namespace big
{
	unsigned int __fastcall game_hooks::glaive_spirit_gauge(__int64 a1, float a2)
	{
		if (features::_inf_spirit_gauge.get_state() && a1)
		{
			auto* glaive = reinterpret_cast<InsectGlaiveData*>(a1);
			glaive->m_red_extract_duration = 1.0f;
			glaive->m_triple_buff_level = 3;
			glaive->m_orange_extract_duration = 1.0f;
		}

		return base_hook::get_original<game_hooks::glaive_spirit_gauge>()(a1, a2);
	}
}
