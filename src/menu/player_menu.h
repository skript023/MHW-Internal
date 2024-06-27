#pragma once
#include "common.hpp"

namespace big
{
	class player_menu
	{
	public:
		static void render_menu();
	private:
		static void invulnerable();

		static void infinite_stamina();
	};
}