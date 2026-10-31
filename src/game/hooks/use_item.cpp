#include "game/features/features.hpp"
#include "game/hooks/hooks.hpp"
#include "core/hooking/hook_manager.hpp"
#include "game/classes/fwddec.hpp"

namespace big
{
	void __fastcall game_hooks::use_item(__int64 rcx, int a2, int8_t a3)
	{
		if (features::_inf_item.get_state() && rcx)
		{
			auto* slot = reinterpret_cast<ItemSlot*>(rcx);
			if (slot->m_item_id > 0xCC)
			{
				if (a3 != 0x01)
				{
					return;
				}
			}
			else if (a3 != 0x00)
			{
				return;
			}
		}

		return base_hook::get_original<game_hooks::use_item>()(rcx, a2, a3);
	}
}
