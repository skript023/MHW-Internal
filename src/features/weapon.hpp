#pragma once

#include "common.hpp"

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
	};
}