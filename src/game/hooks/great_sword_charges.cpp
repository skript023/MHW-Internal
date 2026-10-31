#include "game/features/features.hpp"
#include "game/hooks/hooks.hpp"
#include "core/hooking/hook_manager.hpp"

#include "game/pointers.hpp"

namespace big
{
	int WINAPI game_hooks::great_sword_charges(__int64 a1, uint32_t a2, float a3)
	{
		if (features::_inst_chrgs_gs.get_state()) {}

		return base_hook::get_original<great_sword_charges>()(a1, a2, a3);
	}
}