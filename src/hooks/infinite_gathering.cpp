#include "hooking.hpp"

namespace big
{
	__int64 __fastcall hooks::infinite_gathering(GatheringMonster* monster, __int64 a2)
	{
		if (g_settings->player.inf_gathering)
		{
			if (monster->m_body_count != 0)
			{
				return base_hook::get_original<infinite_gathering>()(monster, a2);
			}

			auto temp = monster->m_material_count;
			monster->m_material_count = 0;
			monster->m_body_count = temp;
			temp = 0;
		}

		return base_hook::get_original<infinite_gathering>()(monster, a2);
	}
}