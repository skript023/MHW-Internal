#include "game/hooks/hooks.hpp"
#include "core/hooking/hook_manager.hpp"
#include "game/settings.hpp"
#include "game/mhw/qol/qol_service.hpp"

namespace big
{
	__int64 __fastcall game_hooks::infinite_gathering(GatheringMonster* monster, __int64 a2)
	{
		if (monster)
		{
			if (g_settings.qol.instant_gathering)
			{
				qol_service::on_gather_node();
				if (monster->m_material_count < monster->m_body_count)
				{
					monster->m_material_count = monster->m_body_count;
				}
			}

			if (g_settings.player.inf_gathering)
			{
				if (monster->m_body_count != 0)
				{
					return base_hook::get_original<infinite_gathering>()(monster, a2);
				}

				auto temp = monster->m_material_count;
				monster->m_material_count = 0;
				monster->m_body_count = temp;
			}
		}

		return base_hook::get_original<infinite_gathering>()(monster, a2);
	}
}
