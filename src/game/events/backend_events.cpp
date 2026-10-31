#include "backend_events.hpp"

#include "core/threading/script.hpp"

#include "game/features/features.hpp"
#include "game/mhw/qol/qol_service.hpp"

namespace big
{
	void backend_events::features()
	{
		qol_service::tick();
	}

	// Only values that the game caches every frame need polling; every toggle is
	// a command whose on_enable/on_disable already did the work.
	void backend_events::player_event()
	{
		while (true)
		{
			TRY_CLAUSE
			{
				features::apply_critical_boost();
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
				features();
			}
			EXCEPT_CLAUSE
			script::get_current()->yield();
		}
	}
}
