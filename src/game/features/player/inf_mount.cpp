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

			g_patches->m_inf_mount.apply();
		}

		void disable()
		{
			if (!g_patches)
				return;

			g_patches->m_inf_mount.restore();
		}
	}

	patch_toggle _inf_mount{"inf_mount", "Infinite Mounting", "Never get thrown off while mounting monsters.", false, enable, disable};
}
