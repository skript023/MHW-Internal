#include "game/features/features.hpp"
#include "game/hooks/hooks.hpp"
#include "core/hooking/hook_manager.hpp"

namespace big
{
	bool __fastcall game_hooks::equipment_crafting(__int64 a1)
	{
		if (features::_unlock_equipment.get_state())
			return true;

		return base_hook::get_original<equipment_crafting>()(a1);
	}
}