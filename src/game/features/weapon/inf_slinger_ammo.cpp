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

			g_patches->m_slinger_max_ammo.apply();
		}

		void disable()
		{
			if (!g_patches)
				return;

			g_patches->m_slinger_max_ammo.restore();
		}
	}

	patch_toggle _inf_slinger_ammo{"inf_slinger_ammo", "Infinite Slinger Ammo", "Slinger ammo is not consumed.", false, enable, disable};
}
