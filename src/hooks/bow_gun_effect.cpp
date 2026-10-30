#include "hooking.hpp"
#include <mhw/fwddec.hpp>

namespace big
{
	int __fastcall hooks::bow_gun_effect(__int64 a1)
	{
		auto* bowgun = reinterpret_cast<BowgunData*>(a1);
		if (bowgun)
		{
			bowgun->m_mods.m_recoil = 4;
			bowgun->m_mods.m_reload = 4;
			bowgun->m_mods.m_deviation = 3;
			bowgun->m_mods.m_close_range = 3;
			bowgun->m_mods.m_ranged_atk_up = 3;
			bowgun->m_mods.m_shield = 4;
			bowgun->m_mods.m_special_scope = 1;
			bowgun->m_mods.m_evading_reload = 3;
			bowgun->m_mods.m_wyvernheart_mod = 1;
			bowgun->m_mods.m_wyvernsnipe_mod = 1;
			bowgun->m_mods.m_wyvernblast_mod = 1;
			bowgun->m_mods.m_power_barrel = 1;
			bowgun->m_mods.m_long_barrel = 1;

			auto* ammo_params = bowgun->m_param_ptr;
			if (ammo_params)
			{
				if (ammo_params->m_ammo_type_1 <= 12)
				{
					bowgun->m_ammo_clip[ammo_params->m_ammo_type_1] -= 1;
				}
				if (ammo_params->m_ammo_type_2 <= 12)
				{
					bowgun->m_ammo_clip[ammo_params->m_ammo_type_2] -= 1;
				}
				if (ammo_params->m_ammo_type_3 <= 12)
				{
					bowgun->m_ammo_clip[ammo_params->m_ammo_type_3] -= 1;
				}
			}
		}

		return base_hook::get_original<hooks::bow_gun_effect>()(a1);
	}
}
