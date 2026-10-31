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

			g_patches->m_player_stamina.apply();
		}

		void disable()
		{
			if (!g_patches)
				return;

			g_patches->m_player_stamina.restore();
		}
	}

	patch_toggle _inf_stamina{"inf_stamina", "Infinite Stamina", "Stamina never depletes.", true, enable, disable};
}
