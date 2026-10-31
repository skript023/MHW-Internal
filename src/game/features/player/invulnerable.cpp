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

			g_patches->m_player_anim.apply();
			g_patches->m_player_state.apply();
			g_patches->m_player_health.apply();
		}

		void disable()
		{
			if (!g_patches)
				return;

			g_patches->m_player_anim.restore();
			g_patches->m_player_state.restore();
			g_patches->m_player_health.restore();
		}
	}

	patch_toggle _invulnerable{"invulnerable", "Invulnerable", "Immune to all monster attacks and damage.", false, enable, disable};
}
