#include "game/features/command_helpers.hpp"
#include "game/features/features.hpp"
#include "game/patches.hpp"

namespace big::features
{
	namespace
	{
		void enable()
		{
			if (!g_patches)
				return;

			g_patches->m_cannon_fire.apply();
		}

		void disable()
		{
			if (!g_patches)
				return;

			g_patches->m_cannon_fire.restore();
		}
	}

	patch_toggle _inf_cannon_fire{"inf_cannon_fire", "Infinite Cannon Fire", "Cannons fire without needing reload.", false, enable, disable};
}
