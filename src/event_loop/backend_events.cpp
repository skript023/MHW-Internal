#include "backend_events.hpp"
#include "script.hpp"
#include <menu/player_menu.h>

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
				player_menu::critical_boost();
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