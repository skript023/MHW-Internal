#include "backend_events.hpp"
#include "script.hpp"
#include <features/player.hpp>

namespace big
{
	void backend_events::features()
	{
		
	}

	void backend_events::player_event()
	{
		while (true)
		{
			TRY_CLAUSE
			{
				player::critical_boost();
				player::infinite_stamina();
				player::invulnerable();
				player::easy_part_breaker();
				player::part_breaker();
				player::max_armor_skill();
				player::max_stat_bonus();
			}
			EXCEPT_CLAUSE
			script::get_current()->yield();
		}
	}

	void backend_events::script_func()
	{
		while (true)
		{
			TRY_CLAUSE
			{
				g_settings->attempt_save();
				features();
			} 
			EXCEPT_CLAUSE
			script::get_current()->yield();
		}
	}
}