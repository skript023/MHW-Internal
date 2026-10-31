#include "game/features/features.hpp"

#include "game/classes/fwddec.hpp"
#include "game/pointers.hpp"

namespace big::features
{
	float_command _critical_boost{"critical_boost", "Critical Boost", "Critical damage multiplier.", 1.f, 100.f, 1.25f};

	// The game caches the stat table, so the value is written every frame from
	// the player tick instead of only when the command changes.
	void apply_critical_boost()
	{
		if (!g_pointers || !g_pointers->m_player_stat || !*g_pointers->m_player_stat)
			return;

		(*g_pointers->m_player_stat)->m_critical_boost = _critical_boost.get_state();
	}
}
