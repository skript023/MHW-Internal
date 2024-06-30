#pragma once
#include "common.hpp"

namespace big
{
	class weapon_menu
	{
	public:
		weapon_menu() = delete;
		~weapon_menu() = delete;

		static void render_menu();
	};
}