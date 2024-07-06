#pragma once

#include "common.hpp"

extern "C" float g_frostcraft_drawn;
extern "C" float g_frostcraft_sheathed;
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
		static void frostcraft_recharge();
		static void instant_charges_greater_sword();
		static void max_slot_armor();
		static void inf_slinger_ammo();
	};
}