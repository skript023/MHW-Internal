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

			g_patches->m_gathering_resource.apply();
		}

		void disable()
		{
			if (!g_patches)
				return;

			g_patches->m_gathering_resource.restore();
		}
	}

	patch_toggle _inf_gathering{"inf_gathering", "Infinite Gathering", "Gather unlimited times from mining outcrops, plants and bones.", false, enable, disable};
}
