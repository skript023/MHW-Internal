#include "game/hooks/hooks.hpp"
#include "core/hooking/hook_manager.hpp"
#include "game/pointers.hpp"

#include "game/mhw/equipment/weapon_service.hpp"

namespace big
{
	__int64 __fastcall game_hooks::highlighted_weapon(__int64 a1)
	{
		auto v3 = *(uint32_t*)(a1 + 24);
		auto v6 = *(uint32_t*)(a1 + 36);
		auto v7 = *(uint32_t*)(a1 + 32);

		if (v3 == 1)
		{
			auto v8 = g_pointers->m_select_waapon(v7, v6);
			weapon_service::init_melee(reinterpret_cast<MeleeWeapon*>(v8));
		}
		else
		{
			auto v9 = g_pointers->m_select_waapon(v7, v6);
			weapon_service::init_ranged(reinterpret_cast<RangedWeapon*>(v9));
		}

		return base_hook::get_original<game_hooks::highlighted_weapon>()(a1);
	}
}
