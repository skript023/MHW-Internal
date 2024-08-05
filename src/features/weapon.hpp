#pragma once

#include "common.hpp"
#include "mhw/fwddec.hpp"

extern "C" float g_frostcraft_drawn;
extern "C" float g_frostcraft_sheathed;
extern "C" uint64_t g_equipment_ptr;
//extern "C" bool g_is_frostcraft_drawn;
//extern "C" bool g_is_frostcraft_sheathed;

namespace big
{
	class weapon
	{
	public:
		weapon() = delete;
		~weapon() = delete;

		weapon(weapon const&) = delete;
		weapon(weapon&&) = delete;

		weapon& operator=(weapon const&) = delete;
		weapon& operator=(weapon&&) = delete;

		static void infinite_frostcraft();
		static void instant_charges_greater_sword();
		static void max_slot_armor();
		static void inf_slinger_ammo();
		static Equipment* eqipment();

		static void frostcraft_recharge();
		static void inf_spirit_gauge();
		static void inf_demon_mode();
		static void full_charged_switch_axe();
		static void inf_gl_ammo();
		static void no_gl_cooldown();
	};
}