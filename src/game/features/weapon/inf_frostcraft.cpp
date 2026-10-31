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

			g_patches->m_inf_frostcraft.apply();
		}

		void disable()
		{
			if (!g_patches)
				return;

			g_patches->m_inf_frostcraft.restore();
		}
	}

	patch_toggle _inf_frostcraft{"inf_frostcraft", "Infinite Frostcraft", "Frostcraft gauge stays permanently full.", false, enable, disable};
}
