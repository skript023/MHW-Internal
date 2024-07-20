#include "hooking.hpp"
#include "pointers.hpp"
#include <services/equipment/weapon_service.hpp>

namespace big
{
	__int64 __fastcall hooks::highlighted_weapon(__int64 a1)
	{
		auto v3 = *(uint32_t*)(a1 + 24);
		auto v6 = *(uint32_t*)(a1 + 36);
		auto v7 = *(uint32_t*)(a1 + 32);

		if (v3 == 1)
		{
			auto v8 = g_pointers->m_select_waapon(v7, v6);
			g_weapon_service->init_melee(reinterpret_cast<MeleeWeapon*>(v8));
		}
		else
		{
			auto v9 = g_pointers->m_select_waapon(v7, v6);
			g_weapon_service->init_ranged(reinterpret_cast<RangedWeapon*>(v9));
		}

		return g_hooking->m_weapon_hook.get_original<decltype(&hooks::highlighted_weapon)>()(a1);
	}
}