#pragma once
#include "imgui.h"
#include "core/ui/menu/submenu.hpp"

namespace big
{
	class view
	{
	public:
		static void register_submenu();

		static void player_submenu();
		static void weapon_submenu();
		static void crafting_submenu();
		static void teleport_submenu();
		static void qol_submenu();
		static void setting_submenu();
	};
}
